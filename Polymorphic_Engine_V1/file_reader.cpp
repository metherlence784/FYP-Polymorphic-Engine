#include "file_reader.h"



File_Reader::File_Reader()
{
 this->cur_wind = nullptr;
}

File_Reader::File_Reader(MainWindow *mw_ptr)
{
    this->cur_wind = mw_ptr;
}

File_Reader::~File_Reader()
{
    this->cur_wind = nullptr;
   delete this->cur_wind;
}

QStringList File_Reader::open_file()
{
    QString filters("C++ file (*.cpp);;C file (*.c)");
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();
    QStringList list;

    file_path = QFileDialog::getOpenFileName(cur_wind,"Select file to open",QDir::currentPath()
                                             ,filters,&default_filter); // this opens the file directory window


    cur_wind->set_cur_filename(file_path);//set file name
    list << file_path;


    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QTextStream in(&file);
        QString text = in.readAll();
        list << text;
        file.close();
    }
    else
    {
        //problem with opening of file
        list.replace(0,QString("error"));
        std::cout << "Problem with opening file inside file reader" << std::endl;
        list << file.errorString();
    }

    return list;
}


QString File_Reader::file_check(QString file_path)
{

    QString file_text = QString("error"); // incase cannot open, by default it is set to "error"

    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QTextStream in(&file);
        QString text = in.readAll();
        file_text = text;
        file.close();
        return file_text;
    }
    else
    {
        // error handling goes here
        return file_text;
    }

}



