#include "file_reader.h"

//constructor
File_Reader::File_Reader()
{
    this->cur_wind = nullptr;
}

File_Reader::File_Reader(MainWindow *mw_ptr)
{
    this->cur_wind = mw_ptr;
}

//destructor
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


    cur_wind->set_cur_file_path(file_path);//set file name
    list << file_path;

    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QTextStream in(&file);
        QString text = in.readAll();
        list << text;
    }
    else
    {
        //problem with opening of file
        list.replace(0,ERROR_NO_FILE_NAME_SPECIFIED);
        // "Problem with opening file inside file reader";
        list << file.errorString();
    }

    file.close();
    return list;
}

QString File_Reader::file_check(QString file_path)
{

    QString file_text = ERROR_FILE_NOT_FOUND; // incase cannot open, by default it is set to "error"

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

QString File_Reader::read_compile_status(QString file_path)
{
    QFile file(file_path);
    QString text = ERROR_INVALID_FILE;

    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QTextStream in(&file);
        text = in.readAll();
        file.close();

        QString to_delete = QString("del \"") + file_path + QString("\"");
        to_delete.replace("/","\\"); // replaces ever occurence of / with \, for some @#$% reason, the file path dosent work with /

        system(to_delete.toStdString().c_str());
        return text;
    }

    return text;
}



