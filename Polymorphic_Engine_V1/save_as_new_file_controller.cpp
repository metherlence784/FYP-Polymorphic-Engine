#include "save_as_new_file_controller.h"


//constructor
Save_As_New_File_Controller::Save_As_New_File_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Save_As_New_File_Controller::~Save_As_New_File_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Save_As_New_File_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Save_As_New_File_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

//saving as new file
void Save_As_New_File_Controller::save_as_new_file()
{
    File_Saver saving;

    Write_Code_In_C_CPP_Controller writer;//uses the writer object to access the text from the UI
    QString file_path = saving.save_file_as(writer.get_text());
    std::cout << " FILE PATH " << file_path.toStdString() << std::endl;

    if(file_path == ERROR_SYSTEM_FAULT)
    {
        //error handling if theres file reading system related problems
    }
    else if (file_path == ERROR_INVALID_CHARACTERS)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "<font size = 4>Unable to save file due to invalid characters</font>");

    }
    else if (file_path == ERROR_INVALID_FILE_EXTENSION)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "<font size = 4>Unable to save file due to invalid file extension</font>");
    }
    else
    {
        this->cur_wind->set_cur_file_path(file_path);
    }
}
