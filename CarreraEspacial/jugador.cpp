#include "jugador.h"
#include "obstaculo.h"
#include <QVariant>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>

Jugador::Jugador(QGraphicsItem *parent)
    : Personaje(parent)
    , m_timerMovimiento(new QTimer(this))
    , m_velocidadBase(140.0)
    , m_direccionActual(Quieto)
    , m_frameActual(0)
    , m_tiempoAcumulado(0.0)
    , m_tiempoPorFrame(0.08)
{
    cargarSprites();

    if (!m_framesAbajo.isEmpty()) {
        setPixmap(m_framesAbajo.first());
    }

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    m_timerMovimiento->setInterval(16);
    connect(m_timerMovimiento, &QTimer::timeout,this, &Jugador::actualizarPaso);
    m_timerMovimiento->start();
}

void Jugador::cargarSprites()
{
    m_framesDer.clear();
    m_framesIzq.clear();
    m_framesArriba.clear();
    m_framesAbajo.clear();

    auto cargar = [](QVector<QPixmap> &dest,const QString &carpeta,int numFrames)
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
                qWarning() << "No se pudo cargar sprite de jugador:" << ruta;
            }
        }
    };

    const int framesPorDireccion = 8;
    cargar(m_framesDer,    "mov_der",      framesPorDireccion);
    cargar(m_framesIzq,    "mov_izq",      framesPorDireccion);
    cargar(m_framesAbajo,  "mov_abajo",    framesPorDireccion);
    cargar(m_framesArriba, "mov_adelante", framesPorDireccion);
}

void Jugador::actualizarPaso()
{
    if (!scene())
        return;
    if (!hasFocus())
        setFocus();
    qreal dt = m_timerMovimiento->interval() / 1000.0;
    QPointF posAnterior = pos();

    actualizarMovimiento(dt);

    corregirLimites();

    manejarColisiones(posAnterior);

    if (m_direccionActual != Quieto &&
        (!qFuzzyIsNull(m_velocidad.x()) || !qFuzzyIsNull(m_velocidad.y())))
    {
        actualizarAnimacion(dt);
    }
}

void Jugador::corregirLimites()
{
    if (!scene())
        return;

    QRectF limites    = scene()->sceneRect();
    QRectF rectLocal  = boundingRect();
    QPointF p         = pos();

    if (p.x() < limites.left())
        p.setX(limites.left());
    if (p.y() < limites.top())
        p.setY(limites.top());
    if (p.x() + rectLocal.width() > limites.right())
        p.setX(limites.right() - rectLocal.width());
    if (p.y() + rectLocal.height() > limites.bottom())
        p.setY(limites.bottom() - rectLocal.height());

    setPos(p);
}

void Jugador::manejarColisiones(const QPointF &posAnterior)
{
    const auto colisiones = collidingItems();
    for (QGraphicsItem *item : colisiones) {

        if (auto obstaculo = dynamic_cast<Obstaculo*>(item)) {

            const QVariant tipo = obstaculo->data(0);
            if (tipo.isValid() && tipo.toString() == "cohete") {
                emit coheteRecolectado(obstaculo);
            } else {
                setPos(posAnterior);
                return;
            }
        }
    }
}


void Jugador::actualizarAnimacion(qreal dt)
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
    case Quieto:    lista = nullptr;         break;
    }

    if (!lista || lista->isEmpty())
        return;

    m_frameActual = (m_frameActual + 1) % lista->size();
    setPixmap(lista->at(m_frameActual));
}

void Jugador::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    switch (event->key()) {
    case Qt::Key_A:
    case Qt::Key_Left:
        m_direccionActual = Izquierda;
        setVelocidadInicial(QPointF(-m_velocidadBase, 0.0));
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        m_direccionActual = Derecha;
        setVelocidadInicial(QPointF(m_velocidadBase, 0.0));
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        m_direccionActual = Arriba;
        setVelocidadInicial(QPointF(0.0, -m_velocidadBase));
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        m_direccionActual = Abajo;
        setVelocidadInicial(QPointF(0.0, m_velocidadBase));
        break;
    default:
        QGraphicsPixmapItem::keyPressEvent(event);
        return;
    }

    event->accept();
}

void Jugador::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    bool detener = false;

    switch (event->key()) {
    case Qt::Key_A:
    case Qt::Key_Left:
        detener = (m_direccionActual == Izquierda);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        detener = (m_direccionActual == Derecha);
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        detener = (m_direccionActual == Arriba);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        detener = (m_direccionActual == Abajo);
        break;
    default:
        QGraphicsPixmapItem::keyReleaseEvent(event);
        return;
    }

    if (detener) {
        setVelocidadInicial(QPointF(0.0, 0.0));
        m_direccionActual = Quieto;
        if (!m_framesAbajo.isEmpty())
            setPixmap(m_framesAbajo.first());
    }

    event->accept();
}
