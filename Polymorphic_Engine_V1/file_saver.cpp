#include "file_saver.h"

#include <iostream>

//constructor
File_Saver::File_Saver()
{
    this->cur_wind = nullptr;
}

File_Saver::File_Saver(MainWindow *mw_ptr)
{
    this->cur_wind = mw_ptr;
}

//destructor
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

    QFile file(file_path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        // any error involving reading of files i.e. system errors
        return ERROR_SYSTEM_FAULT;
    }
    QTextStream out(&file);
    QString text = QString("");
    out << text;
    file.close();

    //checking for invalid characters set up
    QRegExp rx("[@!\'\"#$%^&<>?/|}{~:]");
    std::size_t symbol = file_path.toStdString().find_last_of("/");
    std::string to_load_file = file_path.toStdString().substr(symbol+1);
    QString cur_file = QString::fromStdString(to_load_file);

    if(cur_file.contains(rx) == true) // if file name contains invalid characters
    {

        //delete file here
        QFile delete_file (file_path);
        delete_file.remove();

        return ERROR_INVALID_CHARACTERS;
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

            return ERROR_INVALID_FILE_EXTENSION;
        }
    }

    return file_path;
}

QString File_Saver::save_file_as(QString text)
{
    QString filters("C++ file (*.cpp);;C file (*.c)");
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();

    file_path = QFileDialog::getSaveFileName(cur_wind,"Save your file",QDir::currentPath()
                                             ,filters,&default_filter); // this opens the file directory window


    QFile file(file_path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        // any error involving reading of files i.e. system errors
        return ERROR_SYSTEM_FAULT;
    }

    QTextStream out(&file);
    out << text;
    file.close();

    //checking for invalid characters set up
    QRegExp rx("[@!\'\"#$%^&<>?/|}{~:]");
    std::size_t symbol = file_path.toStdString().find_last_of("/");
    std::string cur_dir = file_path.toStdString().substr(symbol+1);
    QString cur_file = QString::fromStdString(cur_dir);

    if(cur_file.contains(rx) == true) // if file name contains invalid characters
    {

        //delete file here
        QFile delete_file (file_path);
        delete_file.remove();

        return ERROR_INVALID_CHARACTERS;
    }
    else
    {
        // bootleg string compare method, if can fix please do - Jeremiah
        std::string test = cur_file.toStdString().substr(cur_file.toStdString().find_last_of(".")+1);

        if(test.compare("cpp") == 0)
        {
            //if it is cpp, it is valid
        }
        else if(test.compare("c") == 0)
        {
            // if it is c it is valid
        }
        else
        {
            //delete file here
            QFile delete_file (file_path);
            delete_file.remove();
            return ERROR_INVALID_FILE_EXTENSION;
        }
    }

    return file_path;

}

void File_Saver::save_file(QString file_path, QString text)
{
    QFile file(file_path);
    if(file.open(QFile::WriteOnly|QFile::Text))//this means file stream is opened successfully
    {
        QTextStream out(&file);
        out << text;
        file.close();
    }
    else
    {
        // any error involving reading of files i.e. system errors
        //this would mean there is a problem with saving the file hence this should be for error handling - jeremiah
        std::cout << ERROR_SYSTEM_FAULT.toStdString() << std::endl;
    }
}

