#ifndef __MESH_DATA_H__
#define __MESH_DATA_H__

#include "Motor/common/dev/buffer.h"
#include <memory>
#include <optional>
#include "../deps/glm/glm.hpp"
#include <vector>
#include "Motor/common/dev/texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 uv;
	glm::vec3 tangent;

	Vertex(glm::vec3 p = {1.0f,1.0f,1.0f}, glm::vec3 n = {1.0f,1.0f,1.0f}, glm::vec2 u = {1.0f,1.0f}, glm::vec3 c = {1.0f,1.0f,1.0f}) {
		position = p;
		normals = n;
		uv = u;
		tangent = c;
	}
};

using VertexList = std::vector<Vertex>;
using IndexList = std::vector<uint32_t>;


struct MeshSource
{
	VertexList vertices;
	IndexList indices;

	uint32_t textureIndex = 0;
};

class MeshData {
public:
	MeshData();
	~MeshData();

	MeshData(const MeshData& other) = delete;
	MeshData& operator=(const MeshData& other) = delete;

	MeshData(MeshData&& other)noexcept;
	MeshData& operator=(MeshData&& other)noexcept;

	static std::optional<MeshData> CreateMesh(const MeshSource& source);

	unsigned int& GetVao() {
		return VAO_;
	}

	GLsizei GetCount() {
		return vertex_count_;
	}

	uint32_t GetTextureIndex() {
		return texture_index_;
	}

	std::unique_ptr<Buffer> buffer_;
	unsigned int VAO_;
	GLsizei vertex_count_;
	uint32_t texture_index_;
private:
};

#endif // !__MESH_DATA_H__
