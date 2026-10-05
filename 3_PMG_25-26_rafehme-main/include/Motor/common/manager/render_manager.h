#ifndef __RENDER_MANAGER_H__
#define __RENDER_MANAGER_H__ 1

#include <vector>
#include <memory>
#include "Motor/common/interface/render_interface.h"

class RenderManager {
public:

	void Add(std::unique_ptr<RenderInterface> pass) {
		render_manager_.push_back(std::move(pass));
	}

	void Render(RenderContex& context) {
		for (auto& render_pass : render_manager_) {
			render_pass->Render(context);
		}
	}

private:
	std::vector<std::unique_ptr<RenderInterface>> render_manager_;
};

#endif // !__RENDER_MANAGER_H__
