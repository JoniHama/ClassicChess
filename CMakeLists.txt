cmake_minimum_required(VERSION 3.16)

project(ChessWithFriends VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appChessWithFriends
    main.cpp
    gamecontroller.h
    gamecontroller.cpp
)


qt_add_qml_module(appChessWithFriends
    URI ChessWithFriends
    VERSION 1.0
    QML_FILES
        UI/Main.qml
        UI/GameController.qml
        UI/GameBoard/GameBoard.qml
        UI/MainMenu/MainMenu.qml
        UI/GameBoard/GameOverScreen.qml
    SOURCES
        main.cpp
        gamecontroller.h
        gamecontroller.cpp
        chesspiece.h
        chesspiece.cpp
        chessboard.h
        chessboard.cpp
        chessmove.h
        chessmove.cpp
        position.h
    RESOURCES
        UI/Assets/B_Bishop.png
        UI/Assets/B_King.png
        UI/Assets/B_Knight.png
        UI/Assets/B_Pawn.png
        UI/Assets/B_Queen.png
        UI/Assets/B_Rook.png
        UI/Assets/background.jpg
        UI/Assets/W_Bishop.png
        UI/Assets/W_King.png
        UI/Assets/W_Knight.png
        UI/Assets/W_Pawn.png
        UI/Assets/W_Queen.png
        UI/Assets/W_Rook.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appChessWithFriends PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appChessWithFriends
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appChessWithFriends
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appChessWithFriends
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
