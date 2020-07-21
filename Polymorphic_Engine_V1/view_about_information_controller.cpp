#include "view_about_information_controller.h"

//https://sgsean8.wixsite.com/polygine

View_About_Information_Controller::View_About_Information_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->about_us = QString("This polymorphic engine is made by CHIMERA, consisting of : <br>");
    this->about_us += QString("<pre>    V.Yogeswarren<br>"
                              "    Jeremiah Timothy Franis<br>"
                              "    Lester Lim Jun An<br>");
    this->about_us += QString("    Sean Chia Jin Yang<br>"
                              "    Joshua Ho Jia Sheng<br></pre>");
    this->about_us += QString("Special thanks to : <pre>    c3xp1r</pre>");
    this->about_us += QString("Polymorphic Engine version 1.0<br><br>");
    this->about_us += QString("For more information about the polymorphic engine, click ");
    this->about_us += QString("<a href=\'https://sgsean8.wixsite.com/polygine/\'>here</a>");

}

//destructor
View_About_Information_Controller::~View_About_Information_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * View_About_Information_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void View_About_Information_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void View_About_Information_Controller::view_about_us()
{
   QMessageBox about_box;
   about_box.setTextFormat(Qt::RichText);   //this is what makes the links clickable
   about_box.setWindowTitle("About us");
   about_box.setText(this->about_us);
   about_box.setStandardButtons(QMessageBox::Ok);
   about_box.exec();
}
