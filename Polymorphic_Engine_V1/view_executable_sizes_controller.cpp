#include "view_executable_sizes_controller.h"

//constructor
View_Executable_Sizes_Controller::View_Executable_Sizes_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
View_Executable_Sizes_Controller::~View_Executable_Sizes_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow* View_Executable_Sizes_Controller::get_cur_wind()
{
    return this->cur_wind;
}

QString View_Executable_Sizes_Controller::get_text_analysis_textbox()
{
    return this->cur_wind->get_text_analysis_textbox();
}



//mutator
void View_Executable_Sizes_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void View_Executable_Sizes_Controller::set_text_analysis_textbox(QString txt)
{
    this->cur_wind->set_text_analysis_textbox(txt);
}


//other methods
void View_Executable_Sizes_Controller::set_executable_sizes_in_analysis_textbox(QString original_exe_file_path,
                                                                  QString original_exe_name,
                                                                  QString morphed_exe_file_path,
                                                                  QString morphed_exe_name)
{
    File_Reader reader;

    qint64 original_exe_size = reader.get_executable_size(original_exe_file_path);
    qint64 morphed_exe_size = reader.get_executable_size(morphed_exe_file_path);

    const QString stars = QString("******************************************************\n");

    QString analysis_text_box_text = get_text_analysis_textbox();

    analysis_text_box_text += stars;

    analysis_text_box_text += QString("COMPARING EXECUTABLE SIZES\n\n");

    analysis_text_box_text += format_text(original_exe_name,original_exe_size);

    analysis_text_box_text += format_text(morphed_exe_name,morphed_exe_size) + QString("\n");

    qint64 difference = qAbs(morphed_exe_size - original_exe_size);
    analysis_text_box_text += QString("Difference in size: ") + convert_kb_to_mb(difference)
            + QString("\n\n");

    analysis_text_box_text += stars;
    set_text_analysis_textbox(analysis_text_box_text);
}

QString View_Executable_Sizes_Controller::format_text(QString exe_name, qint64 exe_size)
{
    QString result = QString("");
    if(check_if_file_exist(exe_size) == true )
    {
        result += exe_name + QString(": ") +
                convert_kb_to_mb(exe_size) + QString("\n");
    }
    else
    {
        result += ERROR_FILE_NOT_FOUND + QString(": ") +
                convert_kb_to_mb(exe_size) + QString("\n");
    }
    return result;
}

bool View_Executable_Sizes_Controller::check_if_file_exist(qint64 exe_size)
{
    if(exe_size == 0)
    {
        return false;
    }
    return true;
}

QString View_Executable_Sizes_Controller::convert_kb_to_mb(qint64 exe_size)
{
    qint64 temp = exe_size / 1000;
    QString result = QString::number(temp) + QString(".") + QString::number(exe_size - temp);
    result += QString(" mb");
    std::cout << "Test size: " << result.toStdString() << std::endl;
    return result;
}
