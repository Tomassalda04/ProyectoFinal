QT += core gui widgets multimedia

CONFIG+=c++11
TEMPLATE=app
TARGET=CarreraEspacial

SOURCES+=\
    NaveEspacial.cpp \
    main.cpp\
    mainwindow.cpp\
    nivel2.cpp \
    personaje.cpp\
    jugador.cpp\
    enemigo.cpp\
    obstaculo.cpp\
    nivel.cpp

HEADERS+=\
    NaveEspacial.h \
    mainwindow.h\
    nivel2.h \
    personaje.h\
    jugador.h\
    enemigo.h\
    obstaculo.h\
    nivel.h

FORMS+=\
    mainwindow.ui

RESOURCES+=\
    Sprites.qrc
