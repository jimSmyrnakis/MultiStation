#pragma once
#include <ECS.hpp>
namespace MultiStation{

	class EditorSystem : public ISystem {
	public:

		virtual ~EditorSystem(void) = default;
		 

		inline void OnAttach(void) override {};


		
		inline void OnDettach(void) override {};


		
		inline void OnUpdate(float delta_time) override {};

		
		inline void OnEvent(Event& e) override {};
	};

}
