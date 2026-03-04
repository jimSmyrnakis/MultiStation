#include "mspch.h"
#include "SandBox3D.hpp"
#include "ExampleSystem.hpp"
#include <Editor.hpp>
namespace MultiStation {

	SandBox3D* s_singleton = nullptr;

	Application* CreateApplication(void) noexcept {

		if (s_singleton == nullptr) {
			s_singleton = new (std::nothrow)SandBox3D();
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











	SandBox3D::SandBox3D(void) noexcept : Application("SandBox3D") {

	}
	SandBox3D::~SandBox3D(void) noexcept {

	}



	void SandBox3D::SetUp(void) noexcept {
		uint32_t phase = this->CreatePhase();
		this->BindPhase(phase);
		ExampleSystem* sys = new ExampleSystem();
		Renderer3DDS* renderer = new Renderer3DDS();
		Editor* editor = new Editor();
		this->AddSystemOnPhase(sys);
		this->AddSystemOnPhase(new Editor());
		this->PushSystemLayer(sys);
		this->PushSystemLayer(renderer);
		this->PushSystemLayer(editor);
		

	}












	

	
}
