#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>

#include "nivel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_2_clicked();  // NIVEL 1
    void on_pushButton_clicked();    // NIVEL 2
    void on_pushButton_3_clicked();  // NIVEL 3

    void volverAlMenu();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *menuScene = nullptr;

    Nivel *nivel1 = nullptr;
    Nivel *nivel2 = nullptr;
    Nivel *nivel3 = nullptr;
};

#endif // MAINWINDOW_H


