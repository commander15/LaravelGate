#ifndef LARAVELGATE_GLOBAL_H
#define LARAVELGATE_GLOBAL_H

#include <LaravelGate/config.h>

#ifdef LARAVELGATE_SHARED
#ifdef LARAVELGATE_BUILD
#define LARAVELGATE_EXPORT Q_DECL_EXPORT
#else
#define LARAVELGATE_EXPORT Q_DECL_IMPORT
#endif
#else
#define LARAVELGATE_EXPORT
#endif

#define DATAKIT_Q(ClassName) ClassName *q = static_cast<ClassName *>(q_ptr)
#define DATAKIT_D(ClassName) \
    ClassName##Private *d = static_cast<ClassName##Private *>(qGetPtrHelper(d_ptr))

#endif // LARAVELGATE_GLOBAL_H
