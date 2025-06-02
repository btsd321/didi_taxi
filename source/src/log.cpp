#include <iostream>
#include <QDir>
#include <QApplication>
#include <QDateTime>
#include "log.h"
#include "global.h"

namespace Didi
{
    void CLogFunction::std_output(const QString &message, QsLogging::Level level)
    {
        std::cout << "From log function: " << qPrintable(message) << " " << static_cast<int>(level) << std::endl;
    }

    void CLogFunction::Init()
    {
        // 1. init the logging mechanism
        QString strDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
        QsLogging::Logger& logger = QsLogging::Logger::instance();
        logger.setLoggingLevel(QsLogging::TraceLevel);
        // 检查路径是否存在，不存在则创建
        QDir sLogPath(QApplication::instance()->applicationDirPath() + "/" + QString(LOG_FLODER_NAME));
        if (!sLogPath.exists())
        {
            QLOG_INFO() << "Log floder not exist, create it.";
            sLogPath.mkpath(".");
        }
        // 检查文件是否存在，不存在则创建
        for(unsigned int i = 0; i< 0xffffffff ; i++)
        {
            QString strLogFileName = QString("log_%1_%2.txt").arg(strDate).arg(i);
            QFile log_file(QDir(QApplication::instance()->applicationDirPath()).path() + "/" + QString(LOG_FLODER_NAME) + "/" + strLogFileName);
            if (!log_file.exists())
            {
                QLOG_INFO() << "create Log file :" + log_file.fileName();
                log_file.open(QIODevice::WriteOnly);
                log_file.close();
                // 2. add two destinations
                QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(
                    log_file.fileName(), QsLogging::EnableLogRotation, QsLogging::MaxSizeBytes(512), QsLogging::MaxOldLogCount(2)));
                QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
                QsLogging::DestinationPtr functorDestination(QsLogging::DestinationFactory::MakeFunctorDestination(&Didi::CLogFunction::std_output));
                logger.addDestination(debugDestination);
                logger.addDestination(fileDestination);
                logger.addDestination(functorDestination);
                break;
            }
        }
    }
}
