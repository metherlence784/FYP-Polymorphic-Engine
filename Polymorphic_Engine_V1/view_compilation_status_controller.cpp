#include "view_compilation_status_controller.h"
#include "file_reader.h"

//constructor
View_Compilation_Status_Controller::View_Compilation_Status_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//desstructor
View_Compilation_Status_Controller::~View_Compilation_Status_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accessors
QString View_Compilation_Status_Controller::get_compilation_status()
{
    return this->compilation_status;
}

MainWindow *View_Compilation_Status_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutators
void View_Compilation_Status_Controller::set_text_analysis_textbox(QString txt)
{
    this->cur_wind->set_text_analysis_textbox(txt);
}

void View_Compilation_Status_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void View_Compilation_Status_Controller::set_compilation_status(QString compilation_status)
{
    this->compilation_status = compilation_status;
}

void View_Compilation_Status_Controller::update_analysis_textbox_and_enable_morph(QString analysis_textbox_status, qint64 elapsed_time, QString txt_file)
{
    this->cur_wind->ui->tabWidget->setCurrentIndex(1);//switch to analysis tab

        QString previous_text_from_analysis = analysis_textbox_status;

        QDateTime current = QDateTime::currentDateTime(); // to get the current time and day

        const QString stars = QString("*************************************\n");
        QString format;

        File_Reader reader;
        this->compilation_status = reader.read_compile_status(txt_file);

        format += previous_text_from_analysis;
        format += stars;
        format += QString("COMPILATION STATUS AT : ") +  current.toString() + QString("\n");
        format += QString("ELAPSED TIME : ") + QString::number(elapsed_time) + QString(" ms") + QString("\n\n\n");

        if(this->compilation_status == ERROR_INVALID_FILE)
        {
            format += this->compilation_status + QString("\n");
        }
        else if(this->compilation_status == QString(""))
        {
            this->compilation_status = SUCCESS_COMPILED_CODE;
            format +=  QString("Sucessfully Compiled") + QString("\n");
            this->cur_wind->ui->Morph_Button->setEnabled(true);
            this->cur_wind->ui->Morph_Menu_Item->setEnabled(true);
        }
        else
        {
            format += this->compilation_status + QString("\n");
        }

        format += stars + QString("\n\n");
        set_text_analysis_textbox(format);
}

