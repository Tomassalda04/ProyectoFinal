#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    menuScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(menuScene);

    QImage fondoMenu(":/Sprites/Fondo/fondo.png");
    ui->graphicsView->setBackgroundBrush(QBrush(fondoMenu));
    menuScene->setSceneRect(270, 0, 1024, 572);

    nivel1 = new Nivel(1, this);
    nivel2 = new Nivel(2, this);
    nivel3 = new Nivel(3, this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// NIVEL 1
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush); // usamos el fondo del nivel
    ui->graphicsView->setScene(nivel1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setScene(nivel2);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setScene(nivel3);
}

