#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    menuScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(menuScene);

    QImage fondoMenu(":/Sprites/Fondo/fondo.png");
    ui->graphicsView->setBackgroundBrush(QBrush(fondoMenu));
    menuScene->setSceneRect(0, 0, 1024, 572);

    nivel1 = new Nivel(1, this);
    nivel2 = new Nivel(2, this);
    nivel3 = new Nivel(3, this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel2);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel3);
}
