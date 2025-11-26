// nivel.h
#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class Nivel : public QGraphicsScene
{
public:
    explicit Nivel(int numeroNivel, QObject *parent = nullptr);


private:
    int m_numeroNivel;

    void crearNivel1();
    void crearNivel2();
    void crearNivel3();
};

#endif // NIVEL_H
