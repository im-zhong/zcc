#pragma once

#ifdef _WIN32
#define ZCC_EXPORT __declspec(dllexport)
#else
#define ZCC_EXPORT
#endif

ZCC_EXPORT void zcc();
