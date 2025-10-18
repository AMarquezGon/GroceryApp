#include "itemmodel.h"
#include "recipemodel.h"
#include "sortermodel.h"
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QQuickStyle>
#include <QFile>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_FLUENTWINUI3_THEME", QByteArray("Dark"));
    QGuiApplication::setApplicationName("Grocery App");
    QGuiApplication::setOrganizationName("QtProject");
    QGuiApplication app(argc, argv);

    //Create main itemModel
    constexpr bool isMainPage {true};
    ItemModel* m_itemModel = new ItemModel{ &app, isMainPage };

    // Create wrapper for sorting the cart
    SorterModel* sortedModel = new SorterModel{ &app };
    sortedModel->setSourceModel(m_itemModel);
    sortedModel->setSortRole(ItemModel::CheckedRole);
    sortedModel->setDynamicSortFilter(false);
    sortedModel->sort(0,Qt::AscendingOrder);

    //Create the recipe model
    RecipeModel* recipeModel = new RecipeModel(&app);

    //Register ItemModel to make enums available
    qmlRegisterType<ItemModel>("Grocery", 1, 0, "ItemModelType");

    //Register singleton instances
    qmlRegisterSingletonInstance("Grocery", 1, 0, "ItemModel", sortedModel);
    qmlRegisterSingletonInstance("Grocery", 1, 0, "RecipeModel", recipeModel);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GroceryApp", "Main");

    return app.exec();
}
