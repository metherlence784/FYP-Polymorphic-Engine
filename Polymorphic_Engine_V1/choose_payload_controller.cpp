#include "choose_payload_controller.h"

//constructor
Choose_Payload_Controller::Choose_Payload_Controller()
{
   this->cur_wind = MainWindow::getMWptr();
}

//destructor
Choose_Payload_Controller::~Choose_Payload_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesssor
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
void Choose_Payload_Controller::set_enabled_payload_groupbox(QString compilation_status)
{
	//enable the payload radio buttons
    if(compilation_status.contains("ERROR") == false)
    {
        this->cur_wind->set_enabled_payload_groupbox(true);
    }
}

QString Choose_Payload_Controller::get_payload()
{
	//get the payload from thr radio buttons
    this->chosen_payload = this->cur_wind->get_payload_radio_button();
    return this->chosen_payload;
}
