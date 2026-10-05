#ifndef __MESH_LOADER_H__ 
#define __MESH_LOADER_H__ 1


#include "Motor/common/component_data/mesh_data.h"
#include "memory"
#include <string>
#include "Motor/common/dev/texture.h"

struct aiNode;
struct aiScene;
struct aiMesh;

struct ModelSource
{
	std::vector<MeshSource> meshes;
	std::vector<Texture> textures;

	std::string path;
};

class MeshLoader {
public:
	MeshLoader() = default;
	~MeshLoader() = default;

    static ModelSource LoadMesh(const std::string& path);
	
	static void LoadTextures(const struct aiScene* scene, ModelSource& meshes, const std::string& path);
private:
	static void ProcessNode(struct aiNode* node, const struct aiScene* scene, std::vector<MeshSource>& meshes);

	static MeshSource ProcessMesh(struct aiMesh* mesh);
};

#endif // !__MESH_LOADER_H__ 
