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

void Choose_Payload_Controller::set_initial_payload_info(QString compilation_status)
{
    if(compilation_status.contains("ERROR") == false)
    {
        QColor color("green");
        QString info = calculator_payload_info;
        this->cur_wind->set_text_and_color_payload_info_textbox(info,color);
    }
}



void Choose_Payload_Controller::set_text_and_color_payload_info_textbox(int id)
{
    QString info = QString("");
    QColor color("green");

    switch(id)
    {
    case 0:
        info = calculator_payload_info;
        break;
    case 1:
        info = CMD_payload_info;
        break;
    case 2:
        info = message_box_payload_info;
        break;
    case 3:
        info = fatality_payload_info;
        break;
    case 4:
        info = warning + system_info_payload_info;
        color.setNamedColor("red");
        break;
    case 5:
        info = warning + download_putty_payload_info;
        color.setNamedColor("red");
        break;
    case 6:
        info = warning + new_admin_payload_info + as_admin;
        color.setNamedColor("red");
        break;
    default:
        break;
    }

    this->cur_wind->set_text_and_color_payload_info_textbox(info,color);
}

int Choose_Payload_Controller::get_payload()
{
	//get the payload from thr radio buttons
    this->chosen_payload = this->cur_wind->get_payload_radio_button();
    return this->chosen_payload;
}
