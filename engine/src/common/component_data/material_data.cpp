#include "Motor/common/component_data/material_data.h"
#include "Motor/common/dev/shader.h"

MaterialData::MaterialData()
{
    color_ = { 1.0f,1.0f,1.0f,1.0f };
    roughness_ = 0.3f;
    metallic_ = 0.7f;
    ao_ = 0.2f;
}

MaterialData::~MaterialData() = default;

MaterialData::MaterialData(MaterialData&& other) noexcept
    : color_(other.color_),roughness_(other.roughness_),metallic_(other.metallic_),ao_(other.ao_)
{
}

MaterialData& MaterialData::operator=(MaterialData&& other) noexcept
{
    if (this != &other)
    {
        color_ = other.color_;
        roughness_ = other.roughness_;
        metallic_ = other.metallic_;
        ao_ = other.ao_;
    }

    return *this;
}

