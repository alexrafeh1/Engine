#ifndef __SHADOW_RENDERER_H__
#define __SHADOW_RENDERER_H__ 1

#include "Motor/common/interface/render_interface.h"
#include "Motor/common/dev/program.h"
#include "Motor/common/dev/frame_buffer.h"

static constexpr int SHADOW_WIDTH = 2048;
static constexpr int SHADOW_HEIGHT = 2048;

class ShadowRenderer : public RenderInterface {
public:
	ShadowRenderer(std::shared_ptr<Program> shadow_program);
    ~ShadowRenderer() = default;

    const std::vector<ShadowMap>& GetDepthTexture() const
    {
        return shadow_maps_;
    }

	virtual void Render(RenderContex& context) override;
private:
    void RenderShadowCasters(RenderContex& render);

    void CreateShadowMap();

    std::vector<ShadowMap> shadow_maps_;
    std::shared_ptr<Program> shadow_program_;
    Framebuffer framebuffer_;
};

#endif // !__SHADOW_RENDERER_H__
