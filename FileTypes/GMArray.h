#pragma once
#include "FileOperations.h"

template <typename T>
class GMArray
{
	uint32_t m_size = 0;
	std::unique_ptr<T[]> m_elements;

	bool reserve()
	{
		if (m_size == 0)
			return false;

		m_elements = std::make_unique<T[]>(m_size);
		return true;
	}

public:
	GMArray() = default;
	GMArray(GMArray&&) = default;
	GMArray& operator=(GMArray&&) = default;

	GMArray(const GMArray& other) : m_size(other.m_size), m_elements(std::make_unique<T[]>(m_size))
	{
		memcpy(m_elements.get(), other.m_elements.get(), sizeof(T) * m_size);
	}

	GMArray& operator=(const GMArray& other)
	{
		m_size = other.m_size;
		m_elements = std::make_unique<T[]>(m_size);
		memcpy(m_elements.get(), other.m_elements.get(), sizeof(T) * m_size);
		return *this;
	}

	template <bool SizeInBytes = false>
	bool reserve(const char*& input)
	{
		uint32_t size = FileOps::Read<uint32_t>(input);
		if constexpr (SizeInBytes)
			size /= sizeof(T);
		return reserve(size);
	}

	bool reserve(uint32_t size)
	{
		m_size = size;
		return reserve();
	}

	void fill(const char*& input)
	{
		FileOps::Read(m_elements.get(), input, m_size * sizeof(T));
	}

	template <bool SizeInBytes = false>
	void reserve_and_fill(const char*& input)
	{
		if (reserve<SizeInBytes>(input))
			fill(input);
	}

	void reserve_and_fill(const char*& input, uint32_t size)
	{
		if (reserve(size))
			fill(input);
	}

	T& operator[](const size_t index)
	{
		assert(index < m_size);
		return m_elements[index];
	}

	const T& operator[](const size_t index) const
	{
		assert(index < m_size);
		return m_elements[index];
	}

	T* begin() { return m_elements.get(); }
	T* end() { return m_elements.get() + m_size; }
	const T* begin() const { return m_elements.get(); }
	const T* end() const { return m_elements.get() + m_size; }

	T& front() { return *m_elements; }
	T& back() { return m_elements[m_size - 1]; }

	const T& front() const { return *m_elements; }
	const T& back() const { return m_elements[m_size - 1]; }

	uint32_t getSize() { return m_size; }
	bool isEmpty() const { return m_elements != nullptr; }
};

template <typename T>
class GMArray_View
{
	uint32_t m_size = 0;
	const T* m_elements = nullptr;

public:
	template <bool SizeInBytes = false>
	void view(const char*& input)
	{
		uint32_t bytes = 0;
		if constexpr (SizeInBytes)
		{
			FileOps::Read(bytes, input);
			m_size = bytes / sizeof(T);
		}
		else
		{
			FileOps::Read(m_size, input);
			bytes = m_size * sizeof(T);
		}

		if (bytes)
		{
			m_elements = reinterpret_cast<const T*>(input);
			input += bytes;
		}
	}

	T& operator[](const size_t index)
	{
		assert(index < m_size);
		return m_elements[index];
	}

	const T& operator[](const size_t index) const
	{
		assert(index < m_size);
		return m_elements[index];
	}

	T* begin() { return m_elements.get(); }
	T* end() { return m_elements.get() + m_size; }
	const T* begin() const { return m_elements.get(); }
	const T* end() const { return m_elements.get() + m_size; }
	uint32_t getSize() { return m_size; }
};
