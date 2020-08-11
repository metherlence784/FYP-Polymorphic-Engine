#include "view_help_information_controller.h"

//constuctor
View_Help_Information_Controller::View_Help_Information_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->website_link = "https://sgsean8.wixsite.com/polygine/user-manual";
}

//destructor
View_Help_Information_Controller::~View_Help_Information_Controller()
{

}

//accesor
MainWindow * View_Help_Information_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void View_Help_Information_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void View_Help_Information_Controller::help()
{
    QDesktopServices::openUrl(QUrl(website_link));
}
