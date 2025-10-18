# GroceryApp

## About This Project

GroceryApp is an Android application designed to help you manage your recipes and shopping lists. It allows you to store your favorite recipes and easily add all the necessary ingredients to a shopping cart.

This application is built using the Qt framework, with a C++ backend for logic and data management, and QML for the user interface.

## Features

- Recipe Storage: Save and manage your personal recipes.
- Shopping List: View all ingredients from selected recipes in one place.
- Add to Cart: Quickly add recipe ingredients to your grocery list.

## Technologies Used

- C++: Core application logic.
- Qt 6 / QML: User interface and data models.
- CMake: Build system.

## How to Build and Run (for Android)

To build and run this project on an Android device, you will need the Qt framework installed with the necessary Android development components (SDK, NDK, and JDK).

The easiest way to build is using Qt Creator:

1. Open Project: Open the CMakeLists.txt file in Qt Creator.
2. Configure Project: When prompted, select a Qt for Android kit (e.g., Android Clang arm64-v8a).
3. Set Up Device: Connect your Android device (with USB debugging enabled) or start an Android Virtual Device (AVD).
4. Build & Run: Click the "Run" button. Qt Creator will build the project, package it into an .apk, and install/launch it on your selected Android device.
