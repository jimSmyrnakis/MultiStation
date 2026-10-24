#pragma once
#include <ECS.hpp>
namespace MultiStation{

	class EditorSystem : public ISystem {
	public:

		virtual ~EditorSystem(void) noexcept = default;
		 

		inline void OnAttach(void) noexcept override {
			MS_ENGINE_INFO("Editor System OnAttach called !");
		};


		
		inline void OnDettach(void) noexcept override {
			MS_ENGINE_INFO("Editor System OnDettach called !");
		};


		
	};

}
