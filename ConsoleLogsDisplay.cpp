#include "ConsoleLogsDisplay.h"

void ConsoleLogsDisplay::insertMessage(const QString& sMessage, const LOG_LEVELS eLogLevel)
{
    // Write logs to the text edit with the appropriate color
    QTextCursor cursor(textCursor());
    QTextCharFormat format(cursor.charFormat());
    format.setForeground(QBrush(eLogLevel == LOG_LEVELS::LOG_INFO ? palette().color(QPalette::WindowText) : Qt::red));
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(sMessage, format);
    setTextCursor(cursor);
}
