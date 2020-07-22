#include "clear_output_log_controller.h"
#include "file_reader.h"

#include <QFileInfo>

//constructor
Clear_Output_Log_Controller::Clear_Output_Log_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Clear_Output_Log_Controller::~Clear_Output_Log_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Clear_Output_Log_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Clear_Output_Log_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void Clear_Output_Log_Controller::clear_output_log()
{
    this->cur_wind->set_text_analysis_textbox(QString(""));


}
