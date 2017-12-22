#include "Common.h"
#include "Game.h"

vector<string> split(const string& s, const char delim) 
{
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

void checkError(const char *file, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("%s:%d - GL Error: %s\n", file, line, gluErrorString(err));
}

void printVec3(vec3 p)
{
	printf("%f/%f/%f\n", p.x, p.y, p.z);
}

void printRect(SDL_Rect r)
{
	printf("(%f, %f, %f, %f)\n", r.x, r.y, r.w, r.h);
}

bool run = true;
bool paused = false;

int main(int argc, char *argv[])
{
	
	Game *game = new Game();
	game->Run();

    return 0;
}
