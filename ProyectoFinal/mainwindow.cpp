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
    scene -> setSceneRect(1920,1080,3000,200);
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
    agachado= new QTimer(this);
    mov->start(100);
    agachado->start(80);
    connect(mov,&QTimer::timeout,this,&MainWindow::runPlayer);
    connect(agachado,&QTimer::timeout,this,&MainWindow::crouchingPlayer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* e){
    QPointF currentPos= scene-> sceneRect().topLeft();
    QPointF subjectPos= bart->pos();
    QPixmap mov1("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento1.png"); QPixmap mov2("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento2.png");QPixmap mov3("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento3.png"); QPixmap mov4("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento4.png");QPixmap mov5("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento5.png");QPixmap mov6("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Movimiento/Movimiento6.png");
    QPixmap crouch1("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado1.png");QPixmap crouch2("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado2.png");QPixmap crouch3("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado3.png");QPixmap crouch4("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado4.png");QPixmap crouch5("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado5.png");QPixmap crouch6("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado6.png");QPixmap crouch7("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Agachado/agachado7.png");
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
        currentPos.setX(currentPos.x()+15);
        scene->setSceneRect(QRectF(currentPos,scene->sceneRect().size()));
    }
    else if(e->key()==Qt::Key_S){
        if(contadorAgachado==1){
            bart -> setPixmap(crouch1);
        }
        else if(contadorAgachado==2){
            bart -> setPixmap(crouch2);
        }
        else if(contadorAgachado==3){
            bart -> setPixmap(crouch3);
        }
        else if(contadorAgachado==4){
            bart -> setPixmap(crouch4);
        }
        else if(contadorAgachado==5){
            bart -> setPixmap(crouch5);
        }
        else if(contadorAgachado==6){
            bart -> setPixmap(crouch6);
        }
        else if(contadorAgachado==7){
            bart -> setPixmap(crouch7);
        }
        bart -> setPos(bart->pos().x()+15,bart->pos().y());
        currentPos.setX(currentPos.x()+15);
        scene->setSceneRect(QRectF(currentPos,scene->sceneRect().size()));
    }
    else if(e->key()==Qt::Key_W){
        lugar_saltoX = pos().x();
        salto = true;
        subir = true;
        lugar_saltoY = pos().y();
        if (!jump->isActive()) {
            jump->start(18);
        }
    }

}
void MainWindow::runPlayer(){
    contador+=1;
    if(contador>7){
        contador=0;
        posX += 7;
        distancia -= 5;
    }
}
void MainWindow::crouchingPlayer(){
    contadorAgachado+=1;
    if(contadorAgachado>8){
        contadorAgachado=0;
    }
}
void MainWindow::jumpPlayer(){
    QPixmap jump1("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Salto/salto1.png");QPixmap jump2("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Salto/salto2.png");QPixmap jump3("C:/Users/stoma/OneDrive/Escritorio/Proyecto Final/ProyectoFinal/Sprites/Sprite Salto/salto3.png");
    if (subir) {
        bart->setPixmap(jump1);
        if (pos().y() <= 850) {
            bart->setPos(pos().x() + 7, pos().y() - 9);
            posX += 7;
            distancia += 5;
            if (distancia == 90) {
                subir = false;
            }
        }
    } else{
        if(colision == false){
            bart->setPixmap(jump2);
            bart->setPos(pos().x() + 7, pos().y() + 9);
            posX += 7;
            distancia -= 5;
            if (distancia == 0) {
                bart->setPixmap(jump3);
                subir = true;
                salto = false;
                jump->stop();
            }
        }
        else{
            //qDebug()<<pos().y();
            bart->setPixmap(jump1);
            jump-> stop();
            //subir = true;
        }
    }

}

