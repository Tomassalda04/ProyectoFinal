#include "personaje.h"

Personaje::Personaje(QGraphicsItem *parent)
    : QObject()
    , QGraphicsPixmapItem(parent)
    , m_velocidad(0.0, 0.0)
    , m_aceleracion(0.0, 0.0)
{
}

void Personaje::setVelocidadInicial(const QPointF &v0)
{
    m_velocidad = v0;
}

void Personaje::setAceleracion(const QPointF &a)
{
    m_aceleracion = a;
}

void Personaje::actualizarMovimiento(qreal dt)
{
    QPointF p = pos();

    QPointF desplazamiento(m_velocidad.x() * dt + 0.5 * m_aceleracion.x() * dt * dt,
                           m_velocidad.y() * dt + 0.5 * m_aceleracion.y() * dt * dt);

    p += desplazamiento;

    // Actualizar velocidad: v = v + a*dt
    m_velocidad.setX(m_velocidad.x() + m_aceleracion.x() * dt);
    m_velocidad.setY(m_velocidad.y() + m_aceleracion.y() * dt);

    // Aplicar nueva posición
    setPos(p);
}

