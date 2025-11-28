#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

protected:
    QPointF m_velocidad;    // (vx, vy)
    QPointF m_aceleracion;  // (ax, ay)

public:
    explicit Personaje(QGraphicsItem *parent = nullptr);

    // Setters de física
    void setVelocidadInicial(const QPointF &v0);
    void setAceleracion(const QPointF &a);

    // Actualiza posición y velocidad según ecuación de movimiento
    // dt está en segundos (por ejemplo 0.016 ~= 60 FPS)
    void actualizarMovimiento(qreal dt);

    QPointF velocidad() const { return m_velocidad; }
    QPointF aceleracion() const { return m_aceleracion; }
};

#endif // PERSONAJE_H
