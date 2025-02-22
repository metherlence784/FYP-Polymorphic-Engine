#include "save_current_file_controller.h"

//constructor
Save_Current_File_Controller::Save_Current_File_Controller()
{
    this->cur_wind = MainWindow::getMWptr();

}

//destructor
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
    QString filepath = cur_wind->get_cur_file_path();//uses the filepath saved in the main ui object
    Write_Code_In_C_CPP_Controller writer;

    File_Saver saver;
    std::cout<< filepath.toStdString() + "\n";
    saver.save_file(filepath, writer.get_text());//uses the file saver to save the file
}


