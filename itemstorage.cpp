#include "itemstorage.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void ItemStorage::saveItemsToFile(const QString& filename, QVector<Item>& items, bool mainPage)
{
    if(!mainPage){
        return;
    }
    QJsonArray array{};
    for (auto &r : items) {
        QJsonObject obj{};
        obj["name"] = r.name;
        obj["quantity"] = r.quantity;
        obj["unit"] = r.unit;
        obj["checkedOut"] = QJsonValue(r.checkedOut);
        array.append(obj);
    }

    QJsonDocument doc {array};
    QFile file {filename};
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void ItemStorage::loadItemsFromFile(const QString& filename, QVector<Item>& items)
{
    QFile file {filename};
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc { QJsonDocument::fromJson(file.readAll()) };
    QJsonArray array { doc.array() };
    for (const auto& e: array) {
        QJsonObject obj { e.toObject()};
        items.push_back({ obj["name"].toString(), obj["quantity"].toDouble(), obj["unit"].toString(), obj["checkedOut"].toBool()});
    }

}
