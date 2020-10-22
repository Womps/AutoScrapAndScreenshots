#ifndef CONSOLELOGSDISPLAY_H
#define CONSOLELOGSDISPLAY_H

#pragma warning(push, 0)
#include <QtWidgets/QTextBrowser>
#pragma warning(pop)

enum class LOG_LEVELS
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

class ConsoleLogsDisplay final : public QTextBrowser
{
    Q_OBJECT
public:
    explicit ConsoleLogsDisplay(QWidget* const pParent = nullptr);
    ~ConsoleLogsDisplay() = default;

public slots:
    void insertMessage(const QString& sMessage, const LOG_LEVELS eLogLevel);
};

#endif // CONSOLELOGSDISPLAY_H
