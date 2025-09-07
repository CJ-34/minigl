#pragma once

#include <cstdint>

namespace minigl {
	struct Vertex {
		float px, py, pz, nx, ny, nz, u, v;
	};

	class VertexBuffer {
	public:
		VertexBuffer(const void* buffer, size_t bytes);
		~VertexBuffer();
		void bind() const;
		unsigned int m_id;
	};

	class IndexBuffer {
	public:
		IndexBuffer(const uint32_t data, size_t count);
		~IndexBuffer();
		void bind() const;
		size_t count() const;
	private:
		unsigned int m_id;
		size_t count;
	};
}