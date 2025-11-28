#include "enemigo.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>

Enemigo::Enemigo(QGraphicsItem *parent)
    : Personaje(parent)
    , m_timerMovimiento(new QTimer(this))
    , m_timerDireccion(new QTimer(this))
    , m_velocidadBase(80.0)
    , m_direccionActual(Abajo)
    , m_frameActual(0)
    , m_tiempoAcumulado(0.0)
    , m_tiempoPorFrame(0.08) // 80 ms por frame
{
    cargarSprites();

    // Sprite inicial (mirando hacia abajo)
    if (!m_framesAbajo.isEmpty()) {
        setPixmap(m_framesAbajo.first());
    }

    // Timer de movimiento + animación (~60 FPS)
    m_timerMovimiento->setInterval(16);
    connect(m_timerMovimiento, &QTimer::timeout,
            this, &Enemigo::actualizarPaso);
    m_timerMovimiento->start();

    // Timer para cambiar de dirección aleatoriamente
    m_timerDireccion->setInterval(800);
    connect(m_timerDireccion, &QTimer::timeout,
            this, &Enemigo::cambiarDireccionAleatoria);
    m_timerDireccion->start();

    cambiarDireccionAleatoria();
}

// ---------------------- CARGA DE SPRITES ----------------------

void Enemigo::cargarSprites()
{
    m_framesDer.clear();
    m_framesIzq.clear();
    m_framesArriba.clear();
    m_framesAbajo.clear();

    auto cargar = [](QVector<QPixmap> &dest,
                     const QString &carpeta,
                     int numFrames)
    {
        dest.reserve(numFrames);
        for (int i = 1; i <= numFrames; ++i) {
            QString ruta = QString(":/Sprites/SpritesNivel1/Sprites_mov_soldados_enemigos/%1/mov%2.png")
            .arg(carpeta)
                .arg(i);
            QPixmap frame(ruta);
            if (!frame.isNull()) {
                dest.push_back(frame);
            } else {
                qWarning() << "No se pudo cargar sprite:" << ruta;
            }
        }
    };

    const int framesPorDireccion = 8; // usamos 8 por dirección

    cargar(m_framesDer,    "mov_der",      framesPorDireccion);
    cargar(m_framesIzq,    "mov_izq",      framesPorDireccion);
    cargar(m_framesAbajo,  "mov_abajo",    framesPorDireccion);

    // Para ARRIBA uso tus sprites de la carpeta mov_adelante.
    // Si tu carpeta se llama distinto (p. ej. mov_arriba), cambia el string:
    cargar(m_framesArriba, "mov_adelante", framesPorDireccion);
}

// ---------------------- MOVIMIENTO + LÍMITES + ANIMACIÓN ----------------------

void Enemigo::actualizarPaso()
{
    if (!scene())
        return;

    qreal dt = m_timerMovimiento->interval() / 1000.0;

    // 1) Mover usando la física del Personaje
    actualizarMovimiento(dt);

    // 2) Mantener dentro de los límites y cambiar de dirección si choca con el borde
    corregirLimites();

    // Si está prácticamente parado, no animamos
    if (qFuzzyIsNull(m_velocidad.x()) && qFuzzyIsNull(m_velocidad.y())) {
        return;
    }

    // 3) Actualizar dirección según el vector velocidad
    if (qAbs(m_velocidad.x()) > qAbs(m_velocidad.y())) {
        // Se mueve más en horizontal
        m_direccionActual = (m_velocidad.x() > 0.0) ? Derecha : Izquierda;
    } else {
        // Se mueve más en vertical
        m_direccionActual = (m_velocidad.y() > 0.0) ? Abajo : Arriba;
    }

    // 4) Avanzar la animación
    actualizarAnimacion(dt);
}

void Enemigo::corregirLimites()
{
    if (!scene())
        return;

    QRectF limites    = scene()->sceneRect();
    QRectF rectLocal  = boundingRect();
    QPointF p         = pos();
    bool chocoBorde   = false;

    // Izquierda / arriba
    if (p.x() < limites.left()) {
        p.setX(limites.left());
        chocoBorde = true;
    }
    if (p.y() < limites.top()) {
        p.setY(limites.top());
        chocoBorde = true;
    }

    // Derecha / abajo
    if (p.x() + rectLocal.width() > limites.right()) {
        p.setX(limites.right() - rectLocal.width());
        chocoBorde = true;
    }
    if (p.y() + rectLocal.height() > limites.bottom()) {
        p.setY(limites.bottom() - rectLocal.height());
        chocoBorde = true;
    }

    if (chocoBorde) {
        setPos(p);
        // Cambiamos a una nueva dirección aleatoria para que no se quede pegado al borde
        cambiarDireccionAleatoria();
    }
}

void Enemigo::actualizarAnimacion(qreal dt)
{
    m_tiempoAcumulado += dt;
    if (m_tiempoAcumulado < m_tiempoPorFrame)
        return;

    m_tiempoAcumulado = 0.0;

    QVector<QPixmap> *lista = nullptr;
    switch (m_direccionActual) {
    case Derecha:   lista = &m_framesDer;    break;
    case Izquierda: lista = &m_framesIzq;    break;
    case Arriba:    lista = &m_framesArriba; break;
    case Abajo:     lista = &m_framesAbajo;  break;
    }

    if (!lista || lista->isEmpty())
        return;

    m_frameActual = (m_frameActual + 1) % lista->size();
    setPixmap(lista->at(m_frameActual));
}

// ---------------------- DIRECCIÓN ALEATORIA ----------------------

void Enemigo::cambiarDireccionAleatoria()
{
    int dir = QRandomGenerator::global()->bounded(4);

    switch (dir) {
    case 0: // Derecha
        m_direccionActual = Derecha;
        setVelocidadInicial(QPointF(m_velocidadBase, 0.0));
        break;
    case 1: // Izquierda
        m_direccionActual = Izquierda;
        setVelocidadInicial(QPointF(-m_velocidadBase, 0.0));
        break;
    case 2: // Arriba
        m_direccionActual = Arriba;
        setVelocidadInicial(QPointF(0.0, -m_velocidadBase));
        break;
    case 3: // Abajo
    default:
        m_direccionActual = Abajo;
        setVelocidadInicial(QPointF(0.0, m_velocidadBase));
        break;
    }

    // Sin aceleración, velocidad constante
    setAceleracion(QPointF(0.0, 0.0));

    // Reiniciamos el ciclo de animación
    m_frameActual = 0;
}
