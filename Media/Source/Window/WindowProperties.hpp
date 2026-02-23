#pragma once
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <Platform.hpp>
#include <string>
namespace MultiStation {


	struct WindowProperties {
		uint32_t Width;
		uint32_t Height;
		std::string Title;
		bool IsVSync;

		WindowProperties(
			uint32_t Width = 480,
			uint32_t Height = 720,
			std::string Title = "Window" , bool IsVSync = false) noexcept;
		WindowProperties(const WindowProperties& other) noexcept = default;
		WindowProperties(WindowProperties&& other) noexcept = default;

		WindowProperties& operator=(const WindowProperties& other) noexcept = default;
		WindowProperties& operator=(WindowProperties&& other) noexcept = default;
	};

}