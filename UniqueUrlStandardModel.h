#ifndef UNIQUEURLSTANDARDMODEL_H
#define UNIQUEURLSTANDARDMODEL_H

#pragma warning(push)
#pragma warning(disable : 4251 5054)
#include <QtGui/QStandardItemModel>
#pragma warning(pop)
#include <QtCore/QPersistentModelIndex>
#include <QtCore/QHash>
#include <QtCore/QStringList>

class UniqueUrlStandardModel final : public QStandardItemModel
{
    Q_OBJECT
public:
    UniqueUrlStandardModel(QObject* const pParent = nullptr);
    ~UniqueUrlStandardModel() = default;

    bool contains(const QString& sUrl) const;

private slots:
    void onRowsInserted(const QModelIndex& parentIndex, const int nFirst, const int nLast);
    void onRowsAboutToBeRemoved(const QModelIndex& parentIndex, const int nFirst, const int nLast);
    void onRowsRemoved(const QModelIndex& parentIndex, const int nFirst, const int nLast);

private:
    QStringList _lUrlToRemove;
    QHash<QString, QPersistentModelIndex> _hIndexByUrl;
};

inline bool UniqueUrlStandardModel::contains(const QString& sUrl) const {return _hIndexByUrl.contains(sUrl);}

#endif // UNIQUEURLSTANDARDMODEL_H
