#include <iostream>
#include "Motor/common/dev/motor.h"
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


	int width;
	int height;

	Scene scene(window.GetWindow());
	scene.Start();


	editor.Init(window.GetWindow());
	Framebuffer framebuffer;

	printf("%d",framebuffer.Create(1024, 1024, FramebufferTextureType::Color));

	while (!window.isClosed())
	{
		window.StartFrame();
	
		float dt = window.GetDtF();
	
		scene.Update(dt);


		width = framebuffer.GetWidth();
		height = framebuffer.GetHeight();


		editor.BeginFrame();
		editor.Update(framebuffer, scene, window);

		if (width > 0 && height > 0)
		{
			framebuffer.Bind();

			glViewport(0, 0, width, height);
			scene.Render();

			framebuffer.Unbind();
		}




		glDisable(GL_BLEND);
		editor.Render();


		Input::Update();

		window.EndFrame();
	}
	//editor.Shutdown();

	return 0;
}

