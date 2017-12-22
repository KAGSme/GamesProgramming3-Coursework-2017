#include "Font.h"

Font::Font(const string& fileName)
{
	guiCam = new Camera(true);
	vertices = new vector<Vertex>();
	indices = new vector<int>();

	TTF_Font *font = TTF_OpenFont(fileName.c_str(), 64);
	if (!font)
		printf("Unable to load font %s-%s\n", fileName.c_str(), TTF_GetError());

	//first of all, need to render out the text atlas to use for vertex rendering
	SDL_Surface *atlasSurf = SDL_CreateRGBSurface(0, width, height, 32, 
								0xFF000000, 0xFF0000, 0xFF00, 0xFF);
	//to track where to put next glyphs
	int x = 0, y = 0;
	//marks where the newline starts
	int newY = 0; 
	//32 being space, 126 being ~, all visible characters
	for (uint glyph = 32; glyph < 127; glyph++)
	{
		SDL_Surface *glyphSurf = TTF_RenderGlyph_Blended(font, glyph, {255, 255, 255});
		int w = glyphSurf->w, h = glyphSurf->h;

		if (x + w > width) //if we can't go right anymore
		{
			x = 0; //start from left
			y += newY; //on a new line
			newY = 0; //start looking for a new newline offset
		}
		
		SDL_Rect r = { x, y, w, h };
		SDL_BlitSurface(glyphSurf, NULL, atlasSurf, &r);
		RECTF rect = { x, y, w, h - 4 }; //adding a 4pixel boundary from the bottom
		rects[glyph - 32] = rect; //saving the unnormalized UVs
		x += w;
		newY = glm::max(newY, h);
		SDL_FreeSurface(glyphSurf);
	}

	//now normalizing the UVs
	int count = 127 - 32;
	for (int i = 0; i < count; i++)
	{
		RECTF r = rects[i];
		r.x /= (float)width;
		r.y /= (float)height;
		r.w /= (float)width;
		r.h /= (float)height;
		rects[i] = r;
	}

	//for debug
	SDL_SaveBMP(atlasSurf, "atlas.bmp");

	GLuint atlasText = ConvertSDLSurfaceToTexture(atlasSurf);
	SDL_FreeSurface(atlasSurf);
	TTF_CloseFont(font);

	//saving the previous state
	GLint currActive;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &currActive);

	//pushing our texture and setting standard settings
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlasText);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//restoring it
	glActiveTexture(currActive);

	m = new Model();
	m->SetUpAttrib(0, 3, GL_FLOAT, 0);
	m->SetUpAttrib(1, 2, GL_FLOAT, sizeof(vec3) + sizeof(vec4));

	shader = new ShaderProgram(SHADER_PATH + "guiVS.glsl", SHADER_PATH + "guiFS.glsl");
	shader->BindAttribLoc(0, "vertexPosition");
	shader->BindAttribLoc(1, "uvs");
	shader->Link();
	t = new Texture(atlasText);
	renderer = new Renderer();
	renderer->SetModel(m, GL_TRIANGLES);
	renderer->SetShaderProgram(shader);
	renderer->AddTexture(t);
}
 
Font::~Font()
{
	delete t;
	delete m;
	delete renderer;
	delete guiCam;
}

void Font::Render(const string& text, const SDL_Rect rect)
{
	//avoiding extra cpu load by ignoring the lines (since it's gonna be cleared anyway)
	if (!preRenderStage)
		return;

	//first, calculate the amount of newlines
	vector<string> lines;
	int size = text.size();
	int offset = 0;
	int maxLength = 0; //will be used to determine the width of character
	for (int i = 0; i <= size; i++)
	{
		char c = text[i];
		if (c == '\n' || i == size)
		{
			string line = text.substr(offset, i - offset);
			int length = line.length();
			maxLength = glm::max(maxLength, length);
			lines.push_back(line);
			offset = i;
		}
	}

	//dimensions of a character
	int width = rect.w / maxLength;
	int height = rect.h / lines.size();

	for (int y = 0; y < lines.size(); y++)
	{
		string line = lines[y];
		for (int x = 0; x < line.size(); x++)
		{
			char c = line[x];
			RECTF r = rects[c - 32];
			Vertex v;
			//bottom left
			v.pos = vec3(rect.x + x * width, rect.y + y * height, 0);
			v.texture = vec2(r.x, r.y + r.h);
			vertices->push_back(v);

			//bottom right
			v.pos = vec3(rect.x + (x + 1) * width, rect.y + y * height, 0);
			v.texture = vec2(r.x + r.w, r.y + r.h);
			vertices->push_back(v);

			//top left
			v.pos = vec3(rect.x + x * width, rect.y + (y + 1) * height, 0);
			v.texture = vec2(r.x, r.y);
			vertices->push_back(v);

			//top right
			v.pos = vec3(rect.x + (x + 1) * width, rect.y + (y + 1) * height, 0);
			v.texture = vec2(r.x + r.w, r.y);
			vertices->push_back(v);

			//pushing the required indices to form quads
			int vertsStartInd = vertices->size() - 4;
			//winding order matters, turns out >.>
			indices->push_back(vertsStartInd + 2); //top left
			indices->push_back(vertsStartInd); //bottom left
			indices->push_back(vertsStartInd + 1); //bottom right
			indices->push_back(vertsStartInd + 1); //bottom right
			indices->push_back(vertsStartInd + 3); //top right
			indices->push_back(vertsStartInd + 2); //top left
		}
	}
}

void Font::Flush(float deltaTime)
{
	//flush all of the vertices to the GPU for drawing
	if (!preRenderStage)
	{
		timer += deltaTime;
		if (timer > 1.f / 60.f) //60fps text updating
		{
			timer = 0;
			preRenderStage = true;

			//clear out the memory to start rendering new ones
			vertices = new vector<Vertex>();
			indices = new vector<int>();
		}
	}
	else
	{
		m->SetVertices(vertices, GL_STREAM_DRAW, true);
		m->SetIndices(indices, GL_STREAM_DRAW, true);
		m->FlushBuffers();
		preRenderStage = false;
	}

	glEnable(GL_BLEND);
	renderer->Ready();
	ShaderProgram *prog = renderer->GetProgram();
	prog->SetUniform("MVP", (void*)value_ptr(guiCam->Get()));
	renderer->Render();
	glDisable(GL_BLEND);
}

GLuint Font::ConvertSDLSurfaceToTexture(SDL_Surface *surf)
{
	GLint nColors = surf->format->BytesPerPixel;
	GLenum textureFormat;
	GLenum internalFormat;

	if (nColors == 4) //contains alpha
	{
		if (surf->format->Rmask == 0xff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
		internalFormat = GL_RGBA8;
	}
	else if (nColors == 3) //no alpha
	{
		if (surf->format->Rmask == 0xff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
		internalFormat = GL_RGB8;
	}
	else //no idea what format it is
	{
		printf("Unsupported format!");
		return 0;
	}

	GLuint textId = 0;
	glGenTextures(1, &textId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textId);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surf->w, surf->h, 0,
		textureFormat, GL_UNSIGNED_BYTE, surf->pixels);

	return textId;
}