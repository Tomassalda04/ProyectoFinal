#ifndef NIVEL_H
#define NIVEL_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "QObject"
#include <QLayout>
#include <QGraphicsScene>
class NIVEL: public QObject
{
    Q_OBJECT
    protected:
        QGraphicsPixmapItem* Fondo;
        QGraphicsScene* Escena;
        QGridLayout* Layout;
    public:
        NIVEL(QGraphicsScene*& Escena_, QString FileName, QGridLayout*& Layout_);
};

#endif // NIVEL_H
