#ifndef __RENDER_INTERFACE_H__
#define __RENDER_INTERFACE_H__ 1

#include "Motor/common/dev/camera.h"
#include "Motor/common/manager/ecs_manager.h"
#include <glad/gl.h>
#include "Motor/common/dev/frame_buffer.h"

struct ShadowMap
{
	Framebuffer framebuffer;
	glm::mat4 light_space_matrix{ 1.0f };
};

struct RenderContex
{
	ECSManager* ecs = nullptr;
	std::vector<ShadowMap> shadow_maps;
};

class RenderInterface {
public:

	virtual ~RenderInterface() = default;

	virtual void Render(RenderContex& context) = 0;
};

#endif // !__RENDER_INTERFACE_H__
