#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    //constructor
    MainWindow(QWidget *parent = nullptr);

    //destructor
    ~MainWindow();
    static MainWindow* getMWptr();

private slots:
    void on_Exit_Menu_Item_triggered();

private:
    static MainWindow *MWptr;
};
#endif // MAINWINDOW_H
