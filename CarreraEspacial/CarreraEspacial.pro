QT+=core gui widgets
CONFIG+=c++11
TEMPLATE=app
TARGET=CarreraEspacial

SOURCES+=\
    main.cpp\
    mainwindow.cpp\
    personaje.cpp\
    jugador.cpp\
    enemigo.cpp\
    obstaculo.cpp\
    nivel.cpp

HEADERS+=\
    mainwindow.h\
    personaje.h\
    jugador.h\
    enemigo.h\
    obstaculo.h\
    nivel.h

FORMS+=\
    mainwindow.ui

RESOURCES+=\
    Sprites.qrc
