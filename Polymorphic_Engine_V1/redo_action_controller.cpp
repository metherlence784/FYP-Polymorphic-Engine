#include "redo_action_controller.h"


//constructor
Redo_Action_Controller::Redo_Action_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Redo_Action_Controller::~Redo_Action_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Redo_Action_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Redo_Action_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

//redo action
void Redo_Action_Controller::redo_action()
{
    this->cur_wind->ui->Code_Input_Textbox->redo();
}

