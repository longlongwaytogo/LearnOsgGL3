#ifndef OSGEDITOR_INCLUDE_EFFECTCOMPOSITOR_EXPORT_H
#define OSGEDITOR_INCLUDE_EFFECTCOMPOSITOR_EXPORT_H

#ifdef EFFECTCOMPOSITOR_EXPORTS
#define EFFECTCOMPOSITOR_API __declspec(dllexport)
#else
#define EFFECTCOMPOSITOR_API __declspec(dllimport)
#endif

#endif // OSGEDITOR_INCLUDE_EFFECTCOMPOSITOR_EXPORT_H
