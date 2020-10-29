#ifndef CONSOLELOGSDISPLAY_H
#define CONSOLELOGSDISPLAY_H

#pragma warning(push)
#pragma warning(disable : 4251 5054)
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

inline ConsoleLogsDisplay::ConsoleLogsDisplay(QWidget* const pParent) : QTextBrowser(pParent) {}

#endif // CONSOLELOGSDISPLAY_H
