#include "choose_payload_controller.h"

Choose_Payload_Controller::Choose_Payload_Controller()
{
   this->cur_wind = MainWindow::getMWptr();
}

Choose_Payload_Controller::~Choose_Payload_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

QString Choose_Payload_Controller::get_payload()
{
    return this->cur_wind->get_payload_radio_button();
}
