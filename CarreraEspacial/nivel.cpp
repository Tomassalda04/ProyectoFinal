// nivel.cpp
#include "nivel.h"
#include "enemigo.h"
#include <QGraphicsPixmapItem>
#include "obstaculo.h"
#include <QTimer>
#include <QDebug>


Nivel::Nivel(int numeroNivel, QObject *parent)
    : QGraphicsScene(parent)
    , m_numeroNivel(numeroNivel)
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

<<<<<<< Updated upstream
void Nivel::crearNivel1()
{
    QPixmap fondo(":/Sprites/SpritesNivel1/FondoNivel1.png");
    addPixmap(fondo)->setPos(0, 0);

=======
void Nivel::crearNivel1(){

    QPixmap fondo(":/Sprites/SpritesNivel1/fondo1.jpg");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    QPixmap fondoEscalado = fondo.scaled(
        VIEW_W, VIEW_H,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    addPixmap(fondoEscalado)->setPos(0, 0);

    Obstaculo *obs1 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs1.png");
    Obstaculo *obs2 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs2.png");
    Obstaculo *obs3 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs3.png");
    Obstaculo *obs4 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs4.png");
    Obstaculo *obs5 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs5.png");
    Obstaculo *obs6 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs6.png");
    Obstaculo *obs7 = new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs7.png");

    obs1->setPos(300, 380);
    obs2->setPos(600, 380);
    obs3->setPos(900, 2);
    obs4->setPos(550,350);
    obs5->setPos(450,2);
    obs6->setPos(150,35);
    obs7->setPos(50,450);

    addItem(obs1);
    addItem(obs2);
    addItem(obs3);
    addItem(obs4);

    m_personajePrueba = new Enemigo();
    addItem(m_personajePrueba);

    QPixmap sprite(":/Sprites/SpritesNivel1/Sprites_mov_soldados_enemigos/mov_abajo/mov1.png");
    m_personajePrueba->setPixmap(sprite);

    m_personajePrueba->setPos(100, 400);


>>>>>>> Stashed changes
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
