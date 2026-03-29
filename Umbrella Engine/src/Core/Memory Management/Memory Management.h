#pragma once
#include <cstddef> // for size_t
#include <iostream>

// M : Memory
// M : Management
// U : Unit

namespace MMU {
	class StackAllocator
	{
	public:
		StackAllocator(size_t size) : m_size(size) {
			m_buffer = (unsigned char*)malloc(m_size);
			if(!m_buffer) 
				throw std::bad_alloc();
			m_top = m_buffer;
		}

		~StackAllocator() {
			if (m_buffer)
				free(m_buffer);
		}

		void* allocate(size_t size, size_t alignment = 8) {
			unsigned char* top_ptr = m_top;
			unsigned char* aligned_top = (unsigned char*)(((size_t)top_ptr + alignment - 1) & ~(alignment - 1));

			size_t required_space = (aligned_top - top_ptr) + size;

			if (m_top + required_space > m_buffer + m_size) {
				throw std::bad_alloc();
			}

			m_top = aligned_top + size;

			return (void*)aligned_top;
		}

		void reset() {
			m_top = m_buffer;
		}

	private:
		size_t m_size;
		unsigned char* m_buffer;
		unsigned char* m_top;
	};

	struct Node {
		Node* befor;
		unsigned char* ptr;
		bool free = true;
	};

	class PoolAllocator
	{
	public:
		PoolAllocator(size_t size, size_t c_size = 8) : m_size(size), Chank_size(c_size) {
			m_buffer = (unsigned char*)malloc(m_size);
			if (!m_buffer)
				throw std::bad_alloc();
			n_chank = m_size / Chank_size;
			endnode.befor = nullptr;
			endnode.ptr = m_buffer + (n_chank - 1) * Chank_size;
			Node* currentnode = &endnode;
			for (int i = 0; i < n_chank; i++) {
				unsigned char* chank_ptr = m_buffer + (n_chank - i) * Chank_size;
				insertAtBeginning(currentnode, chank_ptr);
				currentnode = currentnode->befor;
			}
		}

		~PoolAllocator() {
			if (m_buffer)
				free(m_buffer);
		}

		void* allocate() {
			Node* currentnode = &endnode;
			for (int i = 0; i < n_chank; i++) {
				if (currentnode->free) {
					currentnode->free = false;
					return currentnode->ptr;
				}
				else {
					currentnode = currentnode->befor;
				}
			}
		}

		void deallocate(void* ptr) {
			auto byte_ptr = (unsigned char*)ptr;

			if (byte_ptr < m_buffer || byte_ptr >= m_buffer + m_size)
				return; // از این pool نیست

			Node* currentnode = &endnode;
			for (int i = 0; i < n_chank; i++) {
				if (!currentnode->free && currentnode->ptr == byte_ptr) {
					currentnode->free = true;
				}
				else {
					currentnode = currentnode->befor;
				}
			}
			throw std::bad_alloc();
		}

		void reset() {
			Node* currentnode = &endnode;
			for (int i = 0; i < n_chank; ++i) {
				currentnode->free = true;
				currentnode = currentnode->befor;
			}

		}

		void insertAtBeginning(Node* head, unsigned char* ptr) {
			Node* newnode = new Node;
			newnode->befor = head;
			newnode->ptr = ptr;
		}

	private:
		Node endnode;
		size_t m_size;
		size_t Chank_size;
		size_t n_chank;
		unsigned char* m_buffer;
	};
 }