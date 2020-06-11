#include "view_compilation_status_controller.h"
#include "file_reader.h"

View_Compilation_Status_Controller::View_Compilation_Status_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

View_Compilation_Status_Controller::~View_Compilation_Status_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

void View_Compilation_Status_Controller::set_text_analysis_textbox(QString txt)
{
    this->cur_wind->set_text_analysis_textbox(txt);
}

void View_Compilation_Status_Controller::update_analysis_textbox(QString status, qint64 elapsed_time, QString txt_file)
{
    this->cur_wind->ui->tabWidget->setCurrentIndex(1);

        QString previous_text_from_analysis = status;

        QDateTime current = QDateTime::currentDateTime(); // to get the current time and day

        const QString stars = QString("******************************************************\n");
        QString format;

        File_Reader reader;
        QString current_status = reader.read_compile_status(txt_file);

        format += previous_text_from_analysis;
        format += stars;
        format += QString("COMPILATION STATUS AT : ") +  current.toString() + QString("\n");
        format += QString("ELAPSED TIME : ") + QString::number(elapsed_time) + QString(" ms") + QString("\n\n\n");

        if(current_status == ERROR_INVALID_FILE)
        {
            format += current_status + QString("\n");
        }
        else if(current_status == QString(""))
        {
            format +=  QString("Sucessfully Compiled") + QString("\n");
        }
        else
        {
            format += current_status + QString("\n");
        }

        format += stars + QString("\n\n");
        set_text_analysis_textbox(format);
}

