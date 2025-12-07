#ifndef NIVEL2_H
#define NIVEL2_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>
#include <QPointF>

class NaveEspacial;
class Obstaculo;

class QMediaPlayer;
class QAudioOutput;

class Nivel2 : public QGraphicsScene
{
    Q_OBJECT

    struct MeteoritoData {
        Obstaculo *item = nullptr;
        QPointF    velocidad;
        QPointF    aceleracion;
    };

public:
    explicit Nivel2(QObject *parent = nullptr);

    NaveEspacial *nave() const { return m_nave; }

    void iniciar();

signals:
    void nivelCompletado();
    void nivelFallado();

private slots:
    void actualizarCuentaAtras();
    void manejarColisionNave();
    void comprobarFinDeNivel();
    void crearMeteorito();

private:
    void crearNivel2();
    void generarObstaculosAleatorios();
    void actualizarMeteoritos();
    void actualizarPosicionHUD();
    void mostrarVictoriaYEmitir();
    void mostrarGameOverYEmitir();

    NaveEspacial          *m_nave              = nullptr;
    QVector<Obstaculo*>    m_obstaculos;
    QVector<MeteoritoData> m_meteoritos;

    QTimer                *m_timerCuentaAtras  = nullptr;
    QTimer                *m_timerLogica       = nullptr;
    QTimer                *m_timerMeteoritos   = nullptr;

    int                    m_tiempoRestante    = 0;
    QGraphicsTextItem     *m_textoTiempo       = nullptr;
    bool                   m_nivelTerminado    = false;

    bool                   m_hudConectado      = false;
    bool                   m_invulnerableInicio = false;

    QGraphicsPixmapItem   *m_mensajeOverlay    = nullptr;

    QMediaPlayer   *m_musicaNivel2 = nullptr;
    QAudioOutput   *m_audioNivel2  = nullptr;
};

#endif // NIVEL2_H
