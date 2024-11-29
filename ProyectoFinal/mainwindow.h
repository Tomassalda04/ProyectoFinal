#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    int posX = -70;
    ~MainWindow();


private:

    Ui::MainWindow *ui;
    QGraphicsPixmapItem* bart;
    QGraphicsScene *scene;
    void keyPressEvent(QKeyEvent *e);
    //void keyReleaseEvent(QKeyEvent *e);
    QTimer* mov;
    QTimer* agachado;
    QTimer* jump;
    int lugar_saltoX = 0;
    int lugar_saltoY = 0;
    int distancia = 0;
    bool tocar_arriba = false;
    bool subir = true;
    bool salto = true;
    bool colision=false;
    int contador=0,contadorAgachado=0;
    void runPlayer();
    void crouchingPlayer();
    void jumpPlayer();
};
#endif // MAINWINDOW_H
