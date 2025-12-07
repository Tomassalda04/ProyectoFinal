#include "nivel2.h"
#include "naveespacial.h"
#include "obstaculo.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRandomGenerator>
#include <QFont>
#include <QGraphicsView>
#include <QStringList>
#include <QPainter>
#include <QtMath>
#include <QScrollBar>
#include <QTimer>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

static const int  VIEW_W = 1024;
static const int  VIEW_H = 572;

static const qreal TIEMPO_OBJETIVO_NIVEL2   = 40.0;
static const qreal VELOCIDAD_BASE_NAVE_N2   = 350.0;
static const int   MARGEN_EXTRA_ESCENA_N2   = 600;

static const qreal GRAVEDAD_METEORITO       = 600.0;

static const char* RUTA_SPRITE_WIN       = ":/Sprites/Fondo/WIN.png";
static const char* RUTA_SPRITE_GAME_OVER = ":/Sprites/Fondo/Game Over.png";

Nivel2::Nivel2(QObject *parent)
    : QGraphicsScene(parent)
{
    crearNivel2();

    m_musicaNivel2 = new QMediaPlayer(this);
    m_audioNivel2  = new QAudioOutput(this);

    m_musicaNivel2->setAudioOutput(m_audioNivel2);

    m_musicaNivel2->setSource(QUrl("qrc:/Sprites/Sonido/musica_nivel2.mp3"));

    m_musicaNivel2->setLoops(QMediaPlayer::Infinite);

    m_audioNivel2->setVolume(0.4f);
}


void Nivel2::crearNivel2()
{
    QPixmap fondo(":/Sprites/SpritesNivel2/FondoNivel2.png");

    QPixmap fondoEscalado = fondo.isNull()
                                ? QPixmap(VIEW_W, VIEW_H)
                                : fondo.scaledToHeight(VIEW_H, Qt::SmoothTransformation);

    if (fondoEscalado.isNull()) {
        fondoEscalado = QPixmap(VIEW_W, VIEW_H);
        fondoEscalado.fill(Qt::black);
    }

    const qreal distObjetivo = VELOCIDAD_BASE_NAVE_N2 * TIEMPO_OBJETIVO_NIVEL2;
    int anchoDeseado = static_cast<int>(distObjetivo) + MARGEN_EXTRA_ESCENA_N2;

    if (anchoDeseado < VIEW_W)
        anchoDeseado = VIEW_W;

    if (anchoDeseado < fondoEscalado.width())
        anchoDeseado = fondoEscalado.width();

    if (anchoDeseado > fondoEscalado.width()) {
        QPixmap fondoLargo(anchoDeseado, VIEW_H);
        fondoLargo.fill(Qt::black);

        QPainter painter(&fondoLargo);
        int x = 0;
        while (x < anchoDeseado) {
            painter.drawPixmap(x, 0, fondoEscalado);
            x += fondoEscalado.width();
        }
        painter.end();

        fondoEscalado = fondoLargo;
    }

    setSceneRect(0, 0, fondoEscalado.width(), fondoEscalado.height());
    addPixmap(fondoEscalado)->setPos(0, 0);

    QPixmap spriteCohete(":/Sprites/SpritesNivel2/Cohete/nave1.png");
    if (spriteCohete.isNull()) {
        spriteCohete = QPixmap(40, 80);
        spriteCohete.fill(Qt::red);
    }

    spriteCohete = spriteCohete.scaled(80,
                                       80,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);

    m_nave = new NaveEspacial();
    m_nave->setPixmap(spriteCohete);
    addItem(m_nave);

    QRectF limites = sceneRect();
    qreal xInicial = limites.left() + 100.0;
    qreal yInicial = limites.height() / 2.0
                     - m_nave->boundingRect().height() / 2.0;

    m_nave->setPos(xInicial, yInicial);

    m_nave->setVelocidadBase(VELOCIDAD_BASE_NAVE_N2);
    m_nave->setVelocidadVertical(420.0);

    connect(m_nave, &NaveEspacial::colisionConObstaculo,
            this,   &Nivel2::manejarColisionNave);

    m_obstaculos.clear();
    generarObstaculosAleatorios();

    m_textoTiempo = addText("Tiempo: 40");
    m_textoTiempo->setDefaultTextColor(Qt::white);
    m_textoTiempo->setFont(QFont("Arial", 18, QFont::Bold));
    m_textoTiempo->setZValue(10);

    actualizarPosicionHUD();
}

void Nivel2::iniciar()
{
    m_nivelTerminado = false;
    m_tiempoRestante = 40;

    for (int i = m_meteoritos.size() - 1; i >= 0; --i) {
        Obstaculo *met = m_meteoritos[i].item;
        if (met) {
            removeItem(met);
            delete met;
        }
    }
    m_meteoritos.clear();

    m_invulnerableInicio = true;
    QTimer::singleShot(2000, this, [this]() {
        m_invulnerableInicio = false;
    });

    if (!m_timerCuentaAtras) {
        m_timerCuentaAtras = new QTimer(this);
        m_timerCuentaAtras->setInterval(1000);
        connect(m_timerCuentaAtras, &QTimer::timeout,
                this,               &Nivel2::actualizarCuentaAtras);
    }
    m_timerCuentaAtras->start();

    if (!m_timerLogica) {
        m_timerLogica = new QTimer(this);
        m_timerLogica->setInterval(16);
        connect(m_timerLogica, &QTimer::timeout,
                this,          &Nivel2::comprobarFinDeNivel);
    }
    m_timerLogica->start();

    if (!m_timerMeteoritos) {
        m_timerMeteoritos = new QTimer(this);
        m_timerMeteoritos->setInterval(1500);
        connect(m_timerMeteoritos, &QTimer::timeout,
                this,              &Nivel2::crearMeteorito);
    }
    m_timerMeteoritos->start();

    if (m_textoTiempo) {
        m_textoTiempo->setPlainText(QStringLiteral("Tiempo: %1").arg(m_tiempoRestante));
        actualizarPosicionHUD();
    }

    if (!m_hudConectado) {
        const auto vs = views();
        if (!vs.isEmpty()) {
            QGraphicsView *view = vs.first();
            if (view && view->horizontalScrollBar()) {
                connect(view->horizontalScrollBar(), &QScrollBar::valueChanged,
                        this, [this](int) { actualizarPosicionHUD(); });
                m_hudConectado = true;
            }
        }
    }

    if (m_nave) {
        m_nave->setArribaPresionado(false);
        m_nave->setAbajoPresionado(false);
        m_nave->setDerechaPresionado(false);

        QRectF limites = sceneRect();
        qreal xInicial = limites.left() + 100.0;
        qreal yInicial = limites.height() / 2.0
                         - m_nave->boundingRect().height() / 2.0;
        m_nave->setPos(xInicial, yInicial);

        m_nave->iniciarMovimiento();
    }

    if (m_musicaNivel2) {
        m_musicaNivel2->play();
    }
}


void Nivel2::generarObstaculosAleatorios()
{
    if (!m_nave)
        return;

    QRectF limites = sceneRect();

    const qreal MARGEN_INICIO_X = 2000.0;
    const qreal xInicio = m_nave->pos().x() + MARGEN_INICIO_X;
    const qreal xFinal  = limites.right() - 200.0;
    if (xInicio >= xFinal)
        return;

    const qreal yMin = limites.top();
    const qreal yMax = limites.bottom();

    const qreal DISTANCIA_MINIMA     = 80.0;
    const int   NUM_OBSTACULOS       = 80;
    const int   NUM_OBSTACULOS_BANDA = 40;

    const int xMinInt = static_cast<int>(xInicio);
    const int xMaxInt = static_cast<int>(xFinal);
    const int yMinInt = static_cast<int>(yMin);
    const int yMaxInt = static_cast<int>(yMax);

    if (xMaxInt <= xMinInt || yMaxInt <= yMinInt)
        return;

    QStringList spritesObstaculos = {
        ":/Sprites/SpritesNivel2/Obstaculos/Obs1.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs2.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs3.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs4.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs4.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs4.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs4.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs4.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs5.png"
    };

    QStringList spritesBanda = {
        ":/Sprites/SpritesNivel2/Obstaculos/Obs1.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs2.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs3.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs5.png"
    };

    auto colocarObstaculo = [&](const QString &rutaSprite,
                                int xMinLocal, int xMaxLocal,
                                int yMinLocal, int yMaxLocal)
    {
        Obstaculo *ob = new Obstaculo(rutaSprite);
        QRectF rectLocal = ob->boundingRect();

        bool colocado = false;

        for (int intento = 0; intento < 50 && !colocado; ++intento) {
            int xMaxForObst = xMaxLocal - static_cast<int>(rectLocal.width());
            int yMaxForObst = yMaxLocal - static_cast<int>(rectLocal.height());

            if (xMaxForObst <= xMinLocal || yMaxForObst <= yMinLocal)
                break;

            int randXInt = QRandomGenerator::global()->bounded(xMinLocal, xMaxForObst);
            int randYInt = QRandomGenerator::global()->bounded(yMinLocal, yMaxForObst);

            qreal x = static_cast<qreal>(randXInt);
            qreal y = static_cast<qreal>(randYInt);

            QRectF nuevoRect(x, y, rectLocal.width(), rectLocal.height());

            bool solapa = false;
            for (Obstaculo *existente : m_obstaculos) {
                if (!existente)
                    continue;

                QRectF rExist   = existente->sceneBoundingRect();
                QRectF rInflado = rExist.adjusted(
                    -DISTANCIA_MINIMA, -DISTANCIA_MINIMA,
                    DISTANCIA_MINIMA,  DISTANCIA_MINIMA
                    );

                if (rInflado.intersects(nuevoRect)) {
                    solapa = true;
                    break;
                }
            }

            if (!solapa) {
                ob->setPos(x, y);
                addItem(ob);
                m_obstaculos.push_back(ob);
                colocado = true;
            }
        }

        if (!colocado) {
            delete ob;
        }
    };

    for (int i = 0; i < NUM_OBSTACULOS; ++i) {
        int idx = QRandomGenerator::global()->bounded(spritesObstaculos.size());
        QString rutaSprite = spritesObstaculos.at(idx);

        colocarObstaculo(rutaSprite, xMinInt, xMaxInt, yMinInt, yMaxInt);
    }

    const qreal ALTURA_BANDA = (yMax - yMin) * 0.22;

    int topMinInt    = yMinInt;
    int topMaxInt    = yMinInt + static_cast<int>(ALTURA_BANDA);
    int bottomMaxInt = yMaxInt;
    int bottomMinInt = yMaxInt - static_cast<int>(ALTURA_BANDA);

    if (topMaxInt > yMaxInt)     topMaxInt = yMaxInt;
    if (bottomMinInt < yMinInt) bottomMinInt = yMinInt;

    for (int i = 0; i < NUM_OBSTACULOS_BANDA; ++i) {
        int idx = QRandomGenerator::global()->bounded(spritesBanda.size());
        QString rutaSprite = spritesBanda.at(idx);

        bool enArriba = (QRandomGenerator::global()->bounded(2) == 0);

        if (enArriba && topMaxInt > topMinInt) {
            colocarObstaculo(rutaSprite, xMinInt, xMaxInt, topMinInt, topMaxInt);
        } else if (!enArriba && bottomMaxInt > bottomMinInt) {
            colocarObstaculo(rutaSprite, xMinInt, xMaxInt, bottomMinInt, bottomMaxInt);
        }
    }
}

void Nivel2::crearMeteorito()
{
    if (m_nivelTerminado)
        return;

    if (views().isEmpty())
        return;

    QGraphicsView *view = views().first();

    QRect visibleViewport = view->viewport()->rect();
    QRectF visibleSceneRect = view->mapToScene(visibleViewport).boundingRect();

    QString rutaSpriteMeteorito = ":/Sprites/SpritesNivel2/Obstaculos/Obs7.png";

    Obstaculo *meteorito = new Obstaculo(rutaSpriteMeteorito);
    QRectF rectLocal = meteorito->boundingRect();

    const qreal ANCHO_ZONA_DERECHA = 400.0;

    qreal xMax = visibleSceneRect.right() - rectLocal.width();
    qreal xMin = xMax - ANCHO_ZONA_DERECHA;
    if (xMin < visibleSceneRect.left())
        xMin = visibleSceneRect.left();

    if (xMax <= xMin) {
        delete meteorito;
        return;
    }

    int xMinInt = static_cast<int>(xMin);
    int xMaxInt = static_cast<int>(xMax);

    int randXInt = QRandomGenerator::global()->bounded(xMinInt, xMaxInt);
    qreal x = static_cast<qreal>(randXInt);

    qreal yBase   = visibleSceneRect.top() - rectLocal.height() - 20.0;
    qreal offsetY = QRandomGenerator::global()->bounded(60.0);
    qreal y       = yBase + offsetY;

    meteorito->setPos(x, y);
    meteorito->setZValue(2);
    addItem(meteorito);

    MeteoritoData data;
    data.item = meteorito;

    qreal angleDegMin = 2.0;
    qreal angleDegMax = 5.0;

    qreal speedMin = 200.0;
    qreal speedMax = 200.0;

    qreal angleDeg = angleDegMin +
                     QRandomGenerator::global()->bounded(angleDegMax - angleDegMin);
    qreal angleRad = qDegreesToRadians(angleDeg);

    qreal speed = speedMin +
                  QRandomGenerator::global()->bounded(speedMax - speedMin);

    qreal velX = -speed * qCos(angleRad);
    qreal velY =  speed * qSin(angleRad);

    data.velocidad   = QPointF(velX, velY);
    data.aceleracion = QPointF(0.0, GRAVEDAD_METEORITO);

    m_meteoritos.push_back(data);
}

void Nivel2::actualizarMeteoritos()
{
    if (m_meteoritos.isEmpty())
        return;

    const qreal dt = m_timerLogica
                         ? static_cast<qreal>(m_timerLogica->interval()) / 1000.0
                         : 0.016;

    QRectF limites = sceneRect();

    for (int i = m_meteoritos.size() - 1; i >= 0; --i) {
        MeteoritoData &data = m_meteoritos[i];
        Obstaculo *met = data.item;
        if (!met) {
            m_meteoritos.remove(i);
            continue;
        }

        data.velocidad.setX(data.velocidad.x() + data.aceleracion.x() * dt);
        data.velocidad.setY(data.velocidad.y() + data.aceleracion.y() * dt);

        qreal dx = data.velocidad.x() * dt;
        qreal dy = data.velocidad.y() * dt;
        met->moveBy(dx, dy);

        qreal x = met->x();
        qreal y = met->y();

        if (x + met->boundingRect().width() < limites.left() - 100.0 ||
            y > limites.bottom() + 100.0) {
            removeItem(met);
            delete met;
            m_meteoritos.remove(i);
        }
    }
}

void Nivel2::actualizarPosicionHUD()
{
    if (!m_textoTiempo)
        return;

    const auto vs = views();
    if (vs.isEmpty())
        return;

    QGraphicsView *view = vs.first();
    if (!view)
        return;

    QPointF esquina = view->mapToScene(QPoint(10, 10));
    m_textoTiempo->setPos(esquina);
}

void Nivel2::actualizarCuentaAtras()
{
    if (m_nivelTerminado)
        return;

    if (m_tiempoRestante > 0)
        --m_tiempoRestante;

    if (m_textoTiempo) {
        m_textoTiempo->setPlainText(QStringLiteral("Tiempo: %1").arg(m_tiempoRestante));
        actualizarPosicionHUD();
    }

    if (m_tiempoRestante <= 0) {
        if (m_nivelTerminado)
            return;

        m_nivelTerminado = true;

        if (m_timerCuentaAtras) m_timerCuentaAtras->stop();
        if (m_timerLogica)      m_timerLogica->stop();
        if (m_timerMeteoritos)  m_timerMeteoritos->stop();
        if (m_nave)             m_nave->detener();

        mostrarVictoriaYEmitir();
    }
}

void Nivel2::manejarColisionNave()
{
    if (m_nivelTerminado)
        return;

    if (m_invulnerableInicio)
        return;

    m_nivelTerminado = true;

    if (m_timerCuentaAtras) m_timerCuentaAtras->stop();
    if (m_timerLogica)      m_timerLogica->stop();
    if (m_timerMeteoritos)  m_timerMeteoritos->stop();
    if (m_nave)             m_nave->detener();

    mostrarGameOverYEmitir();
}

void Nivel2::comprobarFinDeNivel()
{
    if (m_nivelTerminado || !m_nave)
        return;

    actualizarMeteoritos();

    QRectF rectNave  = m_nave->boundingRect();
    QPointF p        = m_nave->pos();

    qreal bordeDerechoNave = p.x() + rectNave.width();

    if (bordeDerechoNave >= sceneRect().right() - 30.0) {
        m_nivelTerminado = true;

        if (m_timerCuentaAtras) m_timerCuentaAtras->stop();
        if (m_timerLogica)      m_timerLogica->stop();
        if (m_timerMeteoritos)  m_timerMeteoritos->stop();
        if (m_nave)             m_nave->detener();

        mostrarVictoriaYEmitir();
    }
}

void Nivel2::mostrarVictoriaYEmitir()
{
    if (m_musicaNivel2) {
        m_musicaNivel2->stop();
    }

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

    const auto vs = views();
    QPointF centroScene;
    if (!vs.isEmpty()) {
        QGraphicsView *view = vs.first();
        centroScene = view->mapToScene(view->viewport()->rect().center());
    } else {
        centroScene = sceneRect().center();
    }

    m_mensajeOverlay = addPixmap(winPixmap);
    if (m_mensajeOverlay) {
        qreal x = centroScene.x()
        - m_mensajeOverlay->boundingRect().width() / 2.0;
        qreal y = centroScene.y()
                  - m_mensajeOverlay->boundingRect().height() / 2.0;
        m_mensajeOverlay->setPos(x, y);
        m_mensajeOverlay->setZValue(100);
    }

    QTimer::singleShot(3000, this, [this]() {
        if (m_mensajeOverlay) {
            removeItem(m_mensajeOverlay);
            delete m_mensajeOverlay;
            m_mensajeOverlay = nullptr;
        }
        emit nivelCompletado();
    });
}

void Nivel2::mostrarGameOverYEmitir()
{
    if (m_musicaNivel2) {
        m_musicaNivel2->stop();
    }

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

    const auto vs = views();
    QPointF centroScene;
    if (!vs.isEmpty()) {
        QGraphicsView *view = vs.first();
        centroScene = view->mapToScene(view->viewport()->rect().center());
    } else {
        centroScene = sceneRect().center();
    }

    m_mensajeOverlay = addPixmap(goPixmap);
    if (m_mensajeOverlay) {
        qreal x = centroScene.x()
        - m_mensajeOverlay->boundingRect().width() / 2.0;
        qreal y = centroScene.y()
                  - m_mensajeOverlay->boundingRect().height() / 2.0;
        m_mensajeOverlay->setPos(x, y);
        m_mensajeOverlay->setZValue(100);
    }

    QTimer::singleShot(3000, this, [this]() {
        if (m_mensajeOverlay) {
            removeItem(m_mensajeOverlay);
            delete m_mensajeOverlay;
            m_mensajeOverlay = nullptr;
        }
        emit nivelFallado();
    });
}
