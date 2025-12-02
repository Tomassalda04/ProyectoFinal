#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include "nivel.h"
QT_BEGIN_NAMESPACE
namespace Ui{class MainWindow;}
QT_END_NAMESPACE
class MainWindow:public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void volverAlMenu();
    void onNivelCompletado(int numeroNivel);
    void onNivelFallado(int numeroNivel);
private:
    Ui::MainWindow *ui;
    QGraphicsScene *menuScene=nullptr;
    Nivel *nivel1=nullptr;
    Nivel *nivel2=nullptr;
    Nivel *nivel3=nullptr;
};
#endif
