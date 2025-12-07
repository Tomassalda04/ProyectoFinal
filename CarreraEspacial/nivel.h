#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>

#include "jugador.h"

class Enemigo;
class QGraphicsItem;

class Nivel : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Nivel(int numeroNivel, QObject *parent = nullptr);

    int numeroNivel() const { return m_numeroNivel; }

    QGraphicsPixmapItem *cohete() const { return m_cohete; }

    // Controles que usa MainWindow
    void moverCohete(int dx, int dy);   // Nivel 2
    void moverMira(int dx, int dy);     // Nivel 3
    void disparar();                    // Nivel 3

signals:
    void nivelCompletado(int numeroNivel);
    void nivelFallado(int numeroNivel);

private slots:
    // Nivel 1
    void actualizarCuentaAtras();
    void manejarCoheteRecolectado(QGraphicsItem *cohete);
    void onJugadorAtrapado();

    // Nivel 3
    void actualizarAliensNivel3();

private:
    void crearNivel1();
    void crearNivel2();
    void crearNivel3();

    // Nivel 3
    void crearOleadaAliens();
    void comprobarEstadoOleadasNivel3();

    // Mensajes de fin (WIN y GAME OVER)
    void mostrarVictoriaYEmitir();
    void mostrarGameOverYEmitir();

    int m_numeroNivel = 0;

    // NIVEL 1
    Enemigo *m_enemigo = nullptr;
    Jugador *m_jugador = nullptr;

    QVector<QGraphicsItem*> m_cohetes;
    int m_cohetesRecolectados = 0;
    int m_cohetesTotales      = 0;

    QTimer *m_timerNivel      = nullptr;
    int m_segundosRestantes   = 40;
    QGraphicsTextItem *m_textoTiempo  = nullptr;
    QGraphicsTextItem *m_textoCohetes = nullptr;

    QGraphicsPixmapItem *m_cohete = nullptr;

    // NIVEL 3
    QGraphicsPixmapItem *m_mira = nullptr;
    QVector<Enemigo*>   m_aliensNivel3;

    QTimer *m_timerAliensNivel3   = nullptr;
    int     m_aliensEscapados     = 0;
    int     m_aliensEscapadosMax  = 5;
    int     m_oleadaActual        = 0;
    int     m_totalOleadasNivel3  = 5;
    int     m_aliensPorOleada     = 3;

    // Para la pausa entre oleadas, 2 segundos
    bool    m_esperandoSiguienteOleada = false;

    // Sprite de WIN y GAME OVER
    QGraphicsPixmapItem *m_mensajeOverlay = nullptr;
};

#endif
