#include "nivel.h"
#include "enemigo.h"
#include "obstaculo.h"
#include "jugador.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>

static const char* RUTA_SPRITE_WIN = ":/Sprites/Fondo/WIN.png";
static const char* RUTA_SPRITE_GAME_OVER = ":/Sprites/Fondo/Game Over.png";

static const int VIEW_W = 1024;
static const int VIEW_H = 572;

Nivel::Nivel(int numeroNivel, QObject *parent)
    : QGraphicsScene(parent),
    m_numeroNivel(numeroNivel)
{
    setSceneRect(0, 0, VIEW_W, VIEW_H);

    switch (m_numeroNivel) {
    case 1: crearNivel1(); break;
    case 2: crearNivel2(); break;
    case 3: crearNivel3(); break;
    default: crearNivel1(); break;
    }
}

void Nivel::crearNivel1()
{
    QPixmap fondo(":/Sprites/SpritesNivel1/fondo1.jpg");
    setSceneRect(0,0,VIEW_W,VIEW_H);

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(VIEW_W,VIEW_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        addPixmap(fondoEscalado)->setPos(0,0);
    }

    QVector<Obstaculo*> obstaculos;
    obstaculos << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs1.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs3.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs5.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs4.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs4.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs4.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs3.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs4.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs2.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs5.png")
               << new Obstaculo(":/Sprites/SpritesNivel1/Obstaculos/Obs5.png");

    const QVector<QPointF> posicionesObstaculos = {
        {430,190},{100,400},{400,420},
        {0,100},{690,220},{125,100},
        {0,400},{380,600},{890,390},{600,420},{850,0}
    };

    for (int i = 0; i < obstaculos.size() && i < posicionesObstaculos.size(); ++i) {
        obstaculos[i]->setPos(posicionesObstaculos[i]);
        addItem(obstaculos[i]);
    }

    QVector<Obstaculo*> cohetes;
    cohetes << new Obstaculo(":/Sprites/SpritesNivel1/Cohete/Coh1.png")
            << new Obstaculo(":/Sprites/SpritesNivel1/Cohete/Coh2.png")
            << new Obstaculo(":/Sprites/SpritesNivel1/Cohete/Coh3.png")
            << new Obstaculo(":/Sprites/SpritesNivel1/Cohete/Coh4.png");

    const QVector<QPointF> posicionesCohetes = {
        {20,50},{950,490},{510,490},{950,50}
    };

    m_cohetes.clear();
    for (int i = 0; i < cohetes.size() && i < posicionesCohetes.size(); ++i) {
        auto c = cohetes[i];
        c->setPos(posicionesCohetes[i]);
        c->setData(0, "cohete");
        addItem(c);
        m_cohetes.push_back(c);
    }

    m_cohetesRecolectados = 0;
    m_cohetesTotales = m_cohetes.size();

    m_jugador = new Jugador();
    addItem(m_jugador);
    m_jugador->setPos(500, 100);
    m_jugador->setFocus();

    connect(m_jugador, &Jugador::coheteRecolectado, this, &Nivel::manejarCoheteRecolectado);

    m_enemigo = new Enemigo();
    addItem(m_enemigo);
    m_enemigo->setPos(100, 300);
    m_enemigo->setObjetivo(m_jugador);
    connect(m_enemigo, &Enemigo::jugadorAlcanzado, this, &Nivel::onJugadorAtrapado);

    Enemigo *enemigo2 = new Enemigo();
    addItem(enemigo2);
    enemigo2->setPos(750, 320);
    enemigo2->setObjetivo(m_jugador);
    connect(enemigo2, &Enemigo::jugadorAlcanzado, this, &Nivel::onJugadorAtrapado);

    m_segundosRestantes = 40;

    if (!m_timerNivel) {
        m_timerNivel = new QTimer(this);
        connect(m_timerNivel, &QTimer::timeout, this, &Nivel::actualizarCuentaAtras);
    }
    m_timerNivel->start(1000);

    if (!m_textoTiempo) {
        m_textoTiempo = addText("");
        m_textoTiempo->setDefaultTextColor(Qt::white);
        m_textoTiempo->setPos(10,10);
    }

    if (!m_textoCohetes) {
        m_textoCohetes = addText("");
        m_textoCohetes->setDefaultTextColor(Qt::white);
        m_textoCohetes->setPos(10,30);
    }

    m_textoTiempo->setPlainText(QString("Tiempo: %1").arg(m_segundosRestantes));
    m_textoCohetes->setPlainText(QString("Cohetes: %1 / %2").arg(m_cohetesRecolectados).arg(m_cohetesTotales));
}

void Nivel::crearNivel2()
{
    QPixmap fondo(":/Sprites/SpritesNivel2/FondoNivel2.png");
    QPixmap fondoEscalado = fondo.isNull() ? QPixmap(VIEW_W, VIEW_H) : fondo.scaledToHeight(VIEW_H, Qt::SmoothTransformation);

    if (fondoEscalado.isNull()) {
        fondoEscalado = QPixmap(VIEW_W, VIEW_H);
        fondoEscalado.fill(Qt::black);
    }

    setSceneRect(0,0,fondoEscalado.width(),fondoEscalado.height());
    addPixmap(fondoEscalado)->setPos(0,0);

    QPixmap spriteCohete(":/Sprites/SpritesNivel2/Cohete/nave1.png");
    if (spriteCohete.isNull()) {
        spriteCohete = QPixmap(40,80);
        spriteCohete.fill(Qt::red);
    }

    int anchoDeseado = 80;
    int altoDeseado = 80;

    spriteCohete = spriteCohete.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_cohete = addPixmap(spriteCohete);

    qreal xInicial = 100.0;
    qreal yInicial = sceneRect().height()/2.0 - m_cohete->boundingRect().height()/2.0;

    m_cohete->setPos(xInicial, yInicial);
}

void Nivel::crearNivel3()
{
    QPixmap fondo(":/Sprites/SpritesNivel3/FondoNivel3.png");
    setSceneRect(0,0,VIEW_W,VIEW_H);

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(VIEW_W,VIEW_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        addPixmap(fondoEscalado)->setPos(0,0);
    }

    // Por si quedara algun mensaje de una partida anterior
    if (m_mensajeOverlay) {
        removeItem(m_mensajeOverlay);
        delete m_mensajeOverlay;
        m_mensajeOverlay = nullptr;
    }

    QPixmap spriteMira(":/Sprites/SpritesNivel3/MiraNivel3.png");
    m_mira = addPixmap(spriteMira);

    if (m_mira) {
        qreal x = sceneRect().width()  / 2.0 - m_mira->boundingRect().width()  / 2.0;
        qreal y = sceneRect().height() / 2.0 - m_mira->boundingRect().height() / 2.0;
        m_mira->setPos(x, y);
        m_mira->setZValue(5);
    }

    m_aliensNivel3.clear();
    m_aliensEscapados          = 0;
    m_aliensEscapadosMax       = 5;
    m_oleadaActual             = 0;
    m_totalOleadasNivel3       = 5;
    m_aliensPorOleada          = 2;
    m_esperandoSiguienteOleada = false;

    crearOleadaAliens();

    if (!m_timerAliensNivel3) {
        m_timerAliensNivel3 = new QTimer(this);
        connect(m_timerAliensNivel3, &QTimer::timeout, this, &Nivel::actualizarAliensNivel3);
    }
    m_timerAliensNivel3->start(30);
}

void Nivel::actualizarCuentaAtras()
{
    --m_segundosRestantes;

    if (m_textoTiempo) {
        m_textoTiempo->setPlainText(QString("Tiempo: %1").arg(m_segundosRestantes));
    }

    if (m_segundosRestantes <= 0) {
        if (m_timerNivel) {
            m_timerNivel->stop();
        }
        mostrarGameOverYEmitir();
    }
}

void Nivel::manejarCoheteRecolectado(QGraphicsItem *cohete)
{
    if (!cohete)
        return;
    if (!m_cohetes.contains(cohete))
        return;

    removeItem(cohete);
    m_cohetes.removeOne(cohete);
    delete cohete;

    ++m_cohetesRecolectados;

    if (m_textoCohetes) {
        m_textoCohetes->setPlainText(QString("Cohetes: %1 / %2").arg(m_cohetesRecolectados).arg(m_cohetesTotales));
    }

    if (m_cohetesRecolectados >= m_cohetesTotales) {
        if (m_timerNivel) {
            m_timerNivel->stop();
        }
        mostrarVictoriaYEmitir();
    }
}

void Nivel::onJugadorAtrapado()
{
    if (m_timerNivel) {
        m_timerNivel->stop();
    }

    mostrarGameOverYEmitir();
}

void Nivel::moverCohete(int dx,int dy)
{
    if (!m_cohete)
        return;

    QPointF pos = m_cohete->pos();
    pos.setX(pos.x() + dx);
    pos.setY(pos.y() + dy);

    qreal minX = 0.0;
    qreal maxX = sceneRect().width()  - m_cohete->boundingRect().width();
    qreal minY = 0.0;
    qreal maxY = sceneRect().height() - m_cohete->boundingRect().height();

    if (pos.x() < minX) pos.setX(minX);
    if (pos.x() > maxX) pos.setX(maxX);
    if (pos.y() < minY) pos.setY(minY);
    if (pos.y() > maxY) pos.setY(maxY);

    m_cohete->setPos(pos);
}

void Nivel::moverMira(int dx,int dy)
{
    if (!m_mira)
        return;

    QPointF pos = m_mira->pos();
    pos.rx() += dx;
    pos.ry() += dy;

    qreal minX = 0.0;
    qreal maxX = sceneRect().width()  - m_mira->boundingRect().width();
    qreal minY = 0.0;
    qreal maxY = sceneRect().height() - m_mira->boundingRect().height();

    if (pos.x() < minX) pos.setX(minX);
    if (pos.x() > maxX) pos.setX(maxX);
    if (pos.y() < minY) pos.setY(minY);
    if (pos.y() > maxY) pos.setY(maxY);

    m_mira->setPos(pos);
}

void Nivel::disparar()
{
    if (!m_mira)
        return;

    const auto colisiones = m_mira->collidingItems();
    bool eliminoAlguno = false;

    for (QGraphicsItem *item : colisiones) {
        if (Enemigo *alien = dynamic_cast<Enemigo*>(item)) {
            m_aliensNivel3.removeOne(alien);
            removeItem(alien);
            delete alien;
            eliminoAlguno = true;
        }
    }

    if (!eliminoAlguno)
        return;

    comprobarEstadoOleadasNivel3();
}

void Nivel::crearOleadaAliens()
{
    if (m_oleadaActual >= m_totalOleadasNivel3)
        return;

    ++m_oleadaActual;

    int aliensEnEstaOleada = m_aliensPorOleada + (m_oleadaActual - 1);

    const qreal margenX = 60.0;
    const qreal anchoEscena = sceneRect().width();
    const qreal yInicial = 280.0;

    QVector<qreal> posicionesX;
    const qreal separacionMin = 80.0;

    for (int i = 0; i < aliensEnEstaOleada; ++i) {
        Enemigo *alien = new Enemigo(Enemigo::AlienNivel3);
        addItem(alien);

        qreal anchoAlien = alien->boundingRect().width();
        qreal minX = margenX;
        qreal maxX = anchoEscena - anchoAlien - margenX;
        if (maxX < minX)
            maxX = minX;

        double minXd = static_cast<double>(minX);
        double maxXd = static_cast<double>(maxX);

        qreal x = 0.0;
        int intentos = 0;
        bool valido = false;

        while (!valido && intentos < 20) {
            double t = QRandomGenerator::global()->generateDouble();
            double randomX = minXd + t * (maxXd - minXd);
            x = static_cast<qreal>(randomX);
            valido = true;
            for (qreal usado : posicionesX) {
                qreal diff = x - usado;
                if (diff < 0)
                    diff = -diff;
                if (diff < separacionMin) {
                    valido = false;
                    break;
                }
            }
            ++intentos;
        }

        posicionesX.push_back(x);

        alien->setPos(x, yInicial);

        const qreal yMax = sceneRect().height();
        alien->configurarMovimientoDescendente(yMax, 40.0);

        m_aliensNivel3.push_back(alien);
    }
}

void Nivel::actualizarAliensNivel3()
{
    if (m_aliensNivel3.isEmpty())
        return;

    const qreal limiteY = sceneRect().height();
    QVector<Enemigo*> aEliminar;

    for (Enemigo *alien : m_aliensNivel3) {
        if (!alien)
            continue;
        if (alien->pos().y() >= limiteY)
            aEliminar.push_back(alien);
    }

    if (aEliminar.isEmpty())
        return;

    for (Enemigo *alien : aEliminar) {
        m_aliensNivel3.removeOne(alien);
        removeItem(alien);
        delete alien;
        ++m_aliensEscapados;
    }

    if (m_aliensEscapados >= m_aliensEscapadosMax) {
        if (m_timerAliensNivel3)
            m_timerAliensNivel3->stop();

        mostrarGameOverYEmitir();
        return;
    }

    comprobarEstadoOleadasNivel3();
}

void Nivel::mostrarVictoriaYEmitir()
{
    if (m_mensajeOverlay) {
        removeItem(m_mensajeOverlay);
        delete m_mensajeOverlay;
        m_mensajeOverlay = nullptr;
    }

    QPixmap winPixmap(QString::fromUtf8(RUTA_SPRITE_WIN));
    if (winPixmap.isNull()) {
        winPixmap = QPixmap(300, 100);
        winPixmap.fill(Qt::green);
    }

    m_mensajeOverlay = addPixmap(winPixmap);
    if (m_mensajeOverlay) {
        qreal x = sceneRect().center().x() - m_mensajeOverlay->boundingRect().width() / 2.0;
        qreal y = sceneRect().center().y() - m_mensajeOverlay->boundingRect().height() / 2.0;
        m_mensajeOverlay->setPos(x, y);
        m_mensajeOverlay->setZValue(100);
    }

    QTimer::singleShot(3000, this, [this]() {
        if (m_mensajeOverlay) {
            removeItem(m_mensajeOverlay);
            delete m_mensajeOverlay;
            m_mensajeOverlay = nullptr;
        }
        emit nivelCompletado(m_numeroNivel);
    });
}

void Nivel::mostrarGameOverYEmitir()
{
    if (m_mensajeOverlay) {
        removeItem(m_mensajeOverlay);
        delete m_mensajeOverlay;
        m_mensajeOverlay = nullptr;
    }

    QPixmap goPixmap(QString::fromUtf8(RUTA_SPRITE_GAME_OVER));
    if (goPixmap.isNull()) {
        goPixmap = QPixmap(300, 100);
        goPixmap.fill(Qt::red);
    }

    m_mensajeOverlay = addPixmap(goPixmap);
    if (m_mensajeOverlay) {
        qreal x = sceneRect().center().x() - m_mensajeOverlay->boundingRect().width() / 2.0;
        qreal y = sceneRect().center().y() - m_mensajeOverlay->boundingRect().height() / 2.0;
        m_mensajeOverlay->setPos(x, y);
        m_mensajeOverlay->setZValue(100);
    }

    QTimer::singleShot(3000, this, [this]() {
        if (m_mensajeOverlay) {
            removeItem(m_mensajeOverlay);
            delete m_mensajeOverlay;
            m_mensajeOverlay = nullptr;
        }
        emit nivelFallado(m_numeroNivel);
    });
}

void Nivel::comprobarEstadoOleadasNivel3()
{
    if (!m_aliensNivel3.isEmpty())
        return;

    if (m_oleadaActual >= m_totalOleadasNivel3) {
        if (m_timerAliensNivel3)
            m_timerAliensNivel3->stop();

        mostrarVictoriaYEmitir();
    } else {
        if (m_esperandoSiguienteOleada)
            return;

        m_esperandoSiguienteOleada = true;

        QTimer::singleShot(2000, this, [this]() {
            m_esperandoSiguienteOleada = false;
            crearOleadaAliens();
        });
    }
}
