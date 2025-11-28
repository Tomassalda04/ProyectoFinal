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

    // Si quieres asegurarte de que siempre quede por delante del fondo:
    setZValue(1);
}
