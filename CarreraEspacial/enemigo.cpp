#include "enemigo.h"
#include "obstaculo.h"
#include "jugador.h"

#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>

Enemigo::Enemigo(TipoSprite tipo, QGraphicsItem *parent)
    : Personaje(parent)
    , m_timerMovimiento(new QTimer(this))
    , m_timerDireccion(new QTimer(this))
    , m_velocidadBase(80.0)
    , m_direccionActual(Abajo)
    , m_frameActual(0)
    , m_tiempoAcumulado(0.0)
    , m_tiempoPorFrame(0.08)
    , m_tipoSprite(tipo)
    , m_objetivo(nullptr)
    , m_enPersecucion(false)
    , m_radioDeteccion(200.0)
    , m_tiempoPersecucionMax(3.5)
    , m_tiempoPersecucionAcumulado(0.0)
    , m_movimientoDescendente(false)
    , m_yMaxDescendente(0.0)
    , m_velocidadDescendente(0.0)
{
    cargarSprites();

    if (!m_framesAbajo.isEmpty()) {
        setPixmap(m_framesAbajo.first());
    }

    // Timer de movimiento, ~60 FPS
    m_timerMovimiento->setInterval(16);
    connect(m_timerMovimiento, &QTimer::timeout,
            this,              &Enemigo::actualizarPaso);
    m_timerMovimiento->start();

    // Cambio aleatorio de dirección solo para los soldados
    m_timerDireccion->setInterval(1500);
    connect(m_timerDireccion, &QTimer::timeout,
            this,             &Enemigo::cambiarDireccionAleatoria);
    m_timerDireccion->start();

    cambiarDireccionAleatoria();
}

void Enemigo::setObjetivo(QGraphicsItem *objetivo)
{
    m_objetivo = objetivo;
}

void Enemigo::setRadioDeteccion(qreal radio)
{
    m_radioDeteccion = radio;
}

void Enemigo::configurarMovimientoDescendente(qreal yMax, qreal velocidadY)
{
    m_movimientoDescendente = true;
    m_yMaxDescendente       = yMax;
    m_velocidadDescendente  = velocidadY;

    // Solo se mueve hacia abajo, sin aceleración
    setVelocidadInicial(QPointF(0.0, m_velocidadDescendente));
    setAceleracion(QPointF(0.0, 0.0));
}

void Enemigo::cargarSprites()
{
    m_framesDer.clear();
    m_framesIzq.clear();
    m_framesArriba.clear();
    m_framesAbajo.clear();

    if (m_tipoSprite == Soldado) {
        // Soldados nivel 1
        auto cargar = [this](QVector<QPixmap> &dest,const QString &carpeta,int numFrames)
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
                    qWarning() << "No se pudo cargar sprite de soldado:" << ruta;
                }
            }
        };

        const int framesPorDireccion = 8;

        cargar(m_framesDer,    "mov_der",      framesPorDireccion);
        cargar(m_framesIzq,    "mov_izq",      framesPorDireccion);
        cargar(m_framesAbajo,  "mov_abajo",    framesPorDireccion);
        cargar(m_framesArriba, "mov_adelante", framesPorDireccion);
    }
    else if (m_tipoSprite == AlienNivel3) {
        // Alien nivel 3
        // Solo los 4 sprites de la fila 2 (movimiento hacia abajo)
        QVector<QString> rutas = {
            ":/Sprites/SpritesNivel3/marciano/fila2/mov1.png",
            ":/Sprites/SpritesNivel3/marciano/fila2/mov2.png",
            ":/Sprites/SpritesNivel3/marciano/fila2/mov3.png",
            ":/Sprites/SpritesNivel3/marciano/fila2/mov4.png"
        };

        for (const QString &ruta : rutas) {
            QPixmap frame(ruta);
            if (!frame.isNull()) {
                m_framesAbajo.push_back(frame);
            } else {
                qWarning() << "No se pudo cargar sprite de alien:" << ruta;
            }
        }

        // Reutilizamos la animación de abajo para todas las direcciones,
        // pero en el nivel 3 siempre forzaremos la dirección a Abajo.
        m_framesDer    = m_framesAbajo;
        m_framesIzq    = m_framesAbajo;
        m_framesArriba = m_framesAbajo;
    }
}

void Enemigo::actualizarPaso()
{
    if (!scene())
        return;

    const qreal dt = m_timerMovimiento->interval() / 1000.0;
    const QPointF posAnterior = pos();

    // modo solo descendente (aliens)
    if (m_movimientoDescendente) {
        // Avanza según la velocidad actual
        actualizarMovimiento(dt);

        QPointF p = pos();
        // Si se pasa del límite en y lo detenemos
        if (p.y() > m_yMaxDescendente) {
            p.setY(m_yMaxDescendente);
            setPos(p);
            setVelocidadInicial(QPointF(0.0, 0.0));
        }

        // Animación siempre mirando hacia abajo
        m_direccionActual = Abajo;
        actualizarAnimacion(dt);
        return; // No hay persecución ni colisiones complejas
    }

    // Comportamiento normal de los soldados
    actualizarEstadoPersecucion(dt);

    actualizarMovimiento(dt);

    corregirLimites();
    manejarColisiones(posAnterior);

    // Si no se mueve, no actualizamos animación
    if (qFuzzyIsNull(m_velocidad.x()) && qFuzzyIsNull(m_velocidad.y()))
        return;

    if (qAbs(m_velocidad.x()) > qAbs(m_velocidad.y())) {
        m_direccionActual = (m_velocidad.x() > 0.0) ? Derecha : Izquierda;
    } else {
        m_direccionActual = (m_velocidad.y() > 0.0) ? Abajo  : Arriba;
    }

    actualizarAnimacion(dt);
}

void Enemigo::actualizarEstadoPersecucion(qreal dt)
{
    if (!m_objetivo || m_movimientoDescendente)
        return;

    QPointF diff = m_objetivo->pos() - pos();
    const qreal distancia = qSqrt(diff.x() * diff.x() + diff.y() * diff.y());

    if (!m_enPersecucion) {
        if (distancia <= m_radioDeteccion) {
            m_enPersecucion = true;
            m_tiempoPersecucionAcumulado = 0.0;
        } else {
            return;
        }
    }

    m_tiempoPersecucionAcumulado += dt;
    if (m_tiempoPersecucionAcumulado > m_tiempoPersecucionMax) {
        m_enPersecucion = false;
        m_tiempoPersecucionAcumulado = 0.0;
        cambiarDireccionAleatoria();
        return;
    }

    if (distancia > 5.0) {
        QPointF dir = diff / distancia;
        setVelocidadInicial(QPointF(dir.x() * m_velocidadBase,
                                    dir.y() * m_velocidadBase));
        setAceleracion(QPointF(0.0, 0.0));
    }
}

void Enemigo::corregirLimites()
{
    if (!scene())
        return;

    QRectF limites   = scene()->sceneRect();
    QRectF rectLocal = boundingRect();
    QPointF p        = pos();
    bool chocoBorde  = false;

    if (p.x() < limites.left()) {
        p.setX(limites.left());
        chocoBorde = true;
    }
    if (p.y() < limites.top()) {
        p.setY(limites.top());
        chocoBorde = true;
    }
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
        cambiarDireccionAleatoria();
    }
}

void Enemigo::manejarColisiones(const QPointF &posAnterior)
{
    if (!scene())
        return;

    const auto colisiones = collidingItems();
    for (QGraphicsItem *item : colisiones) {
        if (dynamic_cast<Obstaculo*>(item)) {
            Direccion dirPrev = m_direccionActual;

            setPos(posAnterior);

            QPointF p = pos();
            switch (dirPrev) {
            case Derecha:   p.rx() -= 2.0; break;
            case Izquierda: p.rx() += 2.0; break;
            case Arriba:    p.ry() += 2.0; break;
            case Abajo:     p.ry() -= 2.0; break;
            }

            setPos(p);
            corregirLimites();

            cambiarDireccionAleatoria();
            break;
        }
        else if (dynamic_cast<Jugador*>(item)) {
            emit jugadorAlcanzado();
            break;
        }
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

void Enemigo::cambiarDireccionAleatoria()
{
    if (m_movimientoDescendente)
        return;

    if (!scene())
        return;

    int dir = QRandomGenerator::global()->bounded(4);

    switch (dir) {
    case 0:
        m_direccionActual = Derecha;
        setVelocidadInicial(QPointF(m_velocidadBase, 0.0));
        break;
    case 1:
        m_direccionActual = Izquierda;
        setVelocidadInicial(QPointF(-m_velocidadBase, 0.0));
        break;
    case 2:
        m_direccionActual = Arriba;
        setVelocidadInicial(QPointF(0.0, -m_velocidadBase));
        break;
    case 3:
    default:
        m_direccionActual = Abajo;
        setVelocidadInicial(QPointF(0.0, m_velocidadBase));
        break;
    }

    setAceleracion(QPointF(0.0, 0.0));
    m_frameActual = 0;
}
