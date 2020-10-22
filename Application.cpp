#include "Application.h"
#pragma warning(push, 0)
#include <iostream>
#pragma warning(pop)

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , _streamCout(std::cout)
    , _streamCerr(std::cerr)
{
    connect(&_streamCerr, &StreamSink::toLogs, [this](const QString& sMessage){emit logMessage(sMessage, LOG_LEVELS::LOG_ERROR);});
    connect(&_streamCout, &StreamSink::toLogs, [this](const QString& sMessage){emit logMessage(sMessage, LOG_LEVELS::LOG_INFO);});
}
