#include "exit_application_controller.h"

//constructor

//MainWindow*::Exit_Application_Controller::cur_wind = nullptr;

Exit_Application_Controller::Exit_Application_Controller()
{
   cur_wind = MainWindow::getMWptr();
}

//destructor
Exit_Application_Controller::~Exit_Application_Controller()
{
    delete cur_wind;
}

//exit function
void Exit_Application_Controller::exit_application()
{
    cur_wind->ui->Code_Input_Textbox->setPlainText("ur mom");
    QApplication::quit();
}
