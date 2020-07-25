#include "file_reader.h"

//constructor
File_Reader::File_Reader()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
File_Reader::~File_Reader()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accessor
MainWindow* File_Reader::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void File_Reader::set_cur_wind(MainWindow *cur_wind)
{
    this->cur_wind = cur_wind;
}

//opening a file
QStringList File_Reader::open_file()
{
    QString filters("C++ file (*.cpp);;C file (*.c)");//file must be .c or .cpp
    QString default_filter("C++ file (*.cpp)");
    QString file_path = QString("");
    QStringList list;
	
	//use dialog box to open file
    file_path = QFileDialog::getOpenFileName(cur_wind,"Select file to open",QDir::currentPath()
                                             ,filters,&default_filter); // this opens the file directory window
											 
    cur_wind->set_cur_file_path(file_path);//set file name
    list << file_path;

    QFile file(file_path);//opening the file chosen
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

//ccheckign if the file is saved
QString File_Reader::file_check(QString file_path)
{
    QString file_text = ERROR_FILE_NOT_FOUND; // incase cannot open, by default it is set to "error"

    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QTextStream in(&file);
        QString text = in.readAll();
        file_text = text;//text from file must be equal to the one in the coding text box

    }

	file.close();
	return file_text;
}

//reading the compilation status
QString File_Reader::read_compile_status(QString file_path)
{
    QFile file(file_path);
    QString text = ERROR_INVALID_FILE;

    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QTextStream in(&file);
        text = in.readAll();//read file from the temp folder
        file.close();

        QString to_delete = QString("del \"") + file_path + QString("\"");
        to_delete.replace("/","\\"); // replaces ever occurence of / with \, for some @#$% reason, the file path dosent work with /
		
		//delete temp text file
        system(to_delete.toStdString().c_str());
    }
	
    file.close();
    return text;
}

QString File_Reader::read_file_into_vector(QString exe_file_path,std::vector<char>& buffer)
{
    // Create the fstream
    QFile file(exe_file_path);
    QString status = QString("");

    if(file.open(QIODevice::ReadOnly))
    {
        char data;

        while(!file.atEnd())
        {
            file.read(&data,sizeof(BYTE));//reading each byte and putting it into a buffer
            buffer.emplace_back(data);
        }
        status = SUCCESS_VALID_EXECUTABLE;
    }
    else
    {
        status = ERROR_FILE_NOT_FOUND;
    }

    file.close();
    return status;
}

qint64 File_Reader::get_executable_size(QString exe_file_path)
{
	QFileInfo info (exe_file_path);
    return info.size(); //if filepath not found, will return 0
}
