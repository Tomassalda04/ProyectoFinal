#include "nivel.h"

NIVEL::NIVEL(QGraphicsScene *&Escena_, QString FileName, QGridLayout*& Layout_): Escena(Escena_), Fondo(new QGraphicsPixmapItem(FileName)), Layout(Layout_) {}
