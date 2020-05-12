#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

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

    //accesor
    static MainWindow* getMWptr();
    QString get_cur_filename();

    //mutator
    void set_cur_filename(QString str);

private slots:
    void on_Exit_Menu_Item_triggered();
    void on_New_File_Menu_Item_triggered();
    void on_Save_As_Menu_Item_triggered();
    void on_Save_Menu_Item_triggered();

    void on_Open_File_Menu_Item_triggered();

private:
    static MainWindow *MWptr;
    QString cur_file_path;

    void closeEvent (QCloseEvent *event);
};
#endif // MAINWINDOW_H
