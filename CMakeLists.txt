cmake_minimum_required(VERSION 3.21)

project(Renegade VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(Renegade
    WIN32

    sursa/main.cpp

    sursa/MainWindow.h
    sursa/MainWindow.cpp

    sursa/FileScanner.h
    sursa/FileScanner.cpp

    sursa/RenegadeTemplate.h
    sursa/RenegadeTemplate.cpp

    sursa/Engine.h
    sursa/Engine.cpp

    sursa/Style.h
    sursa/Style.cpp

    resurse/aplicatie.rc
    resurse/resurse.qrc
)

target_link_libraries(Renegade PRIVATE Qt6::Widgets)

target_include_directories(Renegade PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/sursa
)

set_target_properties(Renegade PROPERTIES
    WIN32_EXECUTABLE TRUE
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Renegade>"
)

if(WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)

    if(QT_QMAKE_EXECUTABLE)
        get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

        add_custom_command(TARGET Renegade POST_BUILD
            COMMAND "${QT_BIN_DIR}/windeployqt.exe"
                    "$<TARGET_FILE:Renegade>"
            COMMENT "Se copiază fișierele Qt necesare lângă executabil..."
        )
    endif()
endif()
