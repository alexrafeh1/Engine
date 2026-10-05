#ifndef __EDITOR_PANELS_H__
#define __EDITOR_PANELS_H__ 1

class Scene;
class Framebuffer;
class Window;

#include "Motor/common/components/transform_component.h"


struct EditorContext
{
	Scene& scene;
	Framebuffer& viewport_framebuffer;
	Window& window;
	size_t& selected_entity;
};


class IEditorPanels {
public:
	virtual ~IEditorPanels() = default;
	virtual void DrawPanel(EditorContext& editor_ctx) = 0;
};



class HierarchyPanel : public IEditorPanels {
public:
	virtual void DrawPanel(EditorContext& editor_ctx) override;
};


class ViewportPanel : public IEditorPanels {
public:
	virtual void DrawPanel(EditorContext& editor_ctx) override;
};

class ComponentsPanel : public IEditorPanels {
public:
	virtual void DrawPanel(EditorContext& editor_ctx) override;
};

#endif // !__EDITOR_PANELS_H__
