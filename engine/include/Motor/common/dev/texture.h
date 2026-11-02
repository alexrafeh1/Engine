#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

#include <glad/gl.h>
#include <string>
#include <optional>


struct aiTexture;

class Texture {

public:
	Texture();
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other)noexcept;
	Texture& operator=(Texture&& other)noexcept;

	static std::optional<Texture> LoadTexture(std::string_view path);
	static std::optional<Texture> LoadEmbeddedTexture(const aiTexture* embedded);


	const unsigned int& GetID() const {
		return id_;
	}

	const std::string& GetName() {
		return name_;
	}

	void SetName(std::string name) {
		name_ = name;
	}

	void Bind(unsigned int slot)const {
		glBindTextureUnit(slot, id_);
	}

private:
	std::string name_;
	unsigned int id_;
};


#endif // !__TEXTURE_H__
