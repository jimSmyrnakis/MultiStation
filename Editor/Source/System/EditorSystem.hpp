#pragma once
#include <ECS.hpp>
namespace MultiStation{

	class EditorSystem : public ISystem {
	public:

		virtual ~EditorSystem(void) = default;
		 

		inline void OnAttach(void) override {
			MS_ENGINE_INFO("Editor System OnAttach called !");
		};


		
		inline void OnDettach(void) override {
			MS_ENGINE_INFO("Editor System OnDettach called !");
		};


		
		inline void OnUpdate(float delta_time) override {};

		
		inline void OnEvent(Event& e) override {};
	};

}
