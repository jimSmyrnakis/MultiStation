#include "mspch.h"
#include "SandBox3D.hpp"
#include "ExampleModules.hpp"
#include <Editor.hpp>
#include <TransformModule.hpp>
#include <Renderer3DDS.hpp>
namespace MultiStation {

	











	SandBox3D::SandBox3D(void) noexcept : Application("SandBox3D" ) {

	}
	SandBox3D::~SandBox3D(void) noexcept {

	}

	

	void SandBox3D::SetUp(Engine& engine) noexcept {
		
		engine.AddModule<ExampleModule>();
		engine.AddModule<EditorModule>();
		engine.AddModule<Transform3DModule>();
		engine.AddModule<Renderer3DDSModule>();
		
	}












	

	
}
