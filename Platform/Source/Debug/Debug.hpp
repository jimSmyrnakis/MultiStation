#pragma once

#if defined(_MSC_VER)
	#define breakpoint __debugbreak()
#elif defined(__GNUC__)
	#define breakpoint __builtin_trap()
#else 
	#error "Unsupported compiler"
#endif 

#ifdef _DEBUG 

	#define ASSERT(expr) if (!expr) { breakpoint; }

#elif defined(NDEBUG)
	
	#define ASSERT(expr) if (!expr) { exit(1); }

#endif 


