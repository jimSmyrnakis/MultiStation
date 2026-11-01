#pragma once
#include <string>
#include <stdint.h>
#include <vector>
namespace MultiStation{

	namespace IO {

		enum class FileType {
			FILE	, 
			DIR		,
			LINK	,
			UNKNOWN
		};

		struct FileInfo {
			
			FileType type;
			
			std::string path;
			
			std::string name;
			
			std::string ext;
			
			uint64_t size;


		};

		struct DirectoryEntry {

			FileInfo	info;

			std::string parent_path;

		};

		struct Directory {

			DirectoryEntry this_entry;

			std::vector<DirectoryEntry> entries;

		};



	}

}
