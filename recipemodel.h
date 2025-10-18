#pragma once
#include "itemmodel.h"

struct Recipe : public QObject {
    QString name{};
    QSharedPointer<ItemModel> model{};

    explicit Recipe(QObject* parent = nullptr) :QObject{ parent }, model{ new ItemModel(this) } {}
};


class RecipeModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int recipeSize  READ numberOfRecipes NOTIFY recipeSizeChanged)
public:
    explicit RecipeModel(QObject *parent = nullptr);
    Q_INVOKABLE int numberOfRecipes() const {return m_recipes.size();};
    Q_INVOKABLE ItemModel* addItemModel(bool loading = false);
    Q_INVOKABLE ItemModel* getItemModel(int row);
    Q_INVOKABLE void setRecipeName(int row, QString name);
    Q_INVOKABLE QString readRecipeName(int row) const;
    Q_INVOKABLE void removeRecipe(int row, bool deleteAll = false);
    Q_INVOKABLE void removeAllRecipes();

signals:
    void recipeSizeChanged();
    void recipeNameChanged(int index, const QString& newName);

private:
    void storeRecipe(int row) const;
    void loadRecipes();

    QVector<QSharedPointer<Recipe>> m_recipes{};
    bool isLoading {false};
};
