#include "naveespacial.h"
#include "obstaculo.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QDebug>

NaveEspacial::NaveEspacial(QGraphicsItem *parent)
    : Personaje(parent)
    , m_timerMovimiento(new QTimer(this))
    , m_velocidadBase(150.0)       // lento para alargar el nivel
    , m_velocidadExtraDer(80.0)
    , m_velocidadVertical(230.0)
    , m_arribaPresionado(false)
    , m_abajoPresionado(false)
    , m_derechaPresionada(false)
{
    m_timerMovimiento->setInterval(16); // ~60 FPS
    connect(m_timerMovimiento, &QTimer::timeout, this, &NaveEspacial::actualizarPaso);
    m_timerMovimiento->start();
}

void NaveEspacial::setVelocidadBase(qreal v)
{
    m_velocidadBase = v;
}

void NaveEspacial::setVelocidadVertical(qreal v)
{
    m_velocidadVertical = v;
}

void NaveEspacial::detener()
{
    if (m_timerMovimiento)
        m_timerMovimiento->stop();
}

void NaveEspacial::iniciarMovimiento()
{
    if (m_timerMovimiento && !m_timerMovimiento->isActive())
        m_timerMovimiento->start();
}

void NaveEspacial::setArribaPresionado(bool v)
{
    m_arribaPresionado = v;
    if (v) m_abajoPresionado = false;
}

void NaveEspacial::setAbajoPresionado(bool v)
{
    m_abajoPresionado = v;
    if (v) m_arribaPresionado = false;
}

void NaveEspacial::setDerechaPresionado(bool v)
{
    m_derechaPresionada = v;
}

void NaveEspacial::actualizarPaso()
{
    if (!scene())
        return;

    const qreal dt = m_timerMovimiento->interval() / 1000.0;

    // Velocidades según teclas
    qreal vx = m_velocidadBase;
    if (m_derechaPresionada)
        vx += m_velocidadExtraDer;

    qreal vy = 0.0;
    if (m_arribaPresionado)
        vy -= m_velocidadVertical;
    else if (m_abajoPresionado)
        vy += m_velocidadVertical;

    setVelocidadInicial(QPointF(vx, vy));
    setAceleracion(QPointF(0.0, 0.0));

    // Movimiento usando la física de Personaje
    actualizarMovimiento(dt);

    corregirLimites();
    actualizarCamara();
    comprobarColisiones();
}

void NaveEspacial::corregirLimites()
{
    if (!scene())
        return;

    QRectF limites   = scene()->sceneRect();
    QRectF rectLocal = boundingRect();
    QPointF p        = pos();

    if (p.x() < limites.left())
        p.setX(limites.left());
    if (p.y() < limites.top())
        p.setY(limites.top());
    if (p.x() + rectLocal.width() > limites.right())
        p.setX(limites.right() - rectLocal.width());
    if (p.y() + rectLocal.height() > limites.bottom())
        p.setY(limites.bottom() - rectLocal.height());

    setPos(p);
}

void NaveEspacial::actualizarCamara()
{
    if (!scene())
        return;

    const auto vs = scene()->views();
    if (vs.isEmpty())
        return;

    QGraphicsView *view = vs.first();

    QPointF centro = pos();
    // Para que no aparezcan tan rápido los obstáculos
    centro.setX(centro.x() + 250.0);

    view->centerOn(centro);
}

void NaveEspacial::comprobarColisiones()
{
    const QList<QGraphicsItem*> lista = collidingItems();
    for (QGraphicsItem *item : lista) {
        if (dynamic_cast<Obstaculo*>(item) != nullptr) {
            emit colisionConObstaculo();
            break;
        }
    }
}

