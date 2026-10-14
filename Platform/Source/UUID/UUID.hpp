#pragma once
#include <stdint.h>
#include <stddef.h>
#include <xhash>
#include <memory>
#include <utility>

#include <unordered_map>
#include <unordered_set>
namespace MultiStation {

	class UUID
	{
	public:
		UUID(void);
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<MultiStation::UUID>
	{
		std::size_t operator()(const MultiStation::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}