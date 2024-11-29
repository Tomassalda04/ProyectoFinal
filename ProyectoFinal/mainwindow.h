#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fscene.h"
#include "tscene.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT




public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onLevelSelected(int level);

private:
    Ui::MainWindow *ui;
    Fscene *First_Scene;
    Tscene *Two_Scene;
    QGraphicsScene *initialScene;
    QGraphicsView *graphicsView;
    QPushButton *menuButton;
    QLabel *titulo;
    QLabel *Cred;


};
#endif // MAINWINDOW_H
