#include "exit_application_controller.h"

//constructor
Exit_Application_Controller::Exit_Application_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
}

//destructor
Exit_Application_Controller::~Exit_Application_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;
}

//accesor
MainWindow * Exit_Application_Controller::get_cur_wind()
{
    return this->cur_wind;
}

//mutator
void Exit_Application_Controller::set_cur_wind(MainWindow *cur)
{
    this->cur_wind = cur;
}

//exit function
void Exit_Application_Controller::exit_application()
{
	//first checking if current progress is saved
    File_Reader reader;
    QString file_text = reader.file_check(cur_wind->get_cur_file_path());

    Write_Code_In_C_CPP_Controller writer;
    QString current_text = writer.get_text();
    if(cur_wind->get_cur_file_path().isEmpty())
    {
       // "The user opened the application and straight away closed it" ;
        QApplication::quit();
    }
    else if(file_text == ERROR_FILE_NOT_FOUND)
    {
        //error 404, file not found
        QMessageBox::warning(cur_wind, "Warning",
                             "Exit error: Misplaced/Missing file\n Please save the file properly" );
    }
    else if (file_text == current_text)
    {
        //never save, give pop up
        // "Content is up to date"
        QApplication::quit();
    }
    else if (file_text != current_text)
    {
        //it dosent match, aka user needs to save
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(cur_wind, "Exit",
                                      "You have unsaved progress!\nAre you sure you want to quit?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)//button to click yes or cancel
        {
            std::cout << "Yes was clicked";
            QApplication::quit();
        }
        else
        {
            std::cout << "Yes was *not* clicked";
        }
    }
}

//same as above function but this is meant for the X button of the window
void Exit_Application_Controller::exit_application(QCloseEvent *event)
{
	//first checking if current progress is saved
    File_Reader reader;
    QString file_text = reader.file_check(cur_wind->get_cur_file_path());

    Write_Code_In_C_CPP_Controller writer;
    QString current_text = writer.get_text();
    if(cur_wind->get_cur_file_path().isEmpty())
    {
        std::cout << "The user opened the application and straight away closed it" ;
        event->accept();
    }
    else if(file_text == ERROR_FILE_NOT_FOUND)
    {
        //error
        QMessageBox::warning(cur_wind, "Warning",
                             "Exit error: Misplaced/Missing file\n Please save the file properly" );
        event->ignore();
    }
    else if (file_text == current_text)
    {
        //never save, give pop up
        std::cout << "Content is up to date" ;
        event->accept();

    }
    else if (file_text != current_text)
    {
        //it dosent match, aka user needs to save
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(cur_wind, "Exit",
                                      "You have unsaved progress!\nAre you sure you want to quit?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            std::cout << "Yes was clicked";
            event->accept();
        }
        else
        {
            std::cout << "Yes was *not* clicked";
            event->ignore();
        }
    }
}
