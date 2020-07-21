#include "clear_output_log_controller.h"
#include "file_reader.h"

#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>

//constructor
Clear_Output_Log_Controller::Clear_Output_Log_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Clear_Output_Log_Controller::~Clear_Output_Log_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Clear_Output_Log_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Clear_Output_Log_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void Clear_Output_Log_Controller::clear_output_log()
{
    this->cur_wind->set_text_analysis_textbox(QString(""));

    std::vector<char> buffer;
    File_Reader *reader = new File_Reader();
    reader->read_file_into_vector(QString("C:/Users/jtf25/Pictures/memes/Test/abc_original.exe"),buffer);
    QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Md5);
    hash->addData(buffer.data(),buffer.size());
    QByteArray my_byte = hash->result();

    qDebug(my_byte.toHex());

}
