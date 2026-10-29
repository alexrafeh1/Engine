#include "Motor/common/component_data/material_data.h"
#include "Motor/common/dev/shader.h"

MaterialData::MaterialData()
{
    color_ = { 1.0f,1.0f,1.0f,1.0f };
}

MaterialData::~MaterialData() = default;

MaterialData::MaterialData(MaterialData&& other) noexcept
    : color_(other.color_)
{
}

MaterialData& MaterialData::operator=(MaterialData&& other) noexcept
{
    if (this != &other)
    {
        color_ = other.color_;
    }

    return *this;
}

