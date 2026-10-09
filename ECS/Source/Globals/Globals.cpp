#include "Globals.hpp"

namespace MultiStation {
	std::atomic<CompID> s_typeID = 0;
	std::atomic<ObjID> obj_id = 0;

	EntID rootEntity = 0;
	EntID nullEntity = 0xFFFFFFFF;
}
