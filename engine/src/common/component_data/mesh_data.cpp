#include "Motor/common/component_data/mesh_data.h"

MeshData::MeshData()
{
}

MeshData::~MeshData()
{
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
    }
}

MeshData::MeshData(MeshData&& other) noexcept
    : buffer_(std::move(other.buffer_)),
    VAO_(std::move(other.VAO_)),
    vertex_count_(std::move(other.vertex_count_)),
    textures_(std::move(other.textures_))
{
}

MeshData& MeshData::operator=(MeshData&& other) noexcept
{
    if (this == &other)
        return *this;

    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
    }

    buffer_ = std::move(other.buffer_);
    VAO_ = std::move(other.VAO_);
    vertex_count_ = std::move(other.vertex_count_);
    textures_ = std::move(other.textures_);
    return *this;
}

std::optional<MeshData> MeshData::CreateMesh(MeshSource& source)
{
    if (source.vertices.empty())
        return std::nullopt;

    MeshData meshData;

    meshData.buffer_ = std::make_unique<Buffer>();
    meshData.buffer_->UploadBuffer(source.vertices.data(), source.vertices.size() * sizeof(Vertex), GL_ARRAY_BUFFER);
    meshData.buffer_->UploadBuffer( source.indices.data(), source.indices.size() * sizeof(uint32_t), GL_ELEMENT_ARRAY_BUFFER);

    meshData.textures_ = std::move(source.textures);

    meshData.vertex_count_ = static_cast<GLsizei>(source.indices.size());

    return meshData;
}

void MeshData::CreateVAO()
{
    glCreateVertexArrays(1,&VAO_);

    buffer_->BindVertexBuffer( sizeof(Vertex),  VAO_);

    buffer_->BindIndexBuffer( VAO_);

    buffer_->VertexAttributePointer( 0, 3, GL_FLOAT, GL_FALSE,offsetof(Vertex, position),VAO_);
    buffer_->VertexAttributePointer( 1, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, normals),VAO_);
    buffer_->VertexAttributePointer( 2, 2, GL_FLOAT, GL_FALSE,offsetof(Vertex, uv),VAO_);
    buffer_->VertexAttributePointer( 3,4,GL_FLOAT,GL_FALSE,offsetof(Vertex, tangent),VAO_);
}