// nivel.h
#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>
#include <QPointF>

class NaveEspacial;
class Obstaculo;

class Nivel : public QGraphicsScene
{
    Q_OBJECT

    // Estructura para manejar meteoritos con física (parábola)
    struct MeteoritoData {
        Obstaculo *item   = nullptr;
        QPointF    velocidad;    // (vx, vy)
        QPointF    aceleracion;  // (ax, ay)
    };

public:
    explicit Nivel(int numeroNivel, QObject *parent = nullptr);

    int numeroNivel() const { return m_numeroNivel; }
    NaveEspacial *nave() const { return m_nave; }

    // Llamar al entrar al nivel
    void iniciar();

signals:
    void nivelCompletado(int numeroNivel);
    void nivelFallado(int numeroNivel);

private slots:
    void actualizarCuentaAtras();
    void manejarColisionNave();
    void comprobarFinDeNivel();

    // Se llama periódicamente para crear meteoritos
    void crearMeteorito();

private:
    int  m_numeroNivel;

    void crearNivel1();
    void crearNivel2();
    void crearNivel3();

    // --- Solo nivel 2 ---
    void generarObstaculosAleatorios();
    void actualizarMeteoritos();   // mover meteoritos y limpiarlos
    void actualizarPosicionHUD();  // mantener HUD fijo en la esquina superior izquierda

    NaveEspacial          *m_nave            = nullptr;

    QVector<Obstaculo*>    m_obstaculos;     // obstáculos fijos del nivel
    QVector<MeteoritoData> m_meteoritos;     // meteoritos con trayectoria parabólica

    QTimer                *m_timerCuentaAtras  = nullptr;
    QTimer                *m_timerLogica       = nullptr;
    QTimer                *m_timerMeteoritos   = nullptr;

    int                    m_tiempoRestante    = 0;
    QGraphicsTextItem     *m_textoTiempo       = nullptr;
    bool                   m_nivelTerminado    = false;

    bool                   m_hudConectado      = false; // para no conectar 2 veces al scroll
};

#endif // NIVEL_H

