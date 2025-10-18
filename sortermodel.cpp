#include "sortermodel.h"

void SorterModel::setSourceModel(QAbstractItemModel *model)
{
    // Call base implementation first
    QSortFilterProxyModel::setSourceModel(model);

    // Disconnect any previous connection, then connect
    if (auto m = qobject_cast<ItemModel *>(model)) {
        connect(m, &ItemModel::mergeRequested,
                this, &SorterModel::mergeRequested);
    }
}
