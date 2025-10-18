#pragma once
#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <QVector>
#include <QList>

struct Item {
    QString name{};
    double quantity {0.0};
    QString unit{};
    bool checkedOut {false};
};

class ItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { NameRole = Qt::UserRole+1, QuantityRole, UnitRole, CheckedRole };
    Q_ENUM(Roles)

    enum MergePolicy { Ask, Always, Never };
    Q_ENUM(MergePolicy)

    explicit ItemModel(QObject* parent = nullptr, bool mainPage = false);

    // QAbstractListModel overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Public API for modifying the model
    Q_INVOKABLE bool addOrMergeItem(const QString& name, double quantity, const QString& unit, MergePolicy policy = Ask);
    Q_INVOKABLE void removeAt(int row);
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool merge(const QString& name, const QString& unit);
    Q_INVOKABLE bool set(int row, int role, const QVariant& value);
    void addRecipeItems(const ItemModel* const recipeItemsModel,int portions);

    //Item model data update functions
    bool updateNameRole(Item& it,const QModelIndex& index, const QVariant& value, int row);
    bool updateQuantityRole(Item& it,const QModelIndex& index, const QVariant& value, int row);
    bool updateUnitRole(Item& it,const QModelIndex& index, const QVariant& value, int row);
    bool updateCheckedRole(Item& it,const QModelIndex& index, const QVariant& value, int row);

    //Getters
    const QVector<Item>& storedItems() const {return m_items;};


signals:
    void mergeRequested(QString name, QString unit);

private:
    //modify data
    void increaseQuantityAt(int row, double delta);
    void insertNew(const QString& name, double quantity, const QString& unit);
    void updateIndex(const QString& oldKey, const QString& newKey, int row);
    void rebuildIndex();

    //helpers
    static bool isFilledOut(const Item& item);
    static QString makeKey(const QString& name, const QString& unit);
    bool hasDuplicate(int currentRow, const QString& key);
    int findConflict(const QString& name, const QString& unit) const;
    void dataUpdated(const QModelIndex& index, int role);

    //Data
    QVector<Item> m_items{};
    QHash<QString,QList<int>> m_indexByKey{};
    bool m_mainPageList {};
};
