#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Para poder recibir eventos de teclado
    setFocusPolicy(Qt::StrongFocus);

    // La escena empieza alineada arriba a la izquierda
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // En el menú no necesitamos scroll
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ───────── NIVEL 1 (botón "NIVEL 1") ─────────
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

// ───────── NIVEL 2 (botón "NIVEL 2") ─────────
void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    ui->graphicsView->setBackgroundBrush(Qt::NoBrush);

    // Nivel 2: scroll horizontal activado
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(nivel2);

    this->setFocus();
}

// ───────── NIVEL 3 (botón "NIVEL 3") ─────────
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

// ───────── Volver al menú (por si lo usas luego) ─────────
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

// ───────── Teclado: mover cohete en NIVEL 2 ─────────
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Solo manejamos el teclado cuando la escena actual es el nivel 2
    if (ui->graphicsView->scene() == nivel2 && nivel2->cohete()) {
        const int paso = 15; // píxeles por pulsación

        int dx = 0;
        int dy = 0;

        if (event->key() == Qt::Key_Right) {
            dx = +paso;
        } else if (event->key() == Qt::Key_Left) {
            dx = -paso;
        } else if (event->key() == Qt::Key_Up) {
            dy = -paso;    // hacia arriba (Y disminuye)
        } else if (event->key() == Qt::Key_Down) {
            dy = +paso;    // hacia abajo (Y aumenta)
        }

        if (dx != 0 || dy != 0) {
            nivel2->moverCohete(dx, dy);
            ui->graphicsView->centerOn(nivel2->cohete());
        }
    }

    QMainWindow::keyPressEvent(event);
}
