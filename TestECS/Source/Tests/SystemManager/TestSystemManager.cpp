#include <ECS.hpp>
#include "SystemsDef.hpp"



int TestSystemManager(void) {
	MultiStation::SystemManager sysMgr;
	uint32_t phase1 = sysMgr.CreatePhase();
	uint32_t phase2 = sysMgr.CreatePhase();
	uint32_t phase3 = sysMgr.CreatePhase();
	sysMgr.BindPhase(phase1);
	sysMgr.AddSystem(std::make_shared<TestSystemA>());
	sysMgr.AddSystem(std::make_shared<TestSystemB>());
	sysMgr.BindPhase(phase2);
	sysMgr.AddSystem(std::make_shared<TestSystemC>());
	sysMgr.BindPhase(phase3);
	sysMgr.AddSystem(std::make_shared<TestSystemD>());
	
	for (int i = 0; i < 10; i++) {
		sysMgr.ExecutePhase(phase1);
		sysMgr.ExecutePhase(phase2);
		sysMgr.ExecutePhase(phase3);
		// sleep thread for one second to see the output clearly
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
