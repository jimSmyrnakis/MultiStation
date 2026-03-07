#include "mspch.h"
#include "SandBox3D.hpp"
#include "ExampleSystem.hpp"
#include <Editor.hpp>
namespace MultiStation {

	SandBox3D* s_singleton = nullptr;

	Application* CreateApplication(void) noexcept {

		if (s_singleton == nullptr) {

			s_singleton = new (std::nothrow)SandBox3D(std::thread::hardware_concurrency());
			MS_ASSERT(s_singleton, "No memory !!!");
		}

		return s_singleton;
	}

	Application& GetApplication(void) noexcept {
		MS_ASSERT(s_singleton, "No Singleton !");
		return *s_singleton;
	}

	void DestroyApplication(void) noexcept {
		MS_ASSERT(s_singleton, "No singleton to destroy!");
		if (s_singleton) {
			delete s_singleton;
			s_singleton = nullptr;
		}
	}











	SandBox3D::SandBox3D(uint32_t threads) noexcept : Application("SandBox3D" , threads) {

	}
	SandBox3D::~SandBox3D(void) noexcept {

	}

	uint32_t phase1 = 0;
	uint32_t phase2 = 1;

	void SandBox3D::SetUp(void) noexcept {
		ExampleSystem* sys = new ExampleSystem();
		Renderer3DDS* renderer = new Renderer3DDS();
		Editor* editor = new Editor();


		this->CreatePhase(phase1);
		
		this->AddSystemToPhase(sys , phase1);
		this->AddSystemToPhase(editor, phase1);

		this->PushSystemLayer(sys);
		this->PushSystemLayer(renderer);
		this->PushSystemLayer(editor);
		

	}












	

	
}
