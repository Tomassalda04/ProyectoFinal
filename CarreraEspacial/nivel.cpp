// nivel.cpp
#include "nivel.h"
#include <QGraphicsPixmapItem>

Nivel::Nivel(int numeroNivel, QObject *parent)
    : QGraphicsScene(parent),
    m_numeroNivel(numeroNivel)
{
    setSceneRect(0, 0, 1024, 572);

    switch (m_numeroNivel) {
    case 1:
        crearNivel1();
        break;
    case 2:
        crearNivel2();
        break;
    case 3:
        crearNivel3();
        break;
    default:
        crearNivel1();
        break;
    }
}

void Nivel::crearNivel1()
{
    QPixmap fondo(":/Sprites/SpritesNivel1/FondoNivel1.png");
    addPixmap(fondo)->setPos(0, 0);

}

void Nivel::crearNivel2()
{
    QPixmap fondo(":/Sprites/SpritesNivel2/FondoNivel2.png");
    addPixmap(fondo)->setPos(0, 0);
}

void Nivel::crearNivel3()
{
    QPixmap fondo(":/Sprites/SpritesNivel3/FondoNivel3.png");
    addPixmap(fondo)->setPos(0, 0);

}
