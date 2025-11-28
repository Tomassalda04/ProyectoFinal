#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QString>

class Obstaculo : public QGraphicsPixmapItem
{
public:
    // rutaSprite = la ruta que tengas en el .qrc, por ejemplo
    // ":/Sprites/SpritesNivel1/Obstaculos/roca1.png"
    explicit Obstaculo(const QString &rutaSprite,
                       QGraphicsItem *parent = nullptr);
};

#endif // OBSTACULO_H
