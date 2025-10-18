#include "itemmodel.h"
#include "itemstorage.h"
#include "constants.h"

ItemModel::ItemModel(QObject* parent, bool mainPage)
    : QAbstractListModel(parent)
    , m_mainPageList{mainPage}
{
    if(mainPage)
    {
        ItemStorage::loadItemsFromFile(constants::fileCartItem,m_items);
        rebuildIndex();
    }
}

int ItemModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

QVariant ItemModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};
    const int row {index.row()};
    if (row < 0 || row >= m_items.size())
        return {};

    const Item& it {m_items[row]};
    switch (role) {
    case NameRole:     return it.name;
    case QuantityRole: return it.quantity;
    case UnitRole:     return it.unit;
    case CheckedRole:  return it.checkedOut;
    default:           return {};
    }
}

Qt::ItemFlags ItemModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ItemModel::roleNames() const
{
    return {
        { NameRole,     "name"     },
        { QuantityRole, "quantity" },
        { UnitRole,     "unit"     },
        { CheckedRole,  "checkedStatus"  }
    };
}

QString ItemModel::makeKey(const QString& name, const QString& unit)
{
    return name.trimmed().toLower() + constants::keySeparator + unit.trimmed().toLower();
}

// Data Modification
bool ItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;
    const int row { index.row() };
    if (row < 0 || row >= m_items.size())
        return false;
    Item& it { m_items[row] };
    switch(role)
    {
    case NameRole: return updateNameRole(it,index,value,row);
    case QuantityRole: return updateQuantityRole(it,index,value,row);
    case UnitRole: return updateUnitRole(it,index,value,row);
    case CheckedRole: return updateCheckedRole(it,index,value,row);
    default: return false;
    }
}

bool ItemModel::updateNameRole(Item& it,const QModelIndex& index, const QVariant& value, int row)
{
    const QString newName { value.toString() };
    if (newName == it.name)
        return true;
    const QString oldKey { makeKey(it.name, it.unit) };
    const QString newKey { makeKey(newName, it.unit) };
    it.name = newName;

    updateIndex(oldKey, newKey, row);
    // If renaming collides with an existing item (same name+unit), merge
    if (m_indexByKey.contains(newKey)) {
        const int existingRow { m_indexByKey[newKey].first() };
        if(hasDuplicate(row,newKey)&&isFilledOut(it)){
            emit mergeRequested(m_items[existingRow].name,
                                m_items[existingRow].unit);
            return true;
        }
    }
    dataUpdated(index,NameRole);
    return true;
}

bool ItemModel::updateQuantityRole(Item& it,const QModelIndex& index, const QVariant& value, int row)
{
    const double q { value.toDouble() };
    double oldValue{it.quantity};
    if (q == it.quantity)
        return true;
    it.quantity = q;
    if(it.quantity != 0.0 && oldValue==0.0 && hasDuplicate(row,makeKey(it.name,it.unit)) && isFilledOut(it)){
        emit mergeRequested(m_items[index.row()].name,
                            m_items[index.row()].unit);
    }
    dataUpdated(index,QuantityRole);
    return true;
}

bool ItemModel::updateUnitRole(Item& it,const QModelIndex& index, const QVariant& value, int row)
{
    const QString newUnit { value.toString()};
    if (newUnit == it.unit)
        return true;

    const QString oldKey { makeKey(it.name, it.unit) };
    const QString newKey { makeKey(it.name, newUnit) };

    it.unit = newUnit;
    updateIndex(oldKey, newKey, row);

    // If changing unit collides with an existing item (same name+unit), merge
    if (m_indexByKey.contains(newKey)) {
        const int existingRow { m_indexByKey[newKey].first()};
        if(hasDuplicate(row,newKey)&&isFilledOut(it)){
            emit mergeRequested(m_items[existingRow].name,
                                m_items[existingRow].unit);
        }
        return true;
    }
    dataUpdated(index,UnitRole);
    return true;
}


bool ItemModel::updateCheckedRole(Item& it,const QModelIndex& index, const QVariant& value, int row)
{
    it.checkedOut = value.toBool();
    dataUpdated(index, CheckedRole);
    return true;
}

bool ItemModel::addOrMergeItem(const QString& name, double quantity, const QString& unit, MergePolicy policy)
{
    const int conflictRow  {findConflict(name, unit)};
    if (conflictRow < 0) {
        insertNew(name, quantity, unit);
        return true;
    }

    // Conflict present
    switch (policy) {
    case Always:
        increaseQuantityAt(conflictRow, quantity);
        return true;

    case Never:
        // Insert a duplicate row (same name+unit). If you want disambiguation, change name here.
        insertNew(name, quantity, unit);
        return true;

    case Ask:
        // Let UI decide. Emit details and return false (not completed yet).
        emit mergeRequested(m_items[conflictRow].name, m_items[conflictRow].unit);
        return false;
    }
    return false;
}

void ItemModel::removeAt(int row)
{
    if (row < 0 || row >= m_items.size())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    rebuildIndex();
    ItemStorage::saveItemsToFile(constants::fileCartItem,m_items,m_mainPageList);
}

void ItemModel::clear()
{
    beginResetModel();
    m_items.clear();
    m_indexByKey.clear();
    endResetModel();
    ItemStorage::saveItemsToFile(constants::fileCartItem,m_items,m_mainPageList);
}

bool ItemModel::set(int row, int role, const QVariant& value)
{
    if (row < 0 || row >= m_items.size()) return false;
    QModelIndex idx { index(row, 0) };
    return setData(idx, value, role);
}

bool ItemModel::merge(const QString& name,const QString& unit)
{
    const QString key{makeKey(name, unit)};
    if (!m_indexByKey.contains(key))
        return false;

    auto rowList{m_indexByKey[key]};


    rowList.removeIf([&](int row) {
        const auto& item { m_items[row]};
        return item.checkedOut || !isFilledOut(item);
    });

    if (rowList.size() < 2)
        return true;

    int targetRow{-1};
    double totalQuantity{0.0};

    for (int row : std::as_const(rowList)) {
        if (!m_items[row].checkedOut && isFilledOut(m_items[row])) {
            totalQuantity += m_items[row].quantity;
            if (targetRow == -1) {
                targetRow = row;
            }
        }
    }

    if (targetRow == -1) {
        return true;
    }

    m_items[targetRow].quantity = totalQuantity;

    std::sort(rowList.begin(), rowList.end(), std::greater<int>());
    for (int row : std::as_const(rowList)) {
        if (row != targetRow)
        {
            removeAt(row);
        }
    }

    dataUpdated(index(targetRow, 0),QuantityRole);
    return true;
}


void ItemModel::addRecipeItems(const ItemModel* const recipeItemsModel,int portions)
{
    for(const Item &item: recipeItemsModel->m_items){
        addOrMergeItem(item.name, item.quantity*portions, item.unit, MergePolicy::Always);
    }
}

void ItemModel::rebuildIndex()
{
    QString key{};
    m_indexByKey.clear();
    for (int row {0}; row < m_items.size(); ++row) {
        key = makeKey(m_items[row].name, m_items[row].unit);
        m_indexByKey[key].append(row);
    }
}

void ItemModel::updateIndex(const QString& oldKey, const QString& newKey, int row)
{
    if(m_indexByKey[oldKey].length() == 1)
        m_indexByKey.remove(oldKey);
    else
        m_indexByKey[oldKey].removeAll(row);
    m_indexByKey[newKey].append(row);
}


//Helpers
bool ItemModel::isFilledOut(const Item& item)
{
    return item.name!="" && item.quantity!=0.0;
}

int ItemModel::findConflict(const QString& name, const QString& unit) const
{
    const QString key { makeKey(name, unit)};
    const QList<int> list { m_indexByKey.value(key)};
    return m_indexByKey.contains(key) ? list[0] : -1;
}

bool ItemModel::hasDuplicate(int currentRow, const QString& key)
{
    if (!m_indexByKey.contains(key))
        return false;

    const QList<int>& rows { m_indexByKey[key] };
    for (int row: rows){
        if(row!=currentRow && isFilledOut(m_items[row]) && !m_items[row].checkedOut){
            return true;
        }
    }
    return false;
}

void ItemModel::increaseQuantityAt(int row, double delta)
{
    if (row < 0 || row >= m_items.size() || delta == 0.0)
        return;
    m_items[row].quantity += delta;
    dataUpdated(index(row, 0),QuantityRole);
}

void ItemModel::insertNew(const QString& name, double quantity, const QString& unit)
{
    const int row { static_cast<int>(m_items.size()) };
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back({ name, quantity, unit });
    endInsertRows();
    QString key { makeKey(name, unit) };
    m_indexByKey[key].append(row);
    ItemStorage::saveItemsToFile(constants::fileCartItem,m_items,m_mainPageList);
}

void ItemModel::dataUpdated(const QModelIndex& index, int role)
{
    ItemStorage::saveItemsToFile(constants::fileCartItem,m_items,m_mainPageList);
    emit dataChanged(index, index, { role });
}
