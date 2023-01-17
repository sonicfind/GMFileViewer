#pragma once
#include "FilePointer.h"
#include <assert.h>

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
	GMArray(uint32_t size) : m_size(size), m_elements(std::make_unique<T[]>(m_size)) {}

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
	bool reserve(FilePointer& file)
	{
		uint32_t size = file.read<uint32_t>();
		if constexpr (SizeInBytes)
			size /= sizeof(T);
		return reserve(size);
	}

	bool reserve(uint32_t size)
	{
		m_size = size;
		return reserve();
	}

	void fill(FilePointer& file)
	{
		file.read(m_elements.get(), m_size * sizeof(T));
	}

	template <bool SizeInBytes = false>
	void reserve_and_fill(FilePointer& file)
	{
		if (reserve<SizeInBytes>(file))
			fill(file);
	}

	void reserve_and_fill(FilePointer& file, uint32_t size)
	{
		if (reserve(size))
			fill(file);
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

	uint32_t getSize() const { return m_size; }
	bool isEmpty() const { return m_elements != nullptr; }
};

template <typename T>
class GMArray_View
{
	uint32_t m_size = 0;
	const T* m_elements = nullptr;

public:
	template <bool SizeInBytes = false>
	void view(FilePointer& file)
	{
		uint32_t bytes = 0;
		if constexpr (SizeInBytes)
		{
			file.read(bytes);
			m_size = bytes / sizeof(T);
		}
		else
		{
			file.read(m_size);
			bytes = m_size * sizeof(T);
		}

		if (bytes)
		{
			m_elements = reinterpret_cast<const T*>(file.get());
			file += bytes;
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
