#pragma once
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <mutex>
#include <atomic>
#include <utility>
namespace MultiStation{

	template<typename T>
	class Queue {

	public:
		Queue(void) noexcept = default;
		Queue(const Queue& other)noexcept = delete;
		Queue(Queue&& other) noexcept = delete;
		~Queue(void) noexcept = default;

		Queue& operator=(const Queue& other) noexcept = delete;
		Queue& operator=(Queue&& other) noexcept = delete;

	public:

		void Push(T& element);
		
		template<typename... Args>
		void Emplace(Args&&... args);

		bool Pop(T& out);

		uint32_t Size(void) const;

	private:
		std::vector<T> m_data;
		std::atomic<uint32_t> m_size = 0;
		std::mutex m_mutex;

	};
}
















namespace MultiStation {
	
	template<typename T>
	void Queue<T>::Push(T& element) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_data.push_back(std::move(element));
		m_size.fetch_add(1, std::memory_order_release);
		
	}

	template<typename T>
	template<typename... Args>
	void Queue<T>::Emplace(Args&&... args) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_data.emplace_back(std::forward<Args>(args)...);
		m_size.fetch_add(1, std::memory_order_release);
	}

	template<typename T>
	bool Queue<T>::Pop(T& out) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_data.empty())
			return false;

		out = std::move(m_data.front());
		m_data.erase(m_data.begin());
		m_size.fetch_sub(1, std::memory_order_release);
		return true;
	}

	template<typename T>
	uint32_t Queue<T>::Size(void) const {
		return m_size.load(std::memory_order_acquire);
	}

}
