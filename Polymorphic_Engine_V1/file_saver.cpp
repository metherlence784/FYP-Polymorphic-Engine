#include "file_saver.h"


//constructor
File_Saver::File_Saver()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
File_Saver::~File_Saver()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accessor
MainWindow* File_Saver::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void File_Saver::set_cur_wind(MainWindow* cur_wind)
{
    this->cur_wind = cur_wind;
}

//saving a new file
QString File_Saver::new_file()
{
    QString filters("C++ file (*.cpp);;C file (*.c)");//msut be saved in .c or .cpp
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();
	
	//use dialog to save file
    file_path = QFileDialog::getSaveFileName(get_cur_wind(),"Save your file",QDir::currentPath()
                                             ,filters,&default_filter); // this opens the file directory window

    QFile file(file_path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        // any error involving reading of files i.e. system errors
        return ERROR_SYSTEM_FAULT;
    }
	
    QTextStream out(&file);
    QString text = QString("");
    out << text;//store empty string to file for safety
    file.close();

    //checking for invalid characters set up
    QRegExp rx("[@!\'\"#$%^&<>?/|}{~:]"); //file name cannot contain these chars
    std::size_t symbol = file_path.toStdString().find_last_of("/");//finding the file name using substring
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
			//if .cpp do nothing
        }
        else if(test.compare("c") == 0)
        {
			//if .c do nothing
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

//saving file as
QString File_Saver::save_file_as(QString text)
{
	//similar to the above function but user can rename the file
    QString filters("C++ file (*.cpp);;C file (*.c)");
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString();

    file_path = QFileDialog::getSaveFileName(get_cur_wind(),"Save your file",QDir::currentPath()
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

//for quick save
void File_Saver::save_file(QString file_path, QString text)
{
    QFile file(file_path);
    if(file.open(QFile::WriteOnly|QFile::Text))//this means file stream is opened successfully
    {
        QTextStream out(&file);
        out << text;
    }
    else
    {
        // any error involving reading of files i.e. system errors
        //this would mean there is a problem with saving the file hence this should be for error handling - jeremiah
        std::cout << ERROR_SYSTEM_FAULT.toStdString() << std::endl;
    }
    file.close();
}

//writing a exe file
void File_Saver::write_exe_file(QString morphed_exe_file_path, std::vector<char> &buffer)
{
    QFile file(morphed_exe_file_path);
    if(file.open(QFile::WriteOnly))//this means file stream is opened successfully
    {
        QDataStream out(&file);
        out.writeRawData(&buffer[0],buffer.size());//put the whoel buffer into the exe file
    }
    else
    {
        // any error involving reading of files i.e. system errors
        //this would mean there is a problem with saving the file hence this should be for error handling - jeremiah
        std::cout << ERROR_SYSTEM_FAULT.toStdString() << std::endl;
    }
    file.close();
}

