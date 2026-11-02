#ifndef __MATERIAL_DATA_H__ 
#define __MATERIAL_DATA_H__ 1

#include <vector>
#include "Motor/common/dev/texture.h"
#include "../deps/glm/glm.hpp"
#include "memory"
#include "string.h"


class MaterialData {
public:
	MaterialData();
	~MaterialData();

	MaterialData(const MaterialData& other) = default;
	MaterialData& operator=(const MaterialData& other) = default;

	MaterialData(MaterialData&& other) noexcept;
	MaterialData& operator=(MaterialData&& other)noexcept;



	glm::vec4& GetColor() {
		return color_;
	}

	float& GetRoughness() {
		return roughness_;
	}

	float& GetMetallic() {
		return metallic_;
	}

	float& GetAO() {
		return ao_;
	}

private:
	glm::vec4 color_;
	float roughness_;
	float metallic_;
	float ao_;

};


#endif // !__MATERIAL_DATA_H__ 1
