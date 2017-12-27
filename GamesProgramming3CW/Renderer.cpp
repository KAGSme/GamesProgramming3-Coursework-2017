#include "Renderer.h"
#include "Game.h"

GLuint Renderer::activeProg = 0, Renderer::activeVao = 0;
GLuint Renderer::activeTexts[TEXTURE_COUNT] = { 0, 0, 0, 0, 0 };

Renderer::Renderer(Texture *t, bool cubeMap, ShaderProgram *s, Model *m, int mode)
{
	AddTexture(t);
	isCubeMap = cubeMap;
	shaderProg = s;
	model = m;
	renderMode = mode;
}

void Renderer::Ready()
{
	GLuint prog = shaderProg->Get();
	if (prog != activeProg)
	{
		glUseProgram(prog);
		activeProg = prog;
	}
}

void Renderer::Render()
{
	if (instancedRender)
	{
		RenderInstanced();
		return;
	}

	//and sending settings
	if (isCubeMap)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]->Get());

		int id = 0;
		shaderProg->SetUniform("skybox", &id);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[1]->Get());

		int id2 = 1;
		shaderProg->SetUniform("skyboxNight", &id2);
	}
	else 
	{
		for (int i = 0; i < textCount; i++)
		{
			GLuint text = textures[i]->Get();
			//if (text != activeTexts[i])
			//{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, text);

				string name = string("texture") + (char)(48 + i);
				shaderProg->SetUniform(name, &i);
				activeTexts[i] = text;
			//}
		}
	}
	CHECK_GL_ERROR();

	//binding the vao
	GLuint vao = model->Get();
	if (vao != activeVao)
	{
		glBindVertexArray(vao);
		activeVao = vao;
	}
	CHECK_GL_ERROR();

	//now on to draw stuff
	switch (renderMode)
	{
	case GL_TRIANGLES:
		glDrawElements(GL_TRIANGLES, model->GetIndCount(), GL_UNSIGNED_INT, 0);
		break;
	case GL_TRIANGLE_FAN:
		glDrawArrays(GL_TRIANGLE_STRIP, 0, model->GetVertCount());
		break;
	default:
		printf("Render mode unsupported!\n");
		break;
	}
	CHECK_GL_ERROR();

	Game::verticesRendered += model->GetVertCount();
	Game::drawCalls++;
}

void Renderer::RenderInstanced()
{
	for (int i = 0; i < textCount; i++)
	{
		GLuint text = textures[i]->Get();
		if (text != activeTexts[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, text);

			string name = string("texture") + (char)(48 + i);
			GLint loc = glGetUniformLocation(shaderProg->Get(), name.c_str());
			glUniform1i(loc, i);

			activeTexts[i] = text;
		}
	}

	//binding the vao
	GLuint vao = model->Get();
	if (vao != activeVao)
	{
		glBindVertexArray(vao);
		activeVao = vao;
	}

	switch (renderMode)
	{
	case GL_TRIANGLES:
		glDrawElementsInstanced(GL_TRIANGLES, model->GetIndCount(), GL_UNSIGNED_INT, 0, instanceCount);
		break;
	case GL_TRIANGLE_FAN:
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, model->GetVertCount(), instanceCount);
		break;
	default:
		printf("Render mode unsupported!");
		break;
	}
	CHECK_GL_ERROR();

	Game::verticesRendered += model->GetVertCount() * instanceCount;
	Game::drawCalls++;
}

void Renderer::SetInstanceMatrices(vector<mat4> *matrices)
{
	instancedRender = true;
	instanceCount = matrices->size();

	if (matVbo == 0)
	{
		glBindVertexArray(model->Get());
		glGenBuffers(1, &matVbo);
		glBindBuffer(GL_ARRAY_BUFFER, matVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * instanceCount, NULL, GL_STATIC_DRAW);
		/*glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
		glVertexAttribDivisor(4, 1);*/
		for (int i = 0; i < 4; i++)
		{
			glEnableVertexAttribArray(4 + i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(i * sizeof(vec4)));
			glVertexAttribDivisor(4 + i, 1);
		}
		glBindVertexArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, matVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4) * instanceCount, matrices->data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}