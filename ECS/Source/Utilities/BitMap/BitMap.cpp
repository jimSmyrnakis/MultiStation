#include "BitMap.hpp"
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
// 
namespace MultiStation {
	
	BitMap::BitMap(void) noexcept {
		m_bitmap = nullptr;
		m_capacity = 0;
		m_width = 0;
		m_height = 0;
	}

	BitMap::BitMap(const BitMap& other) noexcept {
		
		if (other.m_bitmap) {
			m_bitmap = (uint8_t*)malloc(other.m_capacity);
			if (!m_bitmap) {
				
				// TODO : using my debugger
				return;
			}
			m_capacity = other.m_capacity;
			m_height = other.m_height;
			m_width = other.m_width;
			memcpy(m_bitmap, other.m_bitmap, m_capacity);
		}
		else {
			m_bitmap = nullptr;
			m_capacity = 0;
			m_height = 0;
			m_width = 0;
		}
	}
	BitMap::BitMap(BitMap&& other) noexcept {
		m_bitmap = other.m_bitmap;
		m_capacity = other.m_capacity;
		m_height = other.m_height;
		m_width = other.m_width;

		other.m_bitmap = nullptr;
		other.m_capacity = 0;
		other.m_height = 0;
		other.m_width = 0;
	}

	BitMap::~BitMap(void) noexcept {
		if (m_bitmap)
		{
			free(m_bitmap);
		}
	}

	BitMap& BitMap::operator=(const BitMap& other) noexcept {
		if (this == &other) return *this;
		if (m_bitmap)
			free(m_bitmap);
		if (other.m_bitmap) {
			m_bitmap = (uint8_t*)malloc(other.m_capacity);
			if (!m_bitmap) {
				
				// TODO : using my debugger
				return;
			}
			m_capacity = other.m_capacity;
			m_height = other.m_height;
			m_width = other.m_width;
			memcpy(m_bitmap, other.m_bitmap, m_capacity);
		}
		else {
			m_bitmap = nullptr;
			m_capacity = 0;
			m_height = 0;
			m_width = 0;
		}
		return *this;
	}
	BitMap& BitMap::operator=(BitMap&& other) noexcept {
		if (&other == this) {
			return *this;
		}

		m_bitmap = other.m_bitmap;
		m_capacity = other.m_capacity;
		m_height = other.m_height;
		m_width = other.m_width;

		other.m_bitmap = nullptr;
		other.m_capacity = 0;
		other.m_height = 0;
		other.m_width = 0;

		return *this;
	}
}


















namespace MultiStation {
	
	void BitMap::SetDims(uint32_t height, uint32_t width) noexcept {
		if (!height || !width) {
			// TODO : fatal error
			
			return;
		}

		uint32_t bitsNum = height * width;
		uint32_t newCapacity = (bitsNum / 8) + ((bitsNum % 8) ? 1 : 0);
		uint8_t* oldBitmap = m_bitmap;
		uint8_t* newBitmap = m_bitmap;
		if (newCapacity == m_capacity)
			goto out;

		newBitmap = (uint8_t*)malloc(newCapacity);
		if (!newBitmap) {
			
			return;
		}
		
		memset(newBitmap, 0, newCapacity);

		if (oldBitmap) {
			// copy old 
			uint32_t minCapacity = (newCapacity < m_capacity) ? newCapacity : m_capacity;
			memcpy(newBitmap, oldBitmap ,minCapacity);
			free(oldBitmap);
		}

	out:
		m_bitmap = newBitmap;
		m_capacity = newCapacity;
		m_height = height;
		m_width = width;
	}
	
	void BitMap::ResetBitMap(void) noexcept {
		if (!m_bitmap) {
			return;
		}

		memset(m_bitmap, 0, m_capacity);
	}
	
	
	void BitMap::SetBit(uint32_t x, uint32_t y, bool value) noexcept {
		if (x >= m_width || y >= m_height || !m_bitmap) {
			
			return;
		}

		uint32_t bitIndex = y * m_width + x;
		uint32_t byteIndex = bitIndex >> 3;        // / 8
		uint32_t bitOffset = bitIndex & 7;         // % 8

		uint8_t mask = uint8_t(1u << (7 - bitOffset));

		if (value)
			m_bitmap[byteIndex] |= mask;
		else
			m_bitmap[byteIndex] &= ~mask;
	}

	bool BitMap::GetBit(uint32_t x, uint32_t y) const noexcept {
		if (x >= m_width || y >= m_height || !m_bitmap) {
			
			return false;
		}

		uint32_t bitIndex = y * m_width + x;
		uint32_t byteIndex = bitIndex >> 3;        // / 8
		uint32_t bitOffset = bitIndex & 7;         // % 8

		uint8_t AndMask = uint8_t(1u << (7 - bitOffset));

		uint8_t byteValue = m_bitmap[byteIndex] & AndMask;

		return byteValue ? true : false;
	}




	uint32_t BitMap::GetHeight(void) const noexcept { return m_height; }
	uint32_t BitMap::GetWidth(void) const noexcept { return m_width; }
	uint32_t BitMap::GetCapacity(void) const noexcept { return m_capacity; }
}
