#include <iostream>
#include "Motor/common/dev/motor.h"
#include "Motor/common/input/input.h"
#include "Motor/common/scene/scene.h"
#include "Motor/common/scene/scene_serializer.h"
#include "Motor/common/editor/editor.h"
#include "Motor/common/dev/frame_buffer.h"


int main() {
	std::optional<Motor> mo = Motor::create(1024, 1024);
	if (!mo.has_value())return 1;
	Motor m = std::move(mo.value());
	Window window = std::move(m.GetWindow());
	Input::Init(window.GetWindow());
	Editor editor;
	Scene scene;
	scene.Start();


	editor.Init(window.GetWindow());
	Framebuffer framebuffer;

	framebuffer.Create(1024, 1024,FramebufferTextureType::Color);

	while (!window.isClosed())
	{
		window.StartFrame();
	
		float dt = window.GetDtF();
	
		scene.Update(dt);

		editor.BeginFrame();
		editor.Update(framebuffer, scene, window);


		int width = framebuffer.GetWidth();
		int height = framebuffer.GetHeight();

		if (width > 0 && height > 0)
		{
			framebuffer.Bind();

			glViewport(0, 0, width, height);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			scene.Render();

			framebuffer.Unbind();
		}

		editor.Render();


		Input::Update();

		window.EndFrame();
	}
	//editor.Shutdown();

	return 0;
}

