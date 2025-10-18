#include "recipemodel.h"
#include "itemmodel.h"
#include "constants.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

RecipeModel::RecipeModel(QObject *parent)
    : QObject(parent)
    , isLoading{false} // Direct list initialization
{
        loadRecipes();
}

ItemModel* RecipeModel::getItemModel(int row)
{
    if (row < 0 || row >= m_recipes.size()) {
        return nullptr;
    }
    return m_recipes[row]->model.get();
}

ItemModel* RecipeModel::addItemModel(bool loading)
{
    QSharedPointer<Recipe> newRecipe { QSharedPointer<Recipe>::create(this) };
    QSharedPointer<ItemModel> newRecipeModel { newRecipe->model };
    m_recipes.append(newRecipe);

    if(!loading){
        storeRecipe(m_recipes.size()-1);
    }
    emit recipeSizeChanged();
    connect(newRecipeModel.get(), &QAbstractItemModel::dataChanged,
            this, [this,newRecipe](const QModelIndex&,const QModelIndex&,const QList<int>&){
        int row { static_cast<int>(m_recipes.indexOf(newRecipe)) };
        if(row != -1){
            storeRecipe(row);
        }
    } );
    connect(newRecipeModel.get(), &QAbstractItemModel::rowsInserted,
            this, [this, newRecipe](const QModelIndex&,const int,const int){
                if(isLoading) return;
                int row { static_cast<int>(m_recipes.indexOf(newRecipe)) };
                if(row != -1){
                    storeRecipe(row);
                }
            } );
    connect(newRecipeModel.get(), &QAbstractItemModel::rowsRemoved,
            this, [this, newRecipe](const QModelIndex&,const int,const int){
                int row { static_cast<int>(m_recipes.indexOf(newRecipe)) };
                if(row != -1){
                    storeRecipe(row);
                }
            } );
    return newRecipeModel.get();
}

void RecipeModel::setRecipeName(int row, QString newName)
{
    m_recipes[row]->name = newName;
    storeRecipe(row);
    emit recipeNameChanged(row, newName);
}

QString RecipeModel::readRecipeName(int row) const
{
    return m_recipes[row]->name;
}

void RecipeModel::removeRecipe(int row, bool deleteAll)
{
    if (row >= 0 && row < m_recipes.size()) {
        m_recipes.removeAt(row);
    }
    if(!deleteAll){
        QFile file { constants::fileRecipes };
        if (!file.open(QIODevice::ReadOnly)) return;
        QJsonDocument doc { QJsonDocument::fromJson(file.readAll()) };
        QJsonArray recipeArray { doc.array() };
        recipeArray.removeAt(row);
        doc = QJsonDocument{ recipeArray };
        file.close();
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
        }
    }
    emit recipeSizeChanged();
}

void RecipeModel::removeAllRecipes()
{
    while(!m_recipes.empty()){
        removeRecipe(0,true);
    }
    QFile file { constants::fileRecipes };
    QJsonArray recipeArray {};
    QJsonDocument doc { recipeArray };
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void RecipeModel::storeRecipe(int row) const
{
    QFile file { constants::fileRecipes };
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc { QJsonDocument::fromJson(file.readAll())};
    QJsonArray recipeArray { doc.array()};
    QJsonObject newRecipe {};
    newRecipe["name"] = m_recipes[row]->name;
    QJsonArray items {};
    for (const auto &r : m_recipes[row]->model->storedItems()) {
        QJsonObject obj {};
        obj["name"] = r.name;
        obj["quantity"] = r.quantity;
        obj["unit"] = r.unit;
        obj["checkedOut"] = QJsonValue { r.checkedOut };
        items.append(obj);
    }
    newRecipe["items"] = items;

    if(row>=recipeArray.size()){
        recipeArray.append(newRecipe);
    }
    else{
        recipeArray.replace(row,newRecipe);
    }
    file.close();

    doc = QJsonDocument { recipeArray };
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void RecipeModel::loadRecipes()
{
    QFile file { constants::fileRecipes } ;
    if (!file.open(QIODevice::ReadOnly)) return;
    isLoading = true;
    QJsonDocument doc { QJsonDocument::fromJson(file.readAll()) };
    const QJsonArray recipeArray { doc.array() };
    for (int i {0}; i < recipeArray.size(); ++i) {
        QJsonObject obj { recipeArray[i].toObject() };
        ItemModel* newRecipe { addItemModel(true) };
        m_recipes[i]->name = obj["name"].toString();
        const QJsonArray itemArray = obj["items"].toArray();
        for(const auto& item: itemArray) {
            QJsonObject itemObj { item.toObject() };
            newRecipe->addOrMergeItem(itemObj["name"].toString(),itemObj["quantity"].toDouble(),itemObj["unit"].toString(),ItemModel::MergePolicy::Always);
        }
    }
    isLoading = false;
}
