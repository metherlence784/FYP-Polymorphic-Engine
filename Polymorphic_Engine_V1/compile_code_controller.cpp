#include "compile_code_controller.h"
#include "write_code_in_c_cpp_controller.h"
#include "save_current_file_controller.h"
#include "file_reader.h"


//constructor
Compile_Code_Controller::Compile_Code_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->file_name = QString("");
    this->file_path = QString("");
    this->exe_name = QString("");
    this->status = QString("");
    this->temp_compile = QString("");


}

//destructor
Compile_Code_Controller::~Compile_Code_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
QString Compile_Code_Controller::get_file_name()
{
    QString file_path = this->cur_wind->get_cur_file_path();
    std::size_t symbol = file_path.toStdString().find_last_of("/");// gets the last occurence of the "/" symbol in the returned file path
    std::string file_name = file_path.toStdString().substr(symbol+1);// gets everything after the last occurence of the "/" symbol
    this->file_name = QString(file_name.c_str());//file name should be .C or .CPP at this step
    return this->file_name;
}

QString Compile_Code_Controller::get_file_path()
{
    QString file_path = this->cur_wind->get_cur_file_path();
    std::size_t symbol = file_path.toStdString().find_last_of("/");// gets the last occurence of the "/" symbol in the returned file path
    std::string file_name = file_path.toStdString().substr(0,symbol);// gets everything before the last occurence of the "/" symbol
    this->file_path = QString(file_name.c_str());//this should only be the directory to save files
    return this->file_path;
}

MainWindow * Compile_Code_Controller::get_cur_wind()
{
    return this->cur_wind;
}

QString Compile_Code_Controller::get_status()
{
    this->status = cur_wind->get_text_analysis_textbox();
    return this->status;
}

//mutator
void Compile_Code_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void Compile_Code_Controller::set_exe_name()
{
    std::size_t symbol = this->file_name.toStdString().find_last_of(".");// gets the last occurence of the "." symbol in the returned file path
    std::string file_name = this->file_name.toStdString().substr(0,symbol);// gets everything after the last occurence of the "." symbol
    file_name += "_original.exe";
    this->exe_name = QString(file_name.c_str());
}

void Compile_Code_Controller::set_enabled_compile_button(bool set)
{
    this->cur_wind->set_enabled_compile_button(set);
}

void Compile_Code_Controller::set_enabled_analysis_textbox(bool set)
{
    this->cur_wind->set_enabled_analysis_textbox(set);
}

void Compile_Code_Controller::set_status(QString txt)
{
    this->status = txt;
}

void Compile_Code_Controller::set_text_analysis_textbox(QString txt)
{
    this->cur_wind->set_text_analysis_textbox(txt);
}

void Compile_Code_Controller::compile_code()
{
    Save_Current_File_Controller saver;
    saver.save();//save the current file, so that it compiles what evers on screen

    std::string dir = this->file_path.toStdString() + "/";
    this->temp_compile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/temp_compile.txt");

    //cmd line
    QString command("");
    command += QString("g++ -Wall -o ");
    command += QString(dir.c_str()) + this->exe_name + QString(" ");
    command += QString(dir.c_str()) + this->file_name + QString(" ");
    command += QString("2> ");
    command += this->temp_compile;

    QDateTime start = QDateTime::currentDateTime();

    system(command.toStdString().c_str());

    QDateTime end = QDateTime::currentDateTime();

    this->elapsed_time = start.msecsTo(end);
}

void Compile_Code_Controller::update_analysis_textbox()
{
    QString text_from_analysis = get_status();

    QDateTime current = QDateTime::currentDateTime(); // to get the current time and day

    const QString stars = QString("******************************************************\n");
    QString format;

    File_Reader reader;
    this->status = reader.read_compile_status(this->temp_compile);

    format += text_from_analysis;
    format += stars;
    format += QString("COMPILATION STATUS AT : ") +  current.toString() + QString("\n");
    format += QString("ELAPSED TIME : ") + QString::number(this->elapsed_time) + QString(" ms") + QString("\n\n\n");

    if(this->status == ERROR_INVALID_FILE)
    {
        format += this->status + QString("\n");
    }
    else if(this->status == QString(""))
    {
        format +=  QString("Sucessfully Compiled") + QString("\n");
    }
    else
    {
        format += this->status + QString("\n");
    }

    format += stars + QString("\n\n");
    set_text_analysis_textbox(format);

}
