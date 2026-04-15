QT += core gui widgets
CONFIG += c++17

macx: QMAKE_CXXFLAGS += -include arm_acle.h

SOURCES += \
    door.cpp \
    hud.cpp \
    main.cpp \
    game.cpp \
    gameview.cpp \
    level.cpp \
    gridmap.cpp \
    character.cpp \
    player.cpp \
    enemy.cpp \
    trap.cpp \
    treasure.cpp \
    turnmanager.cpp \
    position.cpp \
    tile.cpp \
    widget.cpp

HEADERS += \
    door.h \
    game.h \
    gameview.h \
    hud.h \
    level.h \
    gridmap.h \
    character.h \
    player.h \
    enemy.h \
    trap.h \
    treasure.h \
    turnmanager.h \
    position.h \
    tile.h \
    widget.h

FORMS += widget.ui

RESOURCES += \
    resources.qrc