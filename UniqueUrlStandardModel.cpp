#include "UniqueUrlStandardModel.h"

UniqueUrlStandardModel::UniqueUrlStandardModel(QObject* const pParent)
    : QStandardItemModel(pParent)
{
    connect(this, &UniqueUrlStandardModel::rowsInserted, this, &UniqueUrlStandardModel::onRowsInserted);
    connect(this, &UniqueUrlStandardModel::rowsAboutToBeRemoved, this, &UniqueUrlStandardModel::onRowsAboutToBeRemoved);
    connect(this, &UniqueUrlStandardModel::rowsRemoved, this, &UniqueUrlStandardModel::onRowsRemoved);
}

void UniqueUrlStandardModel::onRowsInserted(const QModelIndex& parentIndex, const int nFirst, const int nLast)
{
    for(int nRow = nFirst; nRow <= nLast; ++nRow)
    {
        const QModelIndex& insertedIndex = index(nRow, 0, parentIndex);
        const QString& sUrl = insertedIndex.data(Qt::DisplayRole).toString();

        if(!sUrl.isEmpty() && !_hIndexByUrl.contains(sUrl))
        {
            _hIndexByUrl.insert(sUrl, insertedIndex);
        }
    }
}

void UniqueUrlStandardModel::onRowsAboutToBeRemoved(const QModelIndex& parentIndex, const int nFirst, const int nLast)
{
    for(int nRow = nFirst; nRow <= nLast; ++nRow)
    {
        const QModelIndex& removingIndex = index(nRow, 0, parentIndex);
        const QString& sUrlToRemove = removingIndex.data(Qt::DisplayRole).toString();

        if(!sUrlToRemove.isEmpty())
        {
            _lUrlToRemove.append(sUrlToRemove);
        }
    }
}

void UniqueUrlStandardModel::onRowsRemoved(const QModelIndex&, const int, const int)
{
    for(const QString& sUrlRemoved : _lUrlToRemove)
    {
        const auto& urlIterator = _hIndexByUrl.constFind(sUrlRemoved);

        if(urlIterator != _hIndexByUrl.constEnd() && !urlIterator.value().isValid())
        {
            _hIndexByUrl.remove(sUrlRemoved);
        }
    }

    _lUrlToRemove.clear();
}
