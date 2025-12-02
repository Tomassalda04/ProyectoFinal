#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QTimer>
#include <QVector>
#include <QPixmap>

class Jugador;

class Enemigo : public Personaje
{
    Q_OBJECT

public:
    explicit Enemigo(QGraphicsItem *parent = nullptr);

    void setObjetivo(Jugador *jugador);

signals:
    void jugadorAlcanzado();

private:
    QTimer *m_timerMovimiento;
    QTimer *m_timerDireccion;
    qreal   m_velocidadBase;

    enum Direccion {
        Derecha,
        Izquierda,
        Arriba,
        Abajo
    };

    Direccion m_direccionActual;
    int       m_frameActual;
    qreal     m_tiempoAcumulado;
    qreal     m_tiempoPorFrame;

    QVector<QPixmap> m_framesDer;
    QVector<QPixmap> m_framesIzq;
    QVector<QPixmap> m_framesArriba;
    QVector<QPixmap> m_framesAbajo;

    Jugador *m_objetivo;
    bool     m_enPersecucion;
    qreal    m_radioDeteccion;
    qreal    m_tiempoPersecucionMax;
    qreal    m_tiempoPersecucionAcumulado;

    void cargarSprites();
    void actualizarAnimacion(qreal dt);
    void corregirLimites();
    void manejarColisiones(const QPointF &posAnterior);
    void actualizarEstadoPersecucion(qreal dt);

private slots:
    void actualizarPaso();
    void cambiarDireccionAleatoria();
};

#endif // ENEMIGO_H
