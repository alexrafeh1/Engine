#include "Motor/common/component_data/mesh_data.h"

MeshData::MeshData()
{
    texture_index_ = 0;
}

MeshData::~MeshData()
{

}

MeshData::MeshData(MeshData&& other) noexcept
{
	buffer_ = std::move(other.buffer_);
	VAO_ = other.VAO_;
	vertex_count_ = other.vertex_count_;
    texture_index_ = other.texture_index_;
}

MeshData& MeshData::operator=(MeshData&& other) noexcept
{
	buffer_ = std::move(other.buffer_);
	VAO_ = other.VAO_;
	vertex_count_ = other.vertex_count_;
    texture_index_ = other.texture_index_;
	return *this;
}

std::optional<MeshData> MeshData::CreateMesh(
    const MeshSource& source
)
{
    if (source.vertices.empty())
        return std::nullopt;

    MeshData meshData;

    meshData.texture_index_ = source.textureIndex;

    meshData.buffer_ = std::make_unique<Buffer>();

    glCreateVertexArrays(
        1,
        &meshData.VAO_
    );


    meshData.buffer_->UploadBuffer(
        source.vertices.data(),
        source.vertices.size() * sizeof(Vertex),
        GL_ARRAY_BUFFER
    );

    meshData.buffer_->BindVertexBuffer(
        sizeof(Vertex),
        meshData.VAO_
    );

    meshData.buffer_->UploadBuffer(
        source.indices.data(),
        source.indices.size() * sizeof(uint32_t),
        GL_ELEMENT_ARRAY_BUFFER
    );

    meshData.buffer_->BindIndexBuffer(
        meshData.VAO_
    );

  

    meshData.buffer_->VertexAttributePointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, position),
        meshData.VAO_
    );

    meshData.buffer_->VertexAttributePointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, normals),
        meshData.VAO_
    );

    meshData.buffer_->VertexAttributePointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, uv),
        meshData.VAO_
    );

    meshData.buffer_->VertexAttributePointer(
        3,
        4,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, tangent),
        meshData.VAO_
    );

    meshData.vertex_count_ =
        static_cast<GLsizei>(
            source.indices.size()
            );

    return meshData;
}