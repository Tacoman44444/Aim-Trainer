#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <openglErrorReporting.h>
#include "SetupHelper.h"
#undef main

#pragma region imgui
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguiThemes.h"
#pragma endregion

void resize_gl_viewport(const SDL_Event& event) {
	if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
		int width = event.window.data1;
		int height = event.window.data2;
		glViewport(0, 0, width, height);
	}
}

int main(int argc, char *argv[])
{
	float vert[] = {	//cube
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	if (!SetupHelper::InitializeSDL()) {
		std::cout << "SDL initialization failed" << std::endl;
		return 1;
	}
	SDL_Window* window;
	SetupHelper::CreateSDLWindow(&window);
	SDL_GLContext glContext;
	SetupHelper::CreateGLContext(window, &glContext);
	SetupHelper::LoadGLFunctions();

	enableReportGlErrors();

	Texture containerTexture("C:/Users/justf/source/repos/Aim Trainer/SDL2OpenglCMakeSetup-master/resources/media/container.jpg");

	Object sampleObject(Models::vertices, sizeof(Models::vertices));
	sampleObject.InitializeBuffers();
	sampleObject.InitializeShaders(RESOURCES_PATH "shaders/basic_vert_shader.vert", RESOURCES_PATH "shaders/basic_frag_shader.frag");
	
	Camera camera(glm::vec3(0.0f, 0.0f, -3.0f), 0.1f, 0.01f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));
	model = glm::translate(model, glm::vec3(0.0f, -200.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	sampleObject.SetTextures();
	SDL_Event e;
	bool running = true;

	while (running) {
		int x, y;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			else if (e.type == SDL_WINDOWEVENT) {
				resize_gl_viewport(e);
				if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_w:
					camera.SetDirection(FORWARD);
					break;

				case SDLK_a:
					camera.SetDirection(LEFT);
					break;

				case SDLK_s:
					camera.SetDirection(BACKWARD);
					break;

				case SDLK_d:
					camera.SetDirection(RIGHT);
					break;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_w:
					camera.ResetDirection(FORWARD);
					break;

				case SDLK_a:
					camera.ResetDirection(LEFT);
					break;

				case SDLK_s:
					camera.ResetDirection(BACKWARD);
					break;

				case SDLK_d:
					camera.ResetDirection(RIGHT);
					break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION) {
				x = e.motion.xrel;
				y = e.motion.yrel;
				camera.Look(static_cast<float>(x), static_cast<float>(y));
			}
		}

		camera.Move();

		glm::mat4 view = camera.GetViewMatrix();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		containerTexture.BindTexture();
		
		sampleObject.BindObject();
		sampleObject.DrawObject(projection, view, model);

		glBindVertexArray(0);
		SDL_GL_SwapWindow(window);

	}

	// Cleanup SDL
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
