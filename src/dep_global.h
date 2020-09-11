#ifndef SRC_GLOBAL_H
#define SRC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SRC_LIBRARY)
#  define SRC_EXPORT Q_DECL_EXPORT
#else
#  define SRC_EXPORT Q_DECL_IMPORT
#endif

#endif // SRC_GLOBAL_H
