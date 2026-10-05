#ifndef __BUFFER_H__
#define __BUFFER_H__ 1

#include <glad/gl.h>

class Buffer {
public:
	Buffer();
	~Buffer();

	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	Buffer(Buffer&& other);
	Buffer& operator=(Buffer&& other);

	void BindVertexBuffer(GLsizei stride, unsigned int vao);

	void BindIndexBuffer(unsigned int vao);

	void DrawIndexed(GLenum mode, GLsizei indexCount, unsigned int vao);

	void UploadBuffer(const void* data, unsigned int size_data, GLenum buffer_type);

	void VertexAttributePointer(GLuint index, GLint components, GLenum type, bool normalized, size_t offset, unsigned int vao);

	unsigned int& GetVBO() {
		return VBO_;
	}

	unsigned int& GetEBO() {
		return EBO_;
	}
private:
	unsigned int VBO_, EBO_;
};

#endif // !__BUFFER_H__
