#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void render();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initPBRTResource();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
