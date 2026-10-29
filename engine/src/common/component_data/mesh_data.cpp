#include "Motor/common/component_data/mesh_data.h"

MeshData::MeshData()
{
    texture_index_ = 0;
}

MeshData::~MeshData()
{
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
    }
}

MeshData::MeshData(MeshData&& other) noexcept
    : buffer_(std::move(other.buffer_)),
    VAO_(std::exchange(other.VAO_, 0)),
    vertex_count_(std::exchange(other.vertex_count_, 0)),
    texture_index_(std::exchange(other.texture_index_, 0))
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
    VAO_ = std::exchange(other.VAO_, 0);
    vertex_count_ = std::exchange(other.vertex_count_, 0);
    texture_index_ = std::exchange(other.texture_index_, 0);

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

    meshData.buffer_->UploadBuffer(
        source.vertices.data(),
        source.vertices.size() * sizeof(Vertex),
        GL_ARRAY_BUFFER
    );

    meshData.buffer_->UploadBuffer(
        source.indices.data(),
        source.indices.size() * sizeof(uint32_t),
        GL_ELEMENT_ARRAY_BUFFER
    );

    meshData.vertex_count_ =
        static_cast<GLsizei>(
            source.indices.size()
            );

    return meshData;
}

void MeshData::CreateVAO()
{
    glCreateVertexArrays(
        1,
        &VAO_
    );

    buffer_->BindVertexBuffer(
        sizeof(Vertex),
        VAO_
    );

    buffer_->BindIndexBuffer(
        VAO_
    );

    buffer_->VertexAttributePointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, position),
        VAO_
    );

    buffer_->VertexAttributePointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, normals),
        VAO_
    );

    buffer_->VertexAttributePointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, uv),
        VAO_
    );

    buffer_->VertexAttributePointer(
        3,
        4,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, tangent),
        VAO_
    );
}