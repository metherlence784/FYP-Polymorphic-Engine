#include "create_new_file_controller.h"
#include "file_saver.h"


//constructor
Create_New_File_Controller::Create_New_File_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Create_New_File_Controller::~Create_New_File_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}
//accesor
MainWindow * Create_New_File_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Create_New_File_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}


//methods below
void Create_New_File_Controller::create_new_file()
{
    File_Saver saving(get_cur_wind());
    QString file_path = saving.new_file();

    if(file_path == QString("system error"))
    {

    }
    else if (file_path == QString("Invalid Characters"))
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Unable save file due to invalid characters");

    }
    else if (file_path == QString("Invalid file extension"))
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Unable save file due to invalid file extension");

    }
    else
    {

        cur_wind->ui->Code_Input_Textbox->setPlainText("");//empties text box

        //enables the UI items after the user creates or opens his first file
        cur_wind->ui->Code_Input_Textbox->setEnabled(true);
        cur_wind->ui->Save_Menu_Item->setEnabled(true);
        cur_wind->ui->Save_As_Menu_Item->setEnabled(true);

        cur_wind->set_cur_filename(file_path);//saves the file path in the ui

    }
}
