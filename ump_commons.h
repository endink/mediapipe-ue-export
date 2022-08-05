#ifndef COMMONS_H
#define COMMONS_H

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

#if defined(MP_EXPORTS)
#define MP_API DLL_EXPORT
#elif defined(MP_IMPORTS)
#define MP_API DLL_IMPORT
#else
#define MP_API
#endif

struct SerializedProto {
    const char* str;
    int length;
};

struct SerializedProtoArray {
    SerializedProto* data;
    int size;
};


#endif // !
