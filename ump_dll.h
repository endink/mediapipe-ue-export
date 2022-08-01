#pragma once

#if defined(_MSC_VER) // M$VS
    #define DLL_EXPORT __declspec(dllexport)
    #define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) // GCC
    #define DLL_EXPORT __attribute__((visibility("default")))
    #define DLL_IMPORT
#else
    #define DLL_EXPORT
    #define DLL_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#if defined(UMP_EXPORTS)
	#define UMP_API DLL_EXPORT
#elif defined(UMP_IMPORTS)
	#define UMP_API DLL_IMPORT
#else
	#define UMP_API
#endif

extern "C"
{
	UMP_API IUmpContext* DYN_UmpCreateContext();
}
