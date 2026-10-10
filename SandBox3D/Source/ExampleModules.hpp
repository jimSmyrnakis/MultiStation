#pragma once
#include "mspch.h"
#include <Application.hpp>
namespace MultiStation{

    class ExamplePanel : public IDrawable
    {
    public:
        virtual inline void Draw(void) override
        {
            ImGui::Begin("Example Panel");

            static bool enabled = true;
            static float speed = 1.0f;
            static float value = 0.0f;
            static int counter = 0;
            static ImVec4 color = ImVec4(0.2f, 0.7f, 1.0f, 1.0f);

            ImGui::Text("MultiStation Editor Test Panel");
            ImGui::Separator();

            ImGui::Checkbox("Enabled", &enabled);
            ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);

            if (ImGui::Button("Increase Counter"))
                ++counter;

            ImGui::SameLine();

            if (ImGui::Button("Reset"))
                counter = 0;

            ImGui::Text("Counter: %d", counter);

            ImGui::Separator();

            ImGui::ColorEdit4("Color", (float*)&color);

            value += ImGui::GetIO().DeltaTime * speed * 0.1f;

            if (value > 1.0f)
                value = 0.0f;

            ImGui::ProgressBar(value, ImVec2(-1.0f, 0.0f));

            if (ImGui::CollapsingHeader("Runtime Info"))
            {
                ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
                ImGui::Text(
                    "Frame Time: %.3f ms",
                    1000.0f / ImGui::GetIO().Framerate
                );

                ImGui::Text(
                    "Mouse: %.1f, %.1f",
                    ImGui::GetIO().MousePos.x,
                    ImGui::GetIO().MousePos.y
                );
            }

            if (ImGui::CollapsingHeader("Mini Graph"))
            {
                static float samples[100]{};
                static int offset = 0;

                samples[offset] =
                    0.5f +
                    0.5f * std::sin(
                        static_cast<float>(ImGui::GetTime()) * speed
                    );

                offset = (offset + 1) % 100;

                ImGui::PlotLines(
                    "Signal",
                    samples,
                    100,
                    offset,
                    nullptr,
                    0.0f,
                    1.0f,
                    ImVec2(0.0f, 80.0f)
                );
            }

            ImGui::Separator();

            ImGui::TextDisabled(
                "Drag this panel around or dock it somewhere."
            );

            ImGui::End();
        }
    };

	class ExampleSystem : public ISystem {
	public:

		virtual inline void OnAttach(void) noexcept override {
			MS_INFO("Example System On Attach");
            
		}

		virtual inline void OnDettach(void) noexcept override {
			MS_INFO("Example System On Detach");
		}

		virtual inline void OnUpdate(float deltaTime) noexcept override {
			if (Input::IsKeyPressed(MS_KEY_T)) {
				MS_INFO("Tab key is pressed !");
			}
		}

		virtual inline void OnEvent(Event& event) noexcept override {
			MultiStation::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MultiStation::KeyPressedEvent>(
				BIND_EVENT_FN(ExampleSystem::OnKeyPressedEvent));
		}

		inline bool OnKeyPressedEvent(MultiStation::KeyPressedEvent& e) {
			if (e.GetKeyCode() == MS_KEY_W) {
				MS_INFO("W key pressed :) ");
				
				return true;
			}
			return false;
		}
	};

	class ExampleModule : public ISystemModule {
	public:
		ExampleModule(void)  noexcept  {

		}

		virtual inline void Init(EngineContext& ctx) noexcept override {
			MS_INFO("Example Module Initialized");
            
			Editor::AddDrawable(&m_panel);
			
		}

		virtual inline void Fini(EngineContext& ctx) noexcept override {
			MS_INFO("Example Module Finilized");
		}

		virtual inline ISystem& GetSystem(void) noexcept override {
			return m_system;
		}
		
		
	private:
		ExampleSystem m_system;
		ExamplePanel m_panel;

	};
}
