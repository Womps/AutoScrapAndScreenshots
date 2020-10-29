cmake_minimum_required(VERSION 3.18.0)

set(PROJECT_NAME auto-scrap-and-screenshots)
set(TS_FILES auto-scrap-and-screenshots_fr_FR.ts)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(${PROJECT_NAME} LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets Network LinguistTools REQUIRED)

add_executable(${PROJECT_NAME} WIN32
main.cpp
Application.h
Application.cpp
MainWindow.cpp
MainWindow.h
MainWindowWidget.cpp
MainWindowWidget.h
StreamSink.h
ConsoleLogsDisplay.h
ConsoleLogsDisplay.cpp
UniqueUrlStandardModel.cpp
UniqueUrlStandardModel.h
CsvParser.cpp
CsvParser.h
AutoScrapAndScreenShots.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Network)

if(MSVC)
  cmake_policy(SET CMP0091 NEW)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
  target_compile_options(${PROJECT_NAME} PRIVATE /W4) 
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
