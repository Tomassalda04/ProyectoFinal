#ifndef NAVEESPACIAL_H
#define NAVEESPACIAL_H

#include "personaje.h"
#include <QTimer>

class NaveEspacial : public Personaje
{
    Q_OBJECT

public:
    explicit NaveEspacial(QGraphicsItem *parent = nullptr);

    void setVelocidadBase(qreal v);
    void setVelocidadVertical(qreal v);
    void detener();
    // 🔹 Nuevo: volver a poner en marcha el movimiento después de detenerla
    void iniciarMovimiento();

    // Estas se llaman desde MainWindow al presionar/soltar teclas
    void setArribaPresionado(bool v);
    void setAbajoPresionado(bool v);
    void setDerechaPresionado(bool v);

signals:
    void colisionConObstaculo();

private slots:
    void actualizarPaso();

private:
    QTimer *m_timerMovimiento;

    qreal   m_velocidadBase;       // avance horizontal constante
    qreal   m_velocidadExtraDer;   // extra al presionar Derecha
    qreal   m_velocidadVertical;   // velocidad arriba/abajo

    bool    m_arribaPresionado;
    bool    m_abajoPresionado;
    bool    m_derechaPresionada;

    void corregirLimites();
    void actualizarCamara();
    void comprobarColisiones();
};

#endif // NAVEESPACIAL_H



