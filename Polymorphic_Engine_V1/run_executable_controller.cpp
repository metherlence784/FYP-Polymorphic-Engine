#include "run_executable_controller.h"

//constructor
Run_Executable_Controller::Run_Executable_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->original_exe_name = QString("");
    this->original_exe_file_path = QString("");
    this->morphed_exe_name = QString("");
    this->morphed_exe_file_path = QString("");
}

//destructor
Run_Executable_Controller::~Run_Executable_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accessor
MainWindow * Run_Executable_Controller::get_cur_wind()
{
    return this->cur_wind;
}

QString Run_Executable_Controller::get_original_exe_name()
{
    return this->original_exe_name;
}

QString Run_Executable_Controller::get_original_exe_file_path()
{
    return this->original_exe_file_path;
}

QString Run_Executable_Controller::get_morphed_exe_name()
{
    return this->morphed_exe_name;
}

QString Run_Executable_Controller::get_morphed_exe_file_path()
{
    return this->morphed_exe_file_path;
}

//mutator
void Run_Executable_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void Run_Executable_Controller::set_morphed_exe_name(QString morphed_exe_name)
{
    this->morphed_exe_name = morphed_exe_name;
}

void Run_Executable_Controller::set_original_exe_name(QString original_exe_name)
{
    this->original_exe_name = original_exe_name;
}

void Run_Executable_Controller::set_original_exe_file_path(QString original_exe_file_path)
{
    this->original_exe_file_path = original_exe_file_path;
}

void Run_Executable_Controller::set_morphed_exe_file_path(QString morphed_exe_file_path)
{
    this->morphed_exe_file_path = morphed_exe_file_path;
}

void Run_Executable_Controller::set_enabled_run_original_button(QString compilation_status)
{
    if(compilation_status.contains("ERROR") == false)
    {
        this->cur_wind->set_enabled_run_original_button(true);
    }
}

void Run_Executable_Controller::set_enabled_run_morphed_button(QString morph_status)
{
    if(morph_status.contains("ERROR") == false)
    {
        this->cur_wind->set_enabled_run_morphed_button(true);
    }
}

void Run_Executable_Controller::set_original_executable_label(QString original_status)
{
    if(original_status.contains("ERROR") == false)
    {
        this->cur_wind->set_original_executable_label(this->original_exe_name);
    }
}

void Run_Executable_Controller::set_morphed_executable_label(QString morphed_status)
{
    if(morphed_status.contains("ERROR") == false)
    {
        this->cur_wind->set_morphed_executable_label(this->morphed_exe_name);
    }
}

void Run_Executable_Controller::run_original_executable(QString original_exe_file_path)
{
    std::string command = "start cmd.exe /k cmd /c \"" + original_exe_file_path.toStdString() +  "\"";
    std::cout << command << std::endl;
    system(command.c_str());
}

void Run_Executable_Controller::run_morphed_executable(QString morphed_exe_file_path)
{
    std::string command = "start cmd.exe /k cmd /c \"" + morphed_exe_file_path.toStdString() +  "\"";
    std::cout << command << std::endl;
    system(command.c_str());
}

