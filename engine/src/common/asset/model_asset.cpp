#include "Motor/common/asset/model_asset.h"
#include "Motor/common/component_data/mesh_data.h"
#include "Motor/common/dev/texture.h"

void ModelAsset::AddMesh(std::shared_ptr<MeshData> mesh)
{
	meshes_.push_back(std::move(mesh));
}

void ModelAsset::AddTexture(Texture tex)
{
	textures_.push_back(std::move(tex));
}

const std::vector<std::shared_ptr<MeshData>>& ModelAsset::GetMeshes() const
{
	return meshes_;
}

const std::vector<Texture>& ModelAsset::GetTextures() const
{
	return textures_;
}
