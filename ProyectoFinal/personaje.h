#ifndef PERSONAJE_H
#define PERSONAJE_H
#include "qobject.h"
#include "QGraphicsPixmapItem"
class Personaje:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
    private:
        qreal x = 200;
        qreal y = 200;
        int spriteX = 0;
        int spriteY = 120;
        int spriteAncho = 60;
        int spriteAlto = 60;
        QPixmap sprite;
        QPixmap hojaSprites;
        int cont = 0;
    public:
        Personaje();
        void keyPressEvent(QKeyEvent *event) override;
        void movimiento(int dx, int dy);
        void setSprite(int dir);
};

class Protagonista:public Personaje{
    public:
        Protagonista() {
        // Puedes llamar al constructor base si es necesario.
        // Código adicional para el protagonista.
        }

    // Sobrescribir un método de Personaje (opcional)
        void keyPressEvent(QKeyEvent *event) override;
        // Comportamiento especial para el protagonista
  // Llamar al método base si es necesario
        // Añadir más acciones específicas aquí

};
#endif // PERSONAJE_H
