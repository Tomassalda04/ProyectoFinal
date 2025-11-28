#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qgraphicsscene.h"
#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *l1,l2,l3;
    QGraphicsScene *text;
};
#endif // MAINWINDOW_H
