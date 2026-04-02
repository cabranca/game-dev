// FORCE OPTIMIZATION:
// We force O3 optimization for this file because stb_image uses SSE2 intrinsics 
// (like _mm_slli_si128) that require immediate arguments (compile-time constants). 
// In Debug builds (-O0), GCC fails to propagate constants effectively, causing 
// "argument must be an 8-bit immediate" compilation errors.
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC optimize("O3")
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
