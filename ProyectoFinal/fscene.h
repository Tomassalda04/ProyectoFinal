#ifndef FSCENE_H
#define FSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "qlabel.h"



class MainWindow;

class Fscene : public QGraphicsScene
{
    Q_OBJECT
public:
    Fscene(MainWindow *parent = nullptr);

protected:
    //void keyPressEvent(QKeyEvent *event) override;

private slots:
    /*void onMoveBackground(int dx);
    void crear_obs();
    void actualizar_obstaculos();
    void verificar_colision();*/

private:
    void showPauseMenu();
    void removeProgressBar();
    //personaje *personaje1;
    QTimer *obs_timer;
    QTimer *coli;
    QTimer *timer;
    QGraphicsPixmapItem *obs;
    void initializeScene();
    MainWindow *mainWindow;

};

#endif // FSCENE_H
