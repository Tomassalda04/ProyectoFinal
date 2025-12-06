#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QBrush>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    if (nivel1) {
        delete nivel1;
        nivel1 = nullptr;
    }
    nivel1 = new Nivel(1, this);
    connect(nivel1, &Nivel::nivelCompletado, this, &MainWindow::onNivelCompletado);
    connect(nivel1, &Nivel::nivelFallado, this, &MainWindow::onNivelFallado);
    ui->graphicsView->setScene(nivel1);
    this->setFocus();
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
    this->setFocus();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (nivel3) {
        delete nivel3;
        nivel3 = nullptr;
    }
    nivel3 = new Nivel(3, this);
    connect(nivel3, &Nivel::nivelCompletado, this, &MainWindow::onNivelCompletado);
    connect(nivel3, &Nivel::nivelFallado,    this, &MainWindow::onNivelFallado);

    ui->graphicsView->setScene(nivel3);
    this->setFocus();
}


void MainWindow::volverAlMenu()
{
    ui->graphicsView->setScene(menuScene);
    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/Sprites/Fondo/fondo.png")));
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    this->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // --- NIVEL 2: mover cohete ---
    if (ui->graphicsView->scene() == nivel2 && nivel2->cohete()) {
        const int paso = 15;
        int dx = 0;
        int dy = 0;

        switch (event->key()) {
        case Qt::Key_Right: dx = +paso; break;
        case Qt::Key_Left:  dx = -paso; break;
        case Qt::Key_Up:    dy = -paso; break;
        case Qt::Key_Down:  dy = +paso; break;
        default: break;
        }

        if (dx != 0 || dy != 0) {
            nivel2->moverCohete(dx, dy);
            ui->graphicsView->centerOn(nivel2->cohete());
            event->accept();
            return;
        }
    }

    // --- NIVEL 3: mover mira y disparar ---
    if (ui->graphicsView->scene() == nivel3) {
        const int paso = 35; // velocidad de la mira (sube/baja esto)
        int dx = 0;
        int dy = 0;

        switch (event->key()) {
        case Qt::Key_Right:
        case Qt::Key_D:
            dx = +paso;
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            dx = -paso;
            break;
        case Qt::Key_Up:
        case Qt::Key_W:
            dy = -paso;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            dy = +paso;
            break;
        case Qt::Key_Space:
            if (nivel3) {
                nivel3->disparar();
                event->accept();
                return;
            }
            break;
        default:
            break;
        }

        if (dx != 0 || dy != 0) {
            nivel3->moverMira(dx, dy);
            event->accept();
            return;
        }
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::onNivelCompletado(int numeroNivel)
{
    if (numeroNivel == 1) {
        if (nivel2) {
            delete nivel2;
            nivel2 = nullptr;
        }
        nivel2 = new Nivel(2, this);
        connect(nivel2, &Nivel::nivelCompletado, this, &MainWindow::onNivelCompletado);
        connect(nivel2, &Nivel::nivelFallado, this, &MainWindow::onNivelFallado);
        ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setScene(nivel2);
    } else {
        volverAlMenu();
    }
}

void MainWindow::onNivelFallado(int)
{
    volverAlMenu();
}

