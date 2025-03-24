#ifndef LARAVELGATE_DEBUG_P_H
#define LARAVELGATE_DEBUG_P_H

#include <QtCore/qloggingcategory.h>

#define laravelgateDebug()    qCDebug(laravelgate).noquote().nospace()
#define laravelgateInfo()     qCInfo(laravelgate).noquote().nospace()
#define laravelgateWarning()  qCWarning(laravelgate).noquote().nospace()
#define laravelgateCritical() qCCritical(laravelgate).noquote().nospace()

Q_DECLARE_LOGGING_CATEGORY(laravelgate)

#endif // LARAVELGATE_DEBUG_P_H
