#include "fscene.h"
#include "tscene.h"
#include <QDebug>
#include <cmath>
#include <QKeyEvent>
#include "mainwindow.h"
Fscene::Fscene(MainWindow *parent): mainWindow(parent)



{
    initializeScene();
    QImage imagen_fondo(":/fondo.png");
    QBrush fondo_escenario(imagen_fondo);
    setBackgroundBrush(fondo_escenario);
}

void Fscene::initializeScene() {
    setSceneRect(0, 0, 1920, 1080);
    /*personaje1 = new personaje();
    addItem(personaje1);
    personaje1->setFocus();*/
}
