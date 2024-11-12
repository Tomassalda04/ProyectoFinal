#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QImage fondo("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/fondo.png");
    ui -> graphicsView -> setBackgroundBrush(fondo);
    scene -> setSceneRect(1920,1080,5000,200);
    ui->graphicsView->scale(0.36,1.2);

    QPixmap mov1("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento1.png");
    /*QPixmap mov2("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento2.png");
    QPixmap mov3("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento3.png");
    QPixmap mov4("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento4.png");
    QPixmap mov5("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento5.png");
    QPixmap mov6("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento6.png");
    */
    bart = new QGraphicsPixmapItem();
    scene -> addItem(bart);
    bart -> setPixmap(mov1);
    bart -> setScale(2.3);
    bart -> setPos(2000,1300);

    mov= new QTimer(this);
    mov->start(80);
    connect(mov,&QTimer::timeout,this,&MainWindow::runPlayer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* e){
    QPixmap mov1("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento1.png");
    QPixmap mov2("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento2.png");
    QPixmap mov3("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento3.png");
    QPixmap mov4("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento4.png");
    QPixmap mov5("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento5.png");
    QPixmap mov6("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento6.png");
    if(e->key()==Qt::Key_D){
        if(contador==1){
            bart -> setPixmap(mov1);
        }
        else if(contador==2){
            bart -> setPixmap(mov2);
        }
        else if(contador==3){
            bart -> setPixmap(mov3);
        }
        else if(contador==4){
            bart -> setPixmap(mov4);
        }
        else if(contador==5){
            bart -> setPixmap(mov5);
        }
        else if(contador==6){
            bart -> setPixmap(mov6);
        }
        bart -> setPos(bart->pos().x()+15,bart->pos().y());
    }
}

void MainWindow::runPlayer(){
    contador+=1;
    if(contador>6){
        contador=1;
    }

}
