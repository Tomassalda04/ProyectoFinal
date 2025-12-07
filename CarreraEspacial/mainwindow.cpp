#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "nivel.h"
#include "nivel2.h"
#include "naveespacial.h"

#include <QImage>
#include <QBrush>
#include <QKeyEvent>
#include <QCoreApplication>

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

    nivel1 = nullptr;
    nivel2 = nullptr;
    nivel3 = nullptr;
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

    connect(nivel1, &Nivel::nivelCompletado,
            this,   &MainWindow::onNivelCompletado);
    connect(nivel1, &Nivel::nivelFallado,
            this,   &MainWindow::onNivelFallado);

    ui->graphicsView->setScene(nivel1);
    this->setFocus();
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (nivel2) {
        delete nivel2;
        nivel2 = nullptr;
    }
    nivel2 = new Nivel2(this);

    connect(nivel2, &Nivel2::nivelCompletado,
            this,   &MainWindow::onNivel2Completado);
    connect(nivel2, &Nivel2::nivelFallado,
            this,   &MainWindow::onNivel2Fallado);

    ui->graphicsView->setScene(nivel2);
    this->setFocus();

    nivel2->iniciar();
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

    connect(nivel3, &Nivel::nivelCompletado,
            this,   &MainWindow::onNivelCompletado);
    connect(nivel3, &Nivel::nivelFallado,
            this,   &MainWindow::onNivelFallado);

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
    if (ui->graphicsView->scene() == nivel1 && nivel1) {
        QCoreApplication::sendEvent(nivel1, event);
        if (event->isAccepted())
            return;
    }

    if (ui->graphicsView->scene() == nivel2 && nivel2 && nivel2->nave()) {
        NaveEspacial *nave = nivel2->nave();

        switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            nave->setArribaPresionado(true);
            event->accept();
            return;
        case Qt::Key_S:
        case Qt::Key_Down:
            nave->setAbajoPresionado(true);
            event->accept();
            return;
        case Qt::Key_D:
        case Qt::Key_Right:
            nave->setDerechaPresionado(true);
            event->accept();
            return;
        default:
            break;
        }
    }

    if (nivel3 && ui->graphicsView->scene() == nivel3) {
        const int paso = 35;
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

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (ui->graphicsView->scene() == nivel1 && nivel1) {
        QCoreApplication::sendEvent(nivel1, event);
        if (event->isAccepted())
            return;
    }

    if (ui->graphicsView->scene() == nivel2 && nivel2 && nivel2->nave()) {
        NaveEspacial *nave = nivel2->nave();

        switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            nave->setArribaPresionado(false);
            event->accept();
            return;
        case Qt::Key_S:
        case Qt::Key_Down:
            nave->setAbajoPresionado(false);
            event->accept();
            return;
        case Qt::Key_D:
        case Qt::Key_Right:
            nave->setDerechaPresionado(false);
            event->accept();
            return;
        default:
            break;
        }
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::onNivelCompletado(int numeroNivel)
{
    if (numeroNivel == 1) {
        if (nivel2) {
            delete nivel2;
            nivel2 = nullptr;
        }
        nivel2 = new Nivel2(this);

        connect(nivel2, &Nivel2::nivelCompletado,
                this,   &MainWindow::onNivel2Completado);
        connect(nivel2, &Nivel2::nivelFallado,
                this,   &MainWindow::onNivel2Fallado);

        ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setScene(nivel2);
        this->setFocus();

        nivel2->iniciar();
    } else {
        volverAlMenu();
    }
}

void MainWindow::onNivelFallado(int)
{
    volverAlMenu();
}

void MainWindow::onNivel2Completado()
{
    if (nivel3) {
        delete nivel3;
        nivel3 = nullptr;
    }
    nivel3 = new Nivel(3, this);

    connect(nivel3, &Nivel::nivelCompletado,
            this,   &MainWindow::onNivelCompletado);
    connect(nivel3, &Nivel::nivelFallado,
            this,   &MainWindow::onNivelFallado);

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(nivel3);
    this->setFocus();
}

void MainWindow::onNivel2Fallado()
{
    volverAlMenu();
}


