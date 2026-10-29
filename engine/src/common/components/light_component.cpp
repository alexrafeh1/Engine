#include "Motor/common/components/light_component.h"
#include "Motor/common/manager/ecs_manager.h"


LightBuffer::LightBuffer()
{
    glCreateBuffers(1, &buffer_);
}

LightBuffer::~LightBuffer()
{
    if (buffer_)
        glDeleteBuffers(1, &buffer_);
}

void LightBuffer::Upload(
    const std::vector<GPULight>& lights)
{
    count_ = static_cast<GLuint>(lights.size());

    if (lights.empty())
        return;

    glNamedBufferData(buffer_, static_cast<GLsizeiptr>(lights.size() * sizeof(GPULight)), lights.data(),GL_DYNAMIC_DRAW);
}

void LightBuffer::Bind(GLuint binding) const
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,binding,buffer_);
}


void LightComponent::Serialize(nlohmann::json& json) {
    json["LightComponent"] = "";
}
void LightComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {

}