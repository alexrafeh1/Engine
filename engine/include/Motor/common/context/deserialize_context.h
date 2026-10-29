#ifndef __DESERIALIZE_CONTEXT_H__ 
#define __DESERIALIZE_CONTEXT_H__ 1

class MeshManager;
class ECSManager;

struct DeserializeContext
{
	MeshManager& meshManager;
	ECSManager& ecs;
};


#endif // !__DESERIALIZE_CONTEXT_H__ 



