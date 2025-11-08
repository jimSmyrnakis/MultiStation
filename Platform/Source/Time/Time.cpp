#include "Time.hpp"

#include <chrono>

namespace MultiStation {

	float Time::GetTimeInSeconds(void) {
		using namespace std::chrono;
		static auto start = high_resolution_clock::now();
		auto now = high_resolution_clock::now();
		duration<float> elapsed = now - start;
		return elapsed.count(); // επιστρέφει float σε δευτερόλεπτα (με κλάσματα)
	}

	uint64_t Time::GetTimeInMilliseconds(void) {
		using namespace std::chrono;
		static auto start = high_resolution_clock::now();
		auto now = high_resolution_clock::now();
		auto elapsed = duration_cast<duration<uint64_t, std::milli>>(now - start);
		return elapsed.count(); // επιστρέφει uint64_t σε milliseconds
	}
}
