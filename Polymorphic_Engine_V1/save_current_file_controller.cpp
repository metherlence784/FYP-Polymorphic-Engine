#include "save_current_file_controller.h"
#include "file_saver.h"

Save_Current_File_Controller::Save_Current_File_Controller()
{
     this->cur_wind = MainWindow::getMWptr();

}

Save_Current_File_Controller::~Save_Current_File_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Save_Current_File_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Save_Current_File_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void Save_Current_File_Controller::save()
{
    QString filepath = cur_wind->get_cur_filename();
    File_Saver save(get_cur_wind());
    save.save_file(filepath);

}


