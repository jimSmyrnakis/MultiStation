#pragma once
#include "Archive/IArchive.hpp"
namespace MultiStation{

	class ISerialize {

	public:
		virtual ~ISerialize(void) = default;

		virtual bool Serialize(void* ref , IArchiveWriter* archive) = 0;
		
		virtual bool Deserialize(void* ref , IArchiveReader* archive) = 0;

	};

}
