#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

protected:
    QPointF m_velocidad;
    QPointF m_aceleracion;

public:
    explicit Personaje(QGraphicsItem *parent = nullptr);


    void setVelocidadInicial(const QPointF &v0);
    void setAceleracion(const QPointF &a);

    void actualizarMovimiento(qreal dt);

    QPointF velocidad() const {
        return m_velocidad;
    }
    QPointF aceleracion() const {
        return m_aceleracion;
    }
};

#endif // PERSONAJE_H
