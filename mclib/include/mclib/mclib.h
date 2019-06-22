#ifndef MCLIB_H_
#define MCLIB_H_

#if defined(_MSC_VER)
#ifdef MCLIB_EXPORTS
#define MCLIB_API __declspec(dllexport)
#else
#define MCLIB_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#ifdef MCLIB_EXPORTS
#define MCLIB_API __attribute__((visibility("default")))
#else
#define MCLIB_API
#endif
#else
#error Unknown compiler. Dynamic link import/export semantics unknown.
#endif

#endif
