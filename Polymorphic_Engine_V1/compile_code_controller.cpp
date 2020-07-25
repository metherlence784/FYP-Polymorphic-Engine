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
    this->analysis_textbox_status = QString("");
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
	//get the file name from the main window (using substring)
    QString file_path = this->cur_wind->get_cur_file_path();
    std::size_t symbol = file_path.toStdString().find_last_of("/");// gets the last occurence of the "/" symbol in the returned file path
    std::string file_name = file_path.toStdString().substr(symbol+1);// gets everything after the last occurence of the "/" symbol
    this->file_name = QString(file_name.c_str());//file name should be .C or .CPP at this step
    return this->file_name;
}

QString Compile_Code_Controller::get_file_path()
{
	//gets the file path from the main window
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

//get the text from the analysis text box
QString Compile_Code_Controller::get_analysis_textbox_status()
{
    this->analysis_textbox_status = cur_wind->get_text_analysis_textbox();//th
    return this->analysis_textbox_status;
}

//get the temporary compilation status
QString Compile_Code_Controller::get_temp_compile()
{
    return this->temp_compile;
}

qint64 Compile_Code_Controller::get_elapsed_time()
{
    return this->elapsed_time;
}

QString Compile_Code_Controller::get_exe_file_path()
{
    return this->exe_file_path;
}

QString Compile_Code_Controller::get_exe_name()
{
    return this->exe_name;
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

void Compile_Code_Controller::set_exe_file_path(QString exe_file_path)
{
    this->exe_file_path = exe_file_path;
}

//enable the compile button
void Compile_Code_Controller::set_enabled_compile_button(bool set)
{
    this->cur_wind->set_enabled_compile_button(set);
}

void Compile_Code_Controller::set_enabled_compile_menu_item(bool set)
{
    this->cur_wind->ui->Compile_Menu_Item->setEnabled(set);
}

void Compile_Code_Controller::set_enabled_analysis_textbox(bool set)
{
    this->cur_wind->set_enabled_analysis_textbox(set);
}

void Compile_Code_Controller::set_status(QString txt)
{
    this->analysis_textbox_status = txt;
}

void Compile_Code_Controller::set_temp_compile(QString temp_compile)
{
    this->temp_compile = temp_compile;
}

void Compile_Code_Controller::set_elapsed_time(qint64 elapsed_time)
{
    this->elapsed_time = elapsed_time;
}

void Compile_Code_Controller::compile_code()
{
    Save_Current_File_Controller saver;
    saver.save();//save the current file, so that it compiles what evers on screen

    std::string dir = this->file_path.toStdString() + "/";
    this->temp_compile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/temp_compile.txt");

    //for cmd line
    QString command("");
    command += QString("g++ -Wall -o ");
    command += QString("\""); // quotation marks so that the file path will be read correctly
    command += QString(dir.c_str()) + this->exe_name + QString("\" ");
    command += QString("\"");
    command += QString(dir.c_str()) + this->file_name + QString("\" ");
    command += QString("2> ");
    command += this->temp_compile;
    std::cout << "Command: " << command.toStdString() << std::endl;

    //saving exe file path
	set_exe_file_path ( QString(dir.c_str()) + this->exe_name);
	
	//start of compiling
    QDateTime start = QDateTime::currentDateTime();
	
	//using the cmd line to compile
    system(command.toStdString().c_str());

	//end of compilation
    QDateTime end = QDateTime::currentDateTime();

    this->elapsed_time = start.msecsTo(end);
}

