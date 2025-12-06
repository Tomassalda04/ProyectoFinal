#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>

#include "jugador.h"

class Enemigo;

class Nivel : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Nivel(int numeroNivel, QObject *parent = nullptr);

    QGraphicsPixmapItem *cohete() const { return m_cohete; }
    void moverCohete(int dx, int dy);

    QGraphicsPixmapItem *mira() const { return m_mira; }
    void moverMira(int dx, int dy);
    void disparar();

signals:
    void nivelCompletado(int numeroNivel);
    void nivelFallado(int numeroNivel);

private slots:
    void actualizarCuentaAtras();
    void manejarCoheteRecolectado(QGraphicsItem *cohete);
    void onJugadorAtrapado();

    void actualizarAliensNivel3();

private:
    int m_numeroNivel;

    void crearNivel1();
    void crearNivel2();
    void crearNivel3();

    void crearOleadaAliens();

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

    QGraphicsPixmapItem *m_mira = nullptr;
    QVector<Enemigo*>   m_aliensNivel3;

    QTimer *m_timerAliensNivel3   = nullptr;
    int     m_aliensEscapados     = 0;
    int     m_aliensEscapadosMax  = 5;
    int     m_oleadaActual        = 0;
    int     m_totalOleadasNivel3  = 5;
    int     m_aliensPorOleada     = 3;
};

#endif // NIVEL_H
