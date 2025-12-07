#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QTimer>
#include <QVector>
#include <QPixmap>

class QGraphicsItem;

class Enemigo : public Personaje
{
    Q_OBJECT

public:
    enum TipoSprite {Soldado, AlienNivel3};

    explicit Enemigo(TipoSprite tipo = Soldado, QGraphicsItem *parent = nullptr);

    void setObjetivo(QGraphicsItem *objetivo);
    void setRadioDeteccion(qreal radio);

    // los aliens del nivel 3 solo bajan en Y
    void configurarMovimientoDescendente(qreal yMax, qreal velocidadY);

signals:
    void jugadorAlcanzado();

private:
    QTimer *m_timerMovimiento;
    QTimer *m_timerDireccion;
    qreal   m_velocidadBase;

    enum Direccion {Derecha, Izquierda, Arriba, Abajo};

    Direccion m_direccionActual;
    int       m_frameActual;
    qreal     m_tiempoAcumulado;
    qreal     m_tiempoPorFrame;

    QVector<QPixmap> m_framesDer;
    QVector<QPixmap> m_framesIzq;
    QVector<QPixmap> m_framesArriba;
    QVector<QPixmap> m_framesAbajo;

    TipoSprite     m_tipoSprite;
    QGraphicsItem *m_objetivo;
    bool           m_enPersecucion;
    qreal          m_radioDeteccion;
    qreal          m_tiempoPersecucionMax;
    qreal          m_tiempoPersecucionAcumulado;

    // Modo de movimiento hacia abajo, aliens nivel 3
    bool  m_movimientoDescendente;
    qreal m_yMaxDescendente;
    qreal m_velocidadDescendente;

    void cargarSprites();
    void actualizarAnimacion(qreal dt);
    void corregirLimites();
    void manejarColisiones(const QPointF &posAnterior);
    void actualizarEstadoPersecucion(qreal dt);

private slots:
    void actualizarPaso();
    void cambiarDireccionAleatoria();
};

#endif
