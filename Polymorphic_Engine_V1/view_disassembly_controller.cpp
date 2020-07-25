#include "view_disassembly_controller.h"

//constructor
View_Disassembly_Controller::View_Disassembly_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->file_text = QString("");
}

//destructor
View_Disassembly_Controller::~View_Disassembly_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow* View_Disassembly_Controller::get_cur_wind()
{
    return this->cur_wind;
}

QString View_Disassembly_Controller::get_file_text()
{
    return this->file_text;
}

//mutator
void View_Disassembly_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void View_Disassembly_Controller::set_file_text(QString file_text)
{
    this->file_text = file_text;
}

void View_Disassembly_Controller::set_enabled_view_disassembly_button(QString morph_status)
{
    if(morph_status.contains("ERROR") == false)//if not error then enable
    {
        this->cur_wind->set_enabled_view_disassembly_button(true);
    }
}

void View_Disassembly_Controller::write_disassembly(QString file_path, QString disassembly)
{
	//get current directory
    std::string file_directory = file_path.toStdString();
    file_directory = file_directory.substr(0,file_directory.find_last_of("/") + 1);

    std::cout << "FILES DIRECTORY: " << file_directory << std::endl;

    file_path = QString(file_directory.c_str());
    file_path = set_disassembly_log_path(file_path);
    get_text_from_disassembly_log(file_path);

    this->file_text += disassembly;

    File_Saver saver;
    saver.save_file(file_path,this->file_text);

    QDesktopServices::openUrl(QUrl(file_path, QUrl::TolerantMode));//pop up txt file for the disassembly
}

void View_Disassembly_Controller::get_text_from_disassembly_log(QString file_path)
{
    File_Reader reader;
    set_file_text(reader.file_check(file_path));

    if(this->file_text.contains("ERROR") == true)//if not error then set
    {
        set_file_text(QString(""));
    }
}

QString View_Disassembly_Controller::set_disassembly_log_path(QString file_path)
{
    return file_path + QString("Disassembly_log.txt");
}


