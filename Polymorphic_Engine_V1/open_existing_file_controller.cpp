#include "open_existing_file_controller.h"
#include "file_reader.h"

Open_Existing_File_Controller::Open_Existing_File_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

Open_Existing_File_Controller::~Open_Existing_File_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Open_Existing_File_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Open_Existing_File_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void Open_Existing_File_Controller::open_existing_file()
{
    File_Reader reader(get_cur_wind());
    QStringList list = reader.open_file();//index 0 is file path, index 1 is file content

    if(list.at(0) == QString("error"))
    {
        // this is if in file_reader, an error has occured because the user tried to open an undefined
        // file type, example if user tries to open a shortcut, as it is not handled by qt filter
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid file type: " + list.at(1));
    }
    else
    {

        cur_wind->set_cur_filename(list.at(0)); // sets the current filepath for the application
        cur_wind->ui->Code_Input_Textbox->setText(list.at(1)); // sets the code text box to the file content

        //enables the UI items after the user creates or opens his first file
        cur_wind->ui->Code_Input_Textbox->setEnabled(true);
        cur_wind->ui->Save_Menu_Item->setEnabled(true);
        cur_wind->ui->Save_As_Menu_Item->setEnabled(true);
    }

}

