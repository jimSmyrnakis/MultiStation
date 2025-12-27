#pragma once
#include <stdint.h>
#include <stddef.h>
namespace MultiStation {

	class BitMap {

	public:
		BitMap(void) noexcept ;
		BitMap(const BitMap& other) noexcept ;
		BitMap(BitMap&& other) noexcept;
		
		~BitMap(void) noexcept ;

		BitMap& operator=(const BitMap& other) noexcept;
		BitMap& operator=(BitMap&& other) noexcept ;


	public:

		void SetDims(uint32_t height, uint32_t width) noexcept;
		void SetBit(uint32_t x, uint32_t y, bool value) noexcept;
		void ResetBitMap(void) noexcept;

		bool GetBit(uint32_t x, uint32_t y) const noexcept;
		uint32_t GetHeight(void) const noexcept;
		uint32_t GetWidth(void) const noexcept;
		uint32_t GetCapacity(void) const noexcept;


	private:
		uint8_t*				m_bitmap;
		uint32_t				m_width;
		uint32_t				m_height;
		uint32_t				m_capacity;
	};

}
