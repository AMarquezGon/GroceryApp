#pragma once
#include <QSortFilterProxyModel>
#include "itemmodel.h"

class SorterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;
    void setSourceModel(QAbstractItemModel *model);
    Q_INVOKABLE bool set(int row, int role, const QVariant& value){
        return qobject_cast<ItemModel*>(sourceModel())->set(originalRow(row),role,value);
    };
    Q_INVOKABLE void removeAt(int row){
        return qobject_cast<ItemModel*>(sourceModel())->removeAt(originalRow(row));
    };
    Q_INVOKABLE void clearAllItems(){
        return qobject_cast<ItemModel*>(sourceModel())->clear();
    };
    Q_INVOKABLE bool addOrMergeItem(const QString& name, double quantity, const QString& unit, ItemModel::MergePolicy policy = ItemModel::Ask){
        return qobject_cast<ItemModel*>(sourceModel())->addOrMergeItem(name, quantity, unit, policy);
    };
    Q_INVOKABLE bool merge(const QString& name,const QString& unit){
        return qobject_cast<ItemModel*>(sourceModel())->merge(name, unit);
    };
    Q_INVOKABLE void addRecipeItems(const ItemModel* const recipeItemsModel,int portions){
        return qobject_cast<ItemModel*>(sourceModel())->addRecipeItems(recipeItemsModel, portions);
    };

private:
    int originalRow(int row){return mapToSource(this->index(row,0)).row();}

signals:
    void mergeRequested(QString name, QString unit);
};

