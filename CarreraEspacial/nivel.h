// nivel.h
#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class Nivel : public QGraphicsScene
{
public:
    explicit Nivel(int numeroNivel, QObject *parent = nullptr);

    // Acceso al cohete y movimiento (para nivel 2)
    QGraphicsPixmapItem *cohete() const { return m_cohete; }
    void moverCohete(int dx, int dy);   // dx, dy: movimiento en X e Y

private:
    int m_numeroNivel;

    void crearNivel1();
    void crearNivel2();
    void crearNivel3();

    QGraphicsPixmapItem *m_cohete = nullptr;  // solo se usa en nivel 2
};

#endif // NIVEL_H


