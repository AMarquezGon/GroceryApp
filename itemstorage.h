#pragma once
#include <QString>
#include "itemmodel.h"

class ItemStorage
{
public:
    static void saveItemsToFile(const QString& filename, QVector<Item>& items, bool mainPage);
    static void loadItemsFromFile(const QString& filename,  QVector<Item>& items);
};
