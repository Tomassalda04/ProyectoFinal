#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"
#include <QVector>
#include <QPixmap>

class QTimer;
class QKeyEvent;

class Jugador : public Personaje
{
    Q_OBJECT

public:
    explicit Jugador(QGraphicsItem *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void coheteRecolectado(QGraphicsItem *cohete);

private:
    QTimer *m_timerMovimiento;
    qreal   m_velocidadBase;

    enum Direccion {Derecha, Izquierda, Arriba, Abajo, Quieto};

    Direccion m_direccionActual;
    int       m_frameActual;
    qreal     m_tiempoAcumulado;
    qreal     m_tiempoPorFrame;

    QVector<QPixmap> m_framesDer;
    QVector<QPixmap> m_framesIzq;
    QVector<QPixmap> m_framesArriba;
    QVector<QPixmap> m_framesAbajo;

    void cargarSprites();
    void actualizarAnimacion(qreal dt);
    void corregirLimites();
    void manejarColisiones(const QPointF &posAnterior);

private slots:
    void actualizarPaso();
};

#endif
