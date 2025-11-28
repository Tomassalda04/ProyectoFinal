// nivel.cpp
#include "nivel.h"
#include "enemigo.h"
#include "obstaculo.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

static const int VIEW_W = 1024;
static const int VIEW_H = 572;

Nivel::Nivel(int numeroNivel, QObject *parent)
    : QGraphicsScene(parent)
    , m_numeroNivel(numeroNivel)
{
    setSceneRect(0, 0, VIEW_W, VIEW_H);

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

// ───────── NIVEL 1: fondo + obstáculos + enemigo ─────────
void Nivel::crearNivel1()
{
    // Usamos el fondo del nivel 1 del branch main
    QPixmap fondo(":/Sprites/SpritesNivel1/fondo1.jpg");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(
            VIEW_W, VIEW_H,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        );
        addPixmap(fondoEscalado)->setPos(0, 0);
    }

    // Obstáculos (de main)
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
    obs4->setPos(550, 350);
    obs5->setPos(450, 2);
    obs6->setPos(150, 35);
    obs7->setPos(50, 450);

    addItem(obs1);
    addItem(obs2);
    addItem(obs3);
    addItem(obs4);
    addItem(obs5);
    addItem(obs6);
    addItem(obs7);

    // Enemigo de prueba
    m_personajePrueba = new Enemigo();
    addItem(m_personajePrueba);

    QPixmap sprite(":/Sprites/SpritesNivel1/Sprites_mov_soldados_enemigos/mov_abajo/mov1.png");
    m_personajePrueba->setPixmap(sprite);
    m_personajePrueba->setPos(100, 400);
}

// ───────── NIVEL 2: fondo largo + cohete ─────────
void Nivel::crearNivel2()
{
    QPixmap fondo(":/Sprites/SpritesNivel2/FondoNivel2.png");

    // Escalamos el fondo a la altura de la vista
    QPixmap fondoEscalado = fondo.isNull()
                                ? QPixmap(VIEW_W, VIEW_H)
                                : fondo.scaledToHeight(VIEW_H, Qt::SmoothTransformation);

    if (fondoEscalado.isNull()) {
        fondoEscalado = QPixmap(VIEW_W, VIEW_H);
        fondoEscalado.fill(Qt::black);
    }

    // La escena mide lo que mida el fondo → habrá scroll horizontal
    setSceneRect(0, 0, fondoEscalado.width(), fondoEscalado.height());
    addPixmap(fondoEscalado)->setPos(0, 0);

    // --------- COHETE ---------
    QPixmap spriteCohete(":/Sprites/SpritesNivel2/nave1.png");
    if (spriteCohete.isNull()) {
        spriteCohete = QPixmap(40, 80);
        spriteCohete.fill(Qt::red);
    }

    // Escalamos la nave para que no se vea gigante
    int anchoDeseado = 80;
    int altoDeseado  = 80;

    spriteCohete = spriteCohete.scaled(
        anchoDeseado,
        altoDeseado,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    m_cohete = addPixmap(spriteCohete);

    // Posición inicial del cohete (un poco desde la izquierda y centrado en altura)
    qreal xInicial = 100.0;
    qreal yInicial = sceneRect().height() / 2.0
                     - m_cohete->boundingRect().height() / 2.0;

    m_cohete->setPos(xInicial, yInicial);
}

// ───────── NIVEL 3: fondo estático ─────────
void Nivel::crearNivel3()
{
    QPixmap fondo(":/Sprites/SpritesNivel3/FondoNivel3.png");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(
            VIEW_W, VIEW_H,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        );
        addPixmap(fondoEscalado)->setPos(0, 0);
    }
}

// ───────── Movimiento del cohete (Nivel 2) ─────────
void Nivel::moverCohete(int dx, int dy)
{
    if (!m_cohete)
        return;

    QPointF pos = m_cohete->pos();

    // sumamos desplazamiento
    pos.setX(pos.x() + dx);
    pos.setY(pos.y() + dy);

    // Límites dentro del fondo
    qreal minX = 0.0;
    qreal maxX = sceneRect().width()  - m_cohete->boundingRect().width();
    qreal minY = 0.0;
    qreal maxY = sceneRect().height() - m_cohete->boundingRect().height();

    if (pos.x() < minX) pos.setX(minX);
    if (pos.x() > maxX) pos.setX(maxX);
    if (pos.y() < minY) pos.setY(minY);
    if (pos.y() > maxY) pos.setY(maxY);

    m_cohete->setPos(pos);
}

