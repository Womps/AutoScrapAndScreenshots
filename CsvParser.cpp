#include "CsvParser.h"
#pragma warning(push, 0)
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QChar>
#pragma warning(pop)

void CSV::fillListWithDataFromColumn(QStringList& lToFill, const QString& sCsvContent, const int iColumn)
{
    enum class State {Normal, Quote} state = State::Normal;
    const int iCsvContentSize = sCsvContent.size();
    int iCurrentColumn = 0;
    QString sValue;

    for (int i = 0; i < iCsvContentSize; i++)
    {
        const QChar& current = sCsvContent.at(i);

        // Normal state
        if (state == State::Normal)
        {
            // Comma
            if (current == ',')
            {
                // Save field
                if(iCurrentColumn == iColumn)
                {
                    lToFill.append(sValue.trimmed());
                }

                sValue.clear();
                iCurrentColumn++;
            }
            // Double-quote
            else if (current == '"')
            {
                state = State::Quote;
            }
            // New row
            else if(current == QChar::CarriageReturn)
            {
                if(iCurrentColumn == iColumn)
                {
                    lToFill.append(sValue.trimmed());
                }

                iCurrentColumn = 0;
                sValue.clear();
            }
            // Other character
            else
            {
                sValue += current;
            }
        }
        // In-quote state
        else if (state == State::Quote)
        {
            // Another double-quote
            if (current == '"')
            {
                if (i < iCsvContentSize)
                {
                    // A double double-quote?
                    if (i+1 < iCsvContentSize && sCsvContent.at(i+1) == '"')
                    {
                        // Skip a second quote character in a row
                        i++;
                    }
                    else
                    {
                        state = State::Normal;
                    }
                }
            }
            // Other character
            else
            {
                sValue += current;
            }
        }
    }

    if (!sValue.isEmpty())
    {
        lToFill.append(sValue.trimmed());
    }
}
