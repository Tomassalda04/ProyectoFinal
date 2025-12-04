// nivel.cpp
#include "nivel.h"
#include "naveespacial.h"
#include "obstaculo.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRandomGenerator>
#include <QFont>
#include <QGraphicsView>
#include <QStringList>
#include <QPainter>
#include <QtMath>      // qCos, qSin, qDegreesToRadians
#include <QScrollBar>  // para engancharse al scroll horizontal

static const int  VIEW_W = 1024;
static const int  VIEW_H = 572;

// Parámetros para que el cohete dure ~40 s en el nivel 2
static const qreal TIEMPO_OBJETIVO_NIVEL2   = 40.0;   // segundos
static const qreal VELOCIDAD_BASE_NAVE_N2   = 350.0;  // px/s
static const int   MARGEN_EXTRA_ESCENA_N2   = 600;    // margen extra para que no acabe justo

// Física de los meteoritos (trayectoria parabólica)
static const qreal GRAVEDAD_METEORITO       = 600.0;  // px/s^2 hacia abajo

Nivel::Nivel(int numeroNivel, QObject *parent)
    : QGraphicsScene(parent)
    , m_numeroNivel(numeroNivel)
    , m_nave(nullptr)
    , m_timerCuentaAtras(nullptr)
    , m_timerLogica(nullptr)
    , m_timerMeteoritos(nullptr)
    , m_tiempoRestante(0)
    , m_textoTiempo(nullptr)
    , m_nivelTerminado(false)
    , m_hudConectado(false)
{
    switch (m_numeroNivel) {
    case 1: crearNivel1(); break;
    case 2: crearNivel2(); break;
    case 3: crearNivel3(); break;
    default: crearNivel1(); break;
    }
}

// ───────── NIVEL 1 (simple) ─────────
void Nivel::crearNivel1()
{
    QPixmap fondo(":/Sprites/SpritesNivel1/FondoNivel1.png");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(
            VIEW_W, VIEW_H,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);
        addPixmap(fondoEscalado)->setPos(0, 0);
    }
}

// ───────── NIVEL 2: fondo largo + nave + obstáculos aleatorios + meteoritos ─────────
void Nivel::crearNivel2()
{
    QPixmap fondo(":/Sprites/SpritesNivel2/FondoNivel2.png");

    // Escalamos el fondo a la altura de la vista
    QPixmap fondoEscalado = fondo.isNull()
                                ? QPixmap(VIEW_W, VIEW_H)
                                : fondo.scaledToHeight(VIEW_H, Qt::SmoothTransformation);

    if (fondoEscalado.isNull()) {
        fondoEscalado = QPixmap(VIEW_W, VIEW_H);
        fondoEscalado.fill(Qt::black);
    }

    // 1) Calcular ancho deseado para que el nivel dure ~40 s
    const qreal distObjetivo = VELOCIDAD_BASE_NAVE_N2 * TIEMPO_OBJETIVO_NIVEL2;
    int anchoDeseado = static_cast<int>(distObjetivo) + MARGEN_EXTRA_ESCENA_N2;

    // Que nunca sea menor que la pantalla
    if (anchoDeseado < VIEW_W)
        anchoDeseado = VIEW_W;

    // Si el fondo original es más ancho, nos quedamos con ese
    if (anchoDeseado < fondoEscalado.width())
        anchoDeseado = fondoEscalado.width();

    // Si hace falta, generamos un fondo más largo repitiendo el original
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

    // La escena mide lo que mida el fondo → scroll horizontal largo
    setSceneRect(0, 0, fondoEscalado.width(), fondoEscalado.height());
    addPixmap(fondoEscalado)->setPos(0, 0);

    // 2) NAVE ESPACIAL
    QPixmap spriteCohete(":/Sprites/SpritesNivel2/Cohete/nave1.png");
    if (spriteCohete.isNull()) {
        spriteCohete = QPixmap(40, 80);
        spriteCohete.fill(Qt::red);
    }

    spriteCohete = spriteCohete.scaled(
        80,
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

    // Velocidades base (coinciden con el cálculo del ancho)
    m_nave->setVelocidadBase(VELOCIDAD_BASE_NAVE_N2);
    m_nave->setVelocidadVertical(420.0);

    connect(m_nave, &NaveEspacial::colisionConObstaculo,
            this, &Nivel::manejarColisionNave);

    // 3) OBSTÁCULOS ALEATORIOS
    m_obstaculos.clear();
    generarObstaculosAleatorios();

    // 4) Texto del tiempo (HUD)
    m_textoTiempo = addText("Tiempo: 40");
    m_textoTiempo->setDefaultTextColor(Qt::white);
    m_textoTiempo->setFont(QFont("Arial", 18, QFont::Bold));
    m_textoTiempo->setZValue(10);

    // Lo posicionamos donde toque según la vista actual (si ya hay una vista)
    actualizarPosicionHUD();
}

// ───────── NIVEL 3 (simple) ─────────
void Nivel::crearNivel3()
{
    QPixmap fondo(":/Sprites/SpritesNivel3/FondoNivel3.png");

    setSceneRect(0, 0, VIEW_W, VIEW_H);

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(
            VIEW_W, VIEW_H,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);
        addPixmap(fondoEscalado)->setPos(0, 0);
    }
}

// ───────── INICIAR NIVEL (para nivel 2) ─────────
void Nivel::iniciar()
{
    if (m_numeroNivel != 2)
        return;

    m_nivelTerminado = false;
    m_tiempoRestante = 40;

    // Eliminar meteoritos anteriores (si los hubiera)
    for (int i = m_meteoritos.size() - 1; i >= 0; --i) {
        Obstaculo *met = m_meteoritos[i].item;
        if (met) {
            removeItem(met);
            delete met;
        }
    }
    m_meteoritos.clear();

    // Timer cuenta atrás
    if (!m_timerCuentaAtras) {
        m_timerCuentaAtras = new QTimer(this);
        m_timerCuentaAtras->setInterval(1000);
        connect(m_timerCuentaAtras, &QTimer::timeout,
                this, &Nivel::actualizarCuentaAtras);
    }
    m_timerCuentaAtras->start();

    // Timer lógica principal (colisiones, fin de nivel, etc.)
    if (!m_timerLogica) {
        m_timerLogica = new QTimer(this);
        m_timerLogica->setInterval(16); // ~60 FPS
        connect(m_timerLogica, &QTimer::timeout,
                this, &Nivel::comprobarFinDeNivel);
    }
    m_timerLogica->start();

    // Timer meteoritos: va creando meteoritos cada cierto tiempo
    if (!m_timerMeteoritos) {
        m_timerMeteoritos = new QTimer(this);
        m_timerMeteoritos->setInterval(1500); // cada 700 ms cae uno nuevo
        connect(m_timerMeteoritos, &QTimer::timeout,
                this, &Nivel::crearMeteorito);
    }
    m_timerMeteoritos->start();

    if (m_textoTiempo) {
        m_textoTiempo->setPlainText(QStringLiteral("Tiempo: %1").arg(m_tiempoRestante));
        actualizarPosicionHUD();
    }

    // Conectar HUD al scroll horizontal de la vista (solo una vez)
    if (!m_hudConectado) {
        const auto vs = views();
        if (!vs.isEmpty()) {
            QGraphicsView *view = vs.first();
            if (view && view->horizontalScrollBar()) {
                connect(view->horizontalScrollBar(), &QScrollBar::valueChanged,
                        this, [this](int) {
                            actualizarPosicionHUD();
                        });
                m_hudConectado = true;
            }
        }
    }

    if (m_nave) {
        // Resetear flags de teclado
        m_nave->setArribaPresionado(false);
        m_nave->setAbajoPresionado(false);
        m_nave->setDerechaPresionado(false);

        // Recolocar la nave en la posición inicial
        QRectF limites = sceneRect();
        qreal xInicial = limites.left() + 100.0;
        qreal yInicial = limites.height() / 2.0
                         - m_nave->boundingRect().height() / 2.0;
        m_nave->setPos(xInicial, yInicial);

        // Reactivar el movimiento por si se había detenido
        m_nave->iniciarMovimiento();
    }
}

// ───────── Generación aleatoria de obstáculos de nivel 2 ─────────
void Nivel::generarObstaculosAleatorios()
{
    if (!m_nave)
        return;

    // Rutas de sprites
    QStringList spritesObstaculos = {
        ":/Sprites/SpritesNivel2/Obstaculos/Obs1.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs2.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs3.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs4.png",
        ":/Sprites/SpritesNivel2/Obstaculos/Obs5.png"
    };

    QStringList spritesCohetes = {
        ":/Sprites/SpritesNivel2/Cohete/Coh1.png"
    };

    QRectF limites = sceneRect();

    // Que empiecen pronto y haya muchos
    const qreal xInicio = m_nave->pos().x() + 150.0;
    const qreal xFinal  = limites.right() - 200.0;
    if (xInicio >= xFinal)
        return;

    // Usamos TODA la altura de la escena (sin franjas seguras)
    const qreal yMin = limites.top();
    const qreal yMax = limites.bottom();

    const qreal DISTANCIA_MINIMA = 80.0;  // separación mínima entre obstáculos
    const int   NUM_OBSTACULOS   = 70;    // cantidad

    // Rangos ENTEROS para QRandomGenerator::bounded(int,int)
    const int xMinInt = static_cast<int>(xInicio);
    const int xMaxInt = static_cast<int>(xFinal);
    const int yMinInt = static_cast<int>(yMin);
    const int yMaxInt = static_cast<int>(yMax);

    if (xMaxInt <= xMinInt || yMaxInt <= yMinInt)
        return;

    for (int i = 0; i < NUM_OBSTACULOS; ++i) {

        bool esCohete = (QRandomGenerator::global()->bounded(4) == 0); // 1 de cada 4

        QString rutaSprite;
        if (esCohete) {
            rutaSprite = spritesCohetes.at(0);
        } else {
            int idx = QRandomGenerator::global()->bounded(spritesObstaculos.size());
            rutaSprite = spritesObstaculos.at(idx);
        }

        Obstaculo *ob = new Obstaculo(rutaSprite);
        QRectF rectLocal = ob->boundingRect();

        bool colocado = false;

        for (int intento = 0; intento < 50 && !colocado; ++intento) {

            int xMaxForObst = xMaxInt - static_cast<int>(rectLocal.width());
            int yMaxForObst = yMaxInt - static_cast<int>(rectLocal.height());

            if (xMaxForObst <= xMinInt || yMaxForObst <= yMinInt)
                break;

            int randXInt = QRandomGenerator::global()->bounded(xMinInt, xMaxForObst);
            int randYInt = QRandomGenerator::global()->bounded(yMinInt, yMaxForObst);

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
    }
}

// ───────── Meteoritos: se lanzan en parábola desde la esquina superior derecha visible ─────────
void Nivel::crearMeteorito()
{
    if (m_numeroNivel != 2 || m_nivelTerminado)
        return;

    // Si por algún motivo no hay vista, no hacemos nada
    if (views().isEmpty())
        return;

    QGraphicsView *view = views().first();

    // Rectángulo visible actualmente en la escena
    QRect visibleViewport = view->viewport()->rect();
    QRectF visibleSceneRect = view->mapToScene(visibleViewport).boundingRect();

    const QString rutaSpriteMeteorito = ":/Sprites/SpritesNivel2/Obstaculos/Obs7.png";

    Obstaculo *meteorito = new Obstaculo(rutaSpriteMeteorito);
    QRectF rectLocal = meteorito->boundingRect();

    // Zona de lanzamiento: franja en la parte superior DERECHA de lo que ve la cámara
    const qreal ANCHO_ZONA_DERECHA = 400.0;  // ancho de la zona desde la derecha hacia la izquierda

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

    // Un poco por encima de lo visible
    qreal yBase   = visibleSceneRect.top() - rectLocal.height() - 20.0;
    qreal offsetY = QRandomGenerator::global()->bounded(60.0); // [0, 60)
    qreal y       = yBase + offsetY;

    meteorito->setPos(x, y);
    meteorito->setZValue(2); // por encima del fondo
    addItem(meteorito);

    // --- Velocidades para la trayectoria parabólica (más lento) ---
    MeteoritoData data;
    data.item = meteorito;

    // Ángulo respecto a la horizontal, hacia abajo y a la izquierda
    const qreal angleDegMin = 2.0;
    const qreal angleDegMax = 5.0;

    // Módulo de la velocidad (más bajo)
    const qreal speedMin = 200.0;
    const qreal speedMax = 200.0;

    qreal angleDeg = angleDegMin +
                     QRandomGenerator::global()->bounded(angleDegMax - angleDegMin);
    qreal angleRad = qDegreesToRadians(angleDeg);

    qreal speed = speedMin +
                  QRandomGenerator::global()->bounded(speedMax - speedMin);

    // Sistema de coordenadas Qt: +x derecha, +y abajo
    // Queremos disparar hacia la IZQUIERDA y ABAJO
    qreal velX = -speed * qCos(angleRad); // izquierda → negativo
    qreal velY =  speed * qSin(angleRad); // abajo → positivo

    data.velocidad   = QPointF(velX, velY);
    data.aceleracion = QPointF(0.0, GRAVEDAD_METEORITO); // gravedad hacia abajo

    m_meteoritos.push_back(data);
}

// Actualiza la física de todos los meteoritos
void Nivel::actualizarMeteoritos()
{
    if (m_meteoritos.isEmpty())
        return;

    // dt según el timer de lógica (16 ms ≈ 0.016 s)
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

        // Integración simple:
        // v = v + a * dt
        data.velocidad.setX(data.velocidad.x() + data.aceleracion.x() * dt);
        data.velocidad.setY(data.velocidad.y() + data.aceleracion.y() * dt);

        // p = p + v * dt
        qreal dx = data.velocidad.x() * dt;
        qreal dy = data.velocidad.y() * dt;
        met->moveBy(dx, dy);

        const qreal x = met->x();
        const qreal y = met->y();

        // Si ya salió por la izquierda o por abajo, lo eliminamos
        if (x + met->boundingRect().width() < limites.left() - 100.0 ||
            y > limites.bottom() + 100.0) {
            removeItem(met);
            delete met;
            m_meteoritos.remove(i);
        }
    }
}

// ───────── HUD: mantener contador en esquina superior izquierda de la vista ─────────
void Nivel::actualizarPosicionHUD()
{
    if (!m_textoTiempo)
        return;

    const auto vs = views();
    if (vs.isEmpty())
        return;

    QGraphicsView *view = vs.first();
    if (!view)
        return;

    // (10,10) en coordenadas de la vista → lo llevamos a coordenadas de la escena
    QPointF esquina = view->mapToScene(QPoint(10, 10));
    m_textoTiempo->setPos(esquina);
}

// ───────── Cuenta atrás 40 s ─────────
void Nivel::actualizarCuentaAtras()
{
    if (m_numeroNivel != 2 || m_nivelTerminado)
        return;

    if (m_tiempoRestante > 0)
        --m_tiempoRestante;

    if (m_textoTiempo) {
        m_textoTiempo->setPlainText(QStringLiteral("Tiempo: %1").arg(m_tiempoRestante));
        actualizarPosicionHUD();
    }

    if (m_tiempoRestante <= 0) {
        m_nivelTerminado = true;

        if (m_timerCuentaAtras) m_timerCuentaAtras->stop();
        if (m_timerLogica)      m_timerLogica->stop();
        if (m_timerMeteoritos)  m_timerMeteoritos->stop();
        if (m_nave)             m_nave->detener();

        emit nivelFallado(m_numeroNivel);
    }
}

// ───────── Colisión = pierdes ─────────
void Nivel::manejarColisionNave()
{
    if (m_numeroNivel != 2 || m_nivelTerminado)
        return;

    m_nivelTerminado = true;

    if (m_timerCuentaAtras) m_timerCuentaAtras->stop();
    if (m_timerLogica)      m_timerLogica->stop();
    if (m_timerMeteoritos)  m_timerMeteoritos->stop();
    if (m_nave)             m_nave->detener();

    emit nivelFallado(m_numeroNivel);
}

// ───────── Llegar al final = ganas ─────────
void Nivel::comprobarFinDeNivel()
{
    if (m_numeroNivel != 2 || m_nivelTerminado || !m_nave)
        return;

    // Mover meteoritos en cada tick de lógica
    actualizarMeteoritos();

    QRectF limites   = sceneRect();
    QRectF rectNave  = m_nave->boundingRect();
    QPointF p        = m_nave->pos();

    qreal bordeDerechoNave = p.x() + rectNave.width();

    // Si llega casi al borde derecho de la escena, ganas
    if (bordeDerechoNave >= limites.right() - 30.0) {
        m_nivelTerminado = true;

        if (m_timerCuentaAtras) m_timerCuentaAtras->stop();
        if (m_timerLogica)      m_timerLogica->stop();
        if (m_timerMeteoritos)  m_timerMeteoritos->stop();
        if (m_nave)             m_nave->detener();

        emit nivelCompletado(m_numeroNivel);
    }
}

