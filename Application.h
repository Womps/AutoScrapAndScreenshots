#ifndef APPLICATION_H
#define APPLICATION_H

#include "StreamSink.h"
#include "ConsoleLogsDisplay.h"

#pragma warning(push)
#pragma warning(disable : 4251)
#include <QtWidgets/QApplication>
#pragma warning(pop)

class QString;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QApplication::instance()))

class Application final : public QApplication
{
    Q_OBJECT
public:
    Application(int& argc, char **argv);
    ~Application() = default;

signals:
    void logMessage(const QString& sMessage, const LOG_LEVELS nLogLevel);

private:
    StreamSink _streamCout;
    StreamSink _streamCerr;
};

#endif // APPLICATION_H
