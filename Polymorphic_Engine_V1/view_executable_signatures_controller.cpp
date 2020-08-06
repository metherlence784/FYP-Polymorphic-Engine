#include "view_executable_signatures_controller.h"

//constructor
View_Executable_Signatures_Controller::View_Executable_Signatures_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
View_Executable_Signatures_Controller::~View_Executable_Signatures_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow* View_Executable_Signatures_Controller::get_cur_wind()
{
    return this->cur_wind;
}

QString View_Executable_Signatures_Controller::get_text_analysis_textbox()
{
    return this->cur_wind->get_text_analysis_textbox();
}

//mutator
void View_Executable_Signatures_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

void View_Executable_Signatures_Controller::set_text_analysis_textbox(QString txt)
{
    this->cur_wind->set_text_analysis_textbox(txt);
}

bool View_Executable_Signatures_Controller::check_if_file_exist(QString status)
{
    if(status.contains("SUCCESS") == true)//if success then return
    {
        return true;
    }
    return false;
}

//put the hashes in the anaylsis txt box
void View_Executable_Signatures_Controller::set_executable_signature_in_analysis_textbox(QString original_exe_file_path, QString original_exe_name, QString morphed_exe_file_path, QString morphed_exe_name)
{
    File_Reader reader;
	
	//store both exe into buffers
    std::vector<char> original_buffer;
    std::vector<char> morphed_buffer;

    QString original_status = reader.read_file_into_vector(original_exe_file_path,original_buffer);
    QString morphed_status = reader.read_file_into_vector(morphed_exe_file_path,morphed_buffer);
	
	//hash the contents of buffers
    QString original_hash = hash_buffer_MD5(original_buffer,original_status);
    QString morphed_hash = hash_buffer_MD5(morphed_buffer,morphed_status);

    const QString stars = QString("*************************************\n");

    QString analysis_text_box_text = get_text_analysis_textbox();

    analysis_text_box_text += stars;

    analysis_text_box_text += QString("COMPARING EXECUTABLE SIGNATURES\n\n");

    analysis_text_box_text += format_text( original_hash,QString("Original ==> ") +original_exe_name,original_status);

    analysis_text_box_text += format_text(morphed_hash,QString("Morphed ==> ") +morphed_exe_name,morphed_status) + QString("\n");

    analysis_text_box_text += compare_hashes(original_hash,morphed_hash) + QString("\n\n");

    analysis_text_box_text += stars + QString("\n\n");
    set_text_analysis_textbox(analysis_text_box_text);
}

QString View_Executable_Signatures_Controller::hash_buffer_MD5(std::vector<char> buffer, QString status)
{
    QString result = QString("No Signature!");

    if(check_if_file_exist(status) == true)
    {
        QCryptographicHash hasher(QCryptographicHash::Md5);//using md5 hash 
        hasher.addData(buffer.data(),buffer.size());
        result = hasher.result().toHex();
    }
    return result;
}

//just to show if hashes are the same or not
QString View_Executable_Signatures_Controller::compare_hashes(QString original_hash, QString morphed_hash)
{
    if(original_hash == morphed_hash)
    {
        return QString("The hash values are the same!");
    }
    return QString("The hash values are different!");
}

QString View_Executable_Signatures_Controller::format_text(QString hash, QString exe_name, QString status)
{
    QString result = QString("");

    if(check_if_file_exist(status) == true)
    {
        result += exe_name + QString(" : ");
        result += hash + QString("\n");
    }
    else
    {
        result += exe_name + QString(" ") + ERROR_FILE_NOT_FOUND + QString(" : ");
        result += hash + QString("\n");
    }

    return result;
}

void View_Executable_Signatures_Controller::set_enabled_view_executable_signature_button(QString compilation_status)
{
    if(compilation_status.contains("SUCCESS") == true)
    {
        this->cur_wind->set_enabled_view_executable_signature_button(true);
    }
}
