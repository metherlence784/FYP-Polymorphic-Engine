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

//accessor
MainWindow * Choose_Payload_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Choose_Payload_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

//mutator
void Choose_Payload_Controller::set_enabled_payload_groupbox(bool set)
{
    this->cur_wind->set_enabled_payload_groupbox(set);
}

QString Choose_Payload_Controller::get_payload()
{
    return this->cur_wind->get_payload_radio_button();
}
