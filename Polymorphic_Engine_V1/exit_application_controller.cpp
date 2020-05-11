#include "exit_application_controller.h"


//constructor
Exit_Application_Controller::Exit_Application_Controller()
{
   this->cur_wind = MainWindow::getMWptr();
}

//destructor
Exit_Application_Controller::~Exit_Application_Controller()
{
   this->cur_wind = nullptr;
   delete this->cur_wind;
}

//exit function
void Exit_Application_Controller::exit_application()
{

    QApplication::quit();
}
