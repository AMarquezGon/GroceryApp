cmake_minimum_required(VERSION 3.16)

project(GroceryApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

set_source_files_properties(qmlfiles/AppTheme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_executable(appGroceryApp
    main.cpp

)

qt_add_qml_module(appGroceryApp
    URI GroceryApp
    VERSION 1.0
    QML_FILES
        Main.qml
        qmlfiles/AppTheme.qml
        qmlfiles/CartItem.qml
        qmlfiles/RecipeView.qml
        qmlfiles/ListOfRecipes.qml
        qmlfiles/DeleteAllItemsDialog.qml
        qmlfiles/PortionSelector.qml
        qmlfiles/MergeItemsDialog.qml
        qmlfiles/BottomBar.qml
        qmlfiles/HeaderToolBar.qml
    RESOURCES
        icons/add.svg
        icons/recipe.svg
        icons/shoppingCart.svg
        icons/delete.svg
        icons/backArrow.svg
        icons/trashcan.svg
        qtquickcontrols2.conf
        icons/minus.svg
        icons/plus.svg
    SOURCES
        itemmodel.h
        itemmodel.cpp
        recipemodel.h
        recipemodel.cpp
        sortermodel.h
        sortermodel.cpp
        itemstorage.h
        itemstorage.cpp
        constants.h
)

set_target_properties(appGroceryApp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGroceryApp PUBLIC
    Qt6::Quick
    Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appGroceryApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
