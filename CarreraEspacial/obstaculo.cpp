#include "obstaculo.h"
#include <QPixmap>
#include <QDebug>

Obstaculo::Obstaculo(const QString &rutaSprite, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    QPixmap sprite(rutaSprite);
    if (sprite.isNull()) {
        qWarning() << "No se pudo cargar sprite de obstáculo:" << rutaSprite;
    }
    setPixmap(sprite);

    setZValue(1);
}
