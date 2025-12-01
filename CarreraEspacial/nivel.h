#ifndef NIVEL_H
#define NIVEL_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QVector>
#include "jugador.h"
#include "personaje.h"
class Enemigo;
class Nivel:public QGraphicsScene{
    Q_OBJECT
public:
    explicit Nivel(int numeroNivel,QObject *parent=nullptr);
signals:
    void nivelCompletado(int numeroNivel);
    void nivelFallado(int numeroNivel);
private slots:
    void actualizarCuentaAtras();
    void manejarCoheteRecolectado(QGraphicsItem *cohete);
    void onJugadorAtrapado();
private:
    int m_numeroNivel;
    void crearNivel1();
    void crearNivel2();
    void crearNivel3();
    Enemigo *m_enemigo=nullptr;
    Jugador *m_jugador=nullptr;
    QVector<QGraphicsItem*> m_cohetes;
    int m_cohetesRecolectados=0;
    int m_cohetesTotales=0;
    QTimer *m_timerNivel=nullptr;
    int m_segundosRestantes=40;
    QGraphicsTextItem *m_textoTiempo=nullptr;
    QGraphicsTextItem *m_textoCohetes=nullptr;
};
#endif
