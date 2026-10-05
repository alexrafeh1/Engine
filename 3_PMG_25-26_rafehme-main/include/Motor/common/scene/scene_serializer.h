#ifndef __SCENE_SERIALIZER_H__
#define __SCENE_SERIALIZER_H__ 1

#include <string>

class ECSManager;


class SceneSerializer {
public:
	 bool Save(const ECSManager& ecs,const std::string& path);
	 bool Load(ECSManager& ecs,const std::string& path,class MeshManager& mesh_manager);
};


#endif // !__SCENE_SERIALIZER_H
