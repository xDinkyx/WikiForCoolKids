#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WFCKLIB_LIB)
#  define WFCKLIB_EXPORT Q_DECL_EXPORT
# else
#  define WFCKLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define WFCKLIB_EXPORT
#endif
