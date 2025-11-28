#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QString>

class Obstaculo : public QGraphicsPixmapItem
{
public:
    explicit Obstaculo(const QString &rutaSprite,
                       QGraphicsItem *parent = nullptr);
};

#endif // OBSTACULO_H
