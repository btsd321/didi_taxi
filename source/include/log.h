#ifndef LOG_H
#define LOG_H

#include <QsLog/QsLog.h>

namespace Didi
{
    class CLogFunction
    {
    public:
        static void std_output(const QString &message, QsLogging::Level level);
        static void Init();
    };
}

#endif // LOG_H
