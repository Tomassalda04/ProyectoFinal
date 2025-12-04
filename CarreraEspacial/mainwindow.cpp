#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "naveespacial.h"
#include <QImage>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Escena del menú
    menuScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(menuScene);

    QImage fondoMenu(":/Sprites/Fondo/fondo.png");
    ui->graphicsView->setBackgroundBrush(QBrush(fondoMenu));
    menuScene->setSceneRect(0, 0, 1024, 572);

    // Niveles
    nivel1 = new Nivel(1, this);
    nivel2 = new Nivel(2, this);
    nivel3 = new Nivel(3, this);

    // Reacciones del nivel 2
    connect(nivel2, &Nivel::nivelFallado,
            this, [this](int n){
                if (n == 2)
                    volverAlMenu();
            });

    connect(nivel2, &Nivel::nivelCompletado,
            this, [this](int n){
                if (n == 2)
                    on_pushButton_3_clicked(); // pasar a nivel 3
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ───────── NIVEL 1 ─────────
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel1);

    this->setFocus();
}

// ───────── NIVEL 2 ─────────
void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);

    // Scroll horizontal para ver el fondo largo
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel2);

    // El teclado lo manejo yo (MainWindow)
    this->setFocus();

    nivel2->iniciar();
}

// ───────── NIVEL 3 ─────────
void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel3);

    this->setFocus();
}

// ───────── Volver al menú ─────────
void MainWindow::volverAlMenu()
{
    ui->graphicsView->setScene(menuScene);
    ui->graphicsView->setBackgroundBrush(
        QBrush(QImage(":/Sprites/Fondo/fondo.png"))
        );

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();

    this->setFocus();
}

// ───────── Teclado (manda órdenes a la nave) ─────────
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (ui->graphicsView->scene() == nivel2 && nivel2->nave()) {
        auto nave = nivel2->nave();

        if (event->key() == Qt::Key_Up) {
            nave->setArribaPresionado(true);
        } else if (event->key() == Qt::Key_Down) {
            nave->setAbajoPresionado(true);
        } else if (event->key() == Qt::Key_Right) {
            nave->setDerechaPresionado(true);
        }
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (ui->graphicsView->scene() == nivel2 && nivel2->nave()) {
        auto nave = nivel2->nave();

        if (event->key() == Qt::Key_Up) {
            nave->setArribaPresionado(false);
        } else if (event->key() == Qt::Key_Down) {
            nave->setAbajoPresionado(false);
        } else if (event->key() == Qt::Key_Right) {
            nave->setDerechaPresionado(false);
        }
    }

    QMainWindow::keyReleaseEvent(event);
}
