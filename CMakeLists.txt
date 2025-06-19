cmake_minimum_required(VERSION 3.14)
project(MoaBrowser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Add this line for resource handling
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineCore WebEngineWidgets)

# Create Qt resource file
qt_add_resources(RESOURCE_FILES
    resources.qrc
)

add_executable(LightweightBrowser
    main.cpp
    MainWindow.cpp
    MainWindow.h
    ${RESOURCE_FILES}
)

target_link_libraries(LightweightBrowser PRIVATE
    Qt6::Widgets
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
)