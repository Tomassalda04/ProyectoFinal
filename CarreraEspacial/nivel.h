// nivel.h
#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "personaje.h"

class Nivel : public QGraphicsScene
{
public:
    explicit Nivel(int numeroNivel, QObject *parent = nullptr);

private:
    int m_numeroNivel;

    void crearNivel1();
    void crearNivel2();
    void crearNivel3();

    // Personaje de prueba que se mueve con la ecuación de movimiento
    Personaje *m_personajePrueba = nullptr;
};

#endif // NIVEL_H
