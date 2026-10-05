#include <Motor/common/dev/buffer.h>


Buffer::Buffer() {
	VBO_ = EBO_ = -1;
	glCreateBuffers(1, &VBO_);
	glCreateBuffers(1, &EBO_);
}

Buffer::~Buffer()
{
	if (VBO_) glDeleteBuffers(1, &VBO_);
	if (EBO_) glDeleteBuffers(1, &EBO_);
}

Buffer::Buffer(Buffer&& other) : 
 VBO_{ other.VBO_ }, EBO_{ other.EBO_ } {
	other.VBO_ = 0;
	other.EBO_ = 0;
}
Buffer& Buffer::operator=(Buffer&& other)
{
	if (this != &other) {
		if (VBO_) glDeleteBuffers(1, &VBO_);
		if (EBO_) glDeleteBuffers(1, &EBO_);

		VBO_ = other.VBO_;
		EBO_ = other.EBO_;

		other.VBO_ = 0;
		other.EBO_ = 0;
	}
	return *this;
}




void Buffer::BindVertexBuffer(GLsizei stride, unsigned int vao) {
	glVertexArrayVertexBuffer(vao, 0, VBO_, 0, stride);
}

void Buffer::BindIndexBuffer(unsigned int vao) {
	glVertexArrayElementBuffer(vao, EBO_);
}

void Buffer::UploadBuffer(const void* data, unsigned int size_data,GLenum buffer_type) {
	
	if (buffer_type == GL_ARRAY_BUFFER) {
		glNamedBufferData(VBO_, size_data, data, GL_STATIC_DRAW);
	}

	if (buffer_type == GL_ELEMENT_ARRAY_BUFFER) {
		glNamedBufferData(EBO_, size_data, data, GL_STATIC_DRAW);
	}
	
}

void Buffer::DrawIndexed(GLenum mode, GLsizei indexCount, unsigned int vao)
{
	glBindVertexArray(vao);
	glDrawElements(mode, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Buffer::VertexAttributePointer(GLuint index,GLint components,GLenum type,bool normalized,size_t offset,unsigned int vao) {

	glEnableVertexArrayAttrib(vao, index);

	glVertexArrayAttribFormat(
		vao,
		index,
		components,
		type,
		normalized,
		offset
	);

	glVertexArrayAttribBinding(vao, index, 0);
}


