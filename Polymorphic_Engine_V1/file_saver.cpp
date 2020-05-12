#include "file_saver.h"

File_Saver::File_Saver()
{
    this->cur_wind = nullptr;
}

File_Saver::File_Saver(MainWindow *mw_ptr)
{
    this->cur_wind = mw_ptr;
}

File_Saver::~File_Saver()
{
    this->cur_wind = nullptr;
   delete this->cur_wind;
}


QString File_Saver::new_file()
{
    QString filters("C++ file (*.cpp);;C file (*.c)");
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();

    file_path = QFileDialog::getSaveFileName(cur_wind,"Save your file",QDir::currentPath()
                                                 ,filters,&default_filter); // this opens the file directory window

    cur_wind->set_cur_filename(file_path);//set file name

    QFile file(file_path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        // any error involving reading of files i.e. system errors
        return QString("system error");
    }
    QTextStream out(&file);
    QString text = QString("");
    out << text;
    file.close();

    //checking for invalid characters set up
    QRegExp rx("[@!#$%^&*<>?/|}{~:]");
    std::size_t symbol = file_path.toStdString().find_last_of("/");
    std::string cur_dir = file_path.toStdString().substr(symbol+1);
    QString cur_file = QString::fromStdString(cur_dir);

    if(cur_file.contains(rx) == true) // if file name contains invalid characters
    {

        //delete file here
        QFile delete_file (file_path);
        delete_file.remove();

        return QString("Invalid Characters");
    }
    else
    {
        // bootleg string compare method, if can fix please do - Jeremiah
        std::string test = cur_file.toStdString().substr(cur_file.toStdString().find_last_of(".")+1);

        if(test.compare("cpp") == 0)
        {


        }
        else if(test.compare("c") == 0)
        {

        }
        else
        {

            //delete file here
            QFile delete_file (file_path);
            delete_file.remove();

            return QString("Invalid file extension");
        }
    }

    return file_path;
}



QString File_Saver::save_file_as()
{
    QString filters("C++ file (*.cpp);;C file (*.c)");
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();

    file_path = QFileDialog::getSaveFileName(cur_wind,"Save your file",QDir::currentPath()
                                                 ,filters,&default_filter); // this opens the file directory window

    cur_wind->set_cur_filename(file_path);//set file name

    QFile file(file_path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        // any error involving reading of files i.e. system errors

        return QString("system error");
    }
    QTextStream out(&file);
    QString text = QString("");
    out << text;
    file.close();

    //checking for invalid characters set up
    QRegExp rx("[@!#$%^&*<>?/|}{~:]");
    std::size_t symbol = file_path.toStdString().find_last_of("/");
    std::string cur_dir = file_path.toStdString().substr(symbol+1);
    QString cur_file = QString::fromStdString(cur_dir);

    if(cur_file.contains(rx) == true) // if file name contains invalid characters
    {

        //delete file here
        QFile delete_file (file_path);
        delete_file.remove();

        return QString("Invalid Characters");
    }
    else
    {
        // bootleg string compare method, if can fix please do - Jeremiah
        std::string test = cur_file.toStdString().substr(cur_file.toStdString().find_last_of(".")+1);

        if(test.compare("cpp") == 0)
        {


        }
        else if(test.compare("c") == 0)
        {

        }
        else
        {

            //delete file here
            QFile delete_file (file_path);
            delete_file.remove();
            return QString("Invalid file extension");
        }
    }

    return file_path;

}

void File_Saver::save_file(QString filepath)
{
   QFile file(filepath);
   if(file.open(QFile::WriteOnly|QFile::Text))
   {
       // any error involving reading of files i.e. system errors
       QTextStream out(&file);
       QString text = cur_wind->ui->Code_Input_Textbox->toPlainText();
       out << text;
       file.close();
   }
   else
   {
       //this would mean there is a problem with saving the file hence this should be for error handling - jeremiah
       std::cout << "Problem with default save function" << std::endl;
   }


}

