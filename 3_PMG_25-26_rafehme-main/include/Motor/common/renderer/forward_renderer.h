#ifndef __FORWARD_RENDERER_H__ 
#define __FORWARD_RENDERER_H__ 1

#include "Motor/common/components/light_component.h"
#include "Motor/common/interface/render_interface.h"
#include "Motor/common/dev/program.h"


class ForwardRendering : public RenderInterface {
public:

	ForwardRendering(std::shared_ptr<Program> program);
	virtual void Render(RenderContex& render) override;

private:

	void UploadLights(RenderContex& render);
	void RenderGeometry(RenderContex& render);

	LightBuffer lightBuffer_;
	std::shared_ptr<Program> lights_program_;
};

#endif // !__FORWARD_RENDERER_H__ 
