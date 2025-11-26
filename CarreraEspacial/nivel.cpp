#include "nivel.h"
#include <QGraphicsPixmapItem>

static const int VIEW_W = 1024;
static const int VIEW_H = 572;

Nivel::Nivel(int numeroNivel, QObject *parent)
    : QGraphicsScene(parent),
    m_numeroNivel(numeroNivel)
{
    switch (m_numeroNivel) {
    case 1: crearNivel1(); break;
    case 2: crearNivel2(); break;
    case 3: crearNivel3(); break;
    default: crearNivel1(); break;
    }
}

void Nivel::crearNivel1()
{
    QPixmap fondo(":/Sprites/SpritesNivel1/FondoNivel1.png");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    QPixmap fondoEscalado = fondo.scaled(
        VIEW_W, VIEW_H,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    addPixmap(fondoEscalado)->setPos(0, 0);
}

void Nivel::crearNivel2()
{
    QPixmap fondo(":/Sprites/SpritesNivel2/FondoNivel2.png");

    QPixmap fondoEscalado = fondo.scaledToHeight(
        VIEW_H,
        Qt::SmoothTransformation
        );

    setSceneRect(0, 0, fondoEscalado.width(), fondoEscalado.height());

    addPixmap(fondoEscalado)->setPos(0, 0);
}

void Nivel::crearNivel3()
{
    QPixmap fondo(":/Sprites/SpritesNivel3/FondoNivel3.png");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    QPixmap fondoEscalado = fondo.scaled(
        VIEW_W, VIEW_H,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    addPixmap(fondoEscalado)->setPos(0, 0);
}


