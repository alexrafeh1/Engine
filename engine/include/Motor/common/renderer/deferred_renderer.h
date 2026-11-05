#ifndef __DEFERRED_RENDERER_H__ 
#define __DEFFERED_RENDERER_H__ 1

#include "Motor/common/components/light_component.h"
#include "Motor/common/interface/render_interface.h"
#include "Motor/common/dev/program.h"


class DefferedRendering : public RenderInterface {
public:

	DefferedRendering(std::shared_ptr<Program> buffer_program, std::shared_ptr<Program> light_program, std::shared_ptr<Program> SSR_program);
	virtual void Render(RenderContex& render) override;

private:
	void RenderGBuffer(RenderContex& render);
	void RenderLights(RenderContex& render);
	void SetLightUniforms(LightComponent* light);

	void RenderSSR(RenderContex& render);

	std::unique_ptr<Framebuffer> G_Buffer_;
	std::unique_ptr<Framebuffer> Light_Buffer_;

	std::shared_ptr<Program> G_Buffer_Program_;
	std::shared_ptr<Program> Light_Program_;
	std::shared_ptr<Program> SSR_Program_;


	GLuint VAO, VBO, EBO;
};

#endif // !__FORWARD_RENDERER_H__ 
