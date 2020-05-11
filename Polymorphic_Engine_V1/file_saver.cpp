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


QString File_Saver::save_file()
{
    QString filters("C++ file (*.cpp);;C file (*.c)");
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();
    QStringList name_filter;
    name_filter << "C++ file (*.cpp)" << "C file (*.c)";



    file_path = QFileDialog::getSaveFileName(cur_wind,"Save your file",QDir::currentPath()
                                                 ,filters,&default_filter);

    cur_wind->set_cur_filename(file_path);//set file name
    std::cout << file_path.toStdString() << std::endl;



    QFile file(file_path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        std::cout << file.errorString().toStdString() << std::endl;
        return QString("system error");
    }
    QTextStream out(&file);
    QString text = QString("");
    out << text;
    file.close();

    QRegExp rx("[@!#$%^&*<>?/|}{~:]");
    std::size_t symbol = file_path.toStdString().find_last_of("/");
    std::string cur_dir = file_path.toStdString().substr(symbol+1);

    QString cur_file = QString::fromStdString(cur_dir);

    std::cout << "TESTING: " << cur_file.toStdString() << std::endl;


    if(cur_file.contains(rx) == true)
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
        std::cout << test << " " << test.length()<< std::endl;


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



void File_Saver::save_file_as(QString file_path)
{


}

