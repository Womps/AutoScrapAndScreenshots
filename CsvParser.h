#ifndef CSVPARSER_H
#define CSVPARSER_H

class QString;
class QStringList;

namespace CSV
{
     void fillListWithDataFromColumn(QStringList& lToFill, const QString& sCsvContent, const int iColumn);
}

#endif // CSVPARSER_H
