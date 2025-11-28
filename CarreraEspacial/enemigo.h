#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QTimer>
#include <QVector>
#include <QPixmap>

class Enemigo : public Personaje
{
    Q_OBJECT

public:
    explicit Enemigo(QGraphicsItem *parent = nullptr);

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

    void cargarSprites();
    void actualizarAnimacion(qreal dt);
    void corregirLimites();      // que no salga de la pantalla

private slots:
    void actualizarPaso();       // movimiento + animación
    void cambiarDireccionAleatoria();
};

#endif // ENEMIGO_H
