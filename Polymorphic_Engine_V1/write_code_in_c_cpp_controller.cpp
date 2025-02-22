#include "write_code_in_c_cpp_controller.h"

//constructor
Write_Code_In_C_CPP_Controller::Write_Code_In_C_CPP_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->text = QString("");
}

//destructor
Write_Code_In_C_CPP_Controller::~Write_Code_In_C_CPP_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
QString Write_Code_In_C_CPP_Controller::get_text()
{
    this->text = cur_wind->get_text_code_input_textbox();
    return this->text;
}

MainWindow * Write_Code_In_C_CPP_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Write_Code_In_C_CPP_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

//set the text into the input text box
void Write_Code_In_C_CPP_Controller::set_text(QString txt)
{
    this->text = txt;
    this->cur_wind->set_text_code_input_textbox(text);
}

void Write_Code_In_C_CPP_Controller::set_enabled_code_input_textbox(bool set)
{
    this->cur_wind->set_enabled_code_input_textbox(set);
}


