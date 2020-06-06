#include "undo_action_controller.h"

//constructor
Undo_Action_Controller::Undo_Action_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Undo_Action_Controller::~Undo_Action_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

void Undo_Action_Controller::undo_action()
{
    this->cur_wind->ui->Code_Input_Textbox->undo();
}
