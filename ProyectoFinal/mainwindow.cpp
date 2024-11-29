#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fscene.h"
#include "tscene.h"
#include <QScreen>
#include <QVBoxLayout>
#include <QPalette>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), First_Scene(nullptr), Two_Scene(nullptr)
{
    ui->setupUi(this);
    graphicsView = new QGraphicsView(this);
    setCentralWidget(graphicsView);

    QScreen * screen = QGuiApplication :: primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int frameHeight = frameGeometry().height() - geometry().height();

    int viewWidth = screenWidth;
    int viewHeight = screenHeight - frameHeight;

    graphicsView->setFixedSize(viewWidth, viewHeight);

    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    menuButton = new QPushButton(this);
    QString buttonStyleM = "QPushButton {"
                           "background-image: url(:/START.png);"
                           "background-repeat: no-repeat;"
                           "background-position: center;"
                           "border: none;"
                           "width: 793px;"
                           "height: 250px;"
                           "}";

    menuButton->setStyleSheet(buttonStyleM);
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::onLevelSelected);

    int buttonWidth = menuButton->width();
    int buttonHeight = menuButton->height();
    int buttonX = ((viewWidth - buttonWidth) / 2) - 396.5;
    int buttonY = ((viewHeight - buttonHeight) / 2) - 125;
    menuButton->setGeometry(0,0,793,250);
    menuButton->move(buttonX, buttonY);

    titulo = new QLabel("THE SIMPSONS", this);
    titulo->setStyleSheet("QLabel { color: yellow; font-size: 100px; }");
    titulo->adjustSize();
    int labelWidth = 396.5-(titulo->width()/2);
    titulo->move(menuButton->x()+labelWidth, menuButton->y() - 350);

    Cred = new QLabel("Juego proyecto final info 2 \nCreado por Tomas Restrepo Saldarriaga", this);
    Cred->setStyleSheet("QLabel { color: black; font-size: 25px; }");
    Cred->adjustSize();
    Cred->move(50, 900);

    initialScene = new QGraphicsScene(this);
    initialScene->setSceneRect(0, 0, viewWidth, viewHeight);
    graphicsView->setScene(initialScene);
    initialScene->setBackgroundBrush(QBrush(QPixmap(":/simpsons.jpg").scaled(viewWidth, viewHeight)));

}



void MainWindow::onLevelSelected(int level)
{
    qDebug() << "nivel seleccionado:" << level;

    if (First_Scene) {
        delete First_Scene;
        First_Scene = nullptr;
    }
    if (Two_Scene) {
        delete Two_Scene;
        Two_Scene = nullptr;
    }

    if (level == 0) {
        qDebug() << "Crea First_Scene";
        First_Scene = new Fscene(this);
        graphicsView->setScene(First_Scene);


    }/* else if (level == 2) {
        Two_Scene = new twoscene(this);
        graphicsView->setScene(Two_Scene);
        Cred->hide();

    }
*/
}
MainWindow::~MainWindow()
{
    delete ui;
}
