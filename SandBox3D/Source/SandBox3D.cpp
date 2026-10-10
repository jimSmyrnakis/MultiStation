#include "mspch.h"
#include "SandBox3D.hpp"
#include "ExampleModules.hpp"
#include <Editor.hpp>
namespace MultiStation {

	











	SandBox3D::SandBox3D(void) noexcept : Application("SandBox3D" ) {

	}
	SandBox3D::~SandBox3D(void) noexcept {

	}

	

	void SandBox3D::SetUp(Engine& engine) noexcept {
		
		printf("Hello World!!!");

		engine.AddModule<ExampleModule>();
		engine.AddModule<EditorModule>();

	}












	

	
}
