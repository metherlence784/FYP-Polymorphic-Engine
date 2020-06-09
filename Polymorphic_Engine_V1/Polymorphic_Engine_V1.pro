QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choose_payload_controller.cpp \
    create_new_file_controller.cpp \
    exit_application_controller.cpp \
    file_reader.cpp \
    file_saver.cpp \
    main.cpp \
    mainwindow.cpp \
    open_existing_file_controller.cpp \
    redo_action_controller.cpp \
    save_as_new_file_controller.cpp \
    save_current_file_controller.cpp \
    undo_action_controller.cpp \
    write_code_in_c_cpp_controller.cpp

HEADERS += \
    choose_payload_controller.h \
    create_new_file_controller.h \
    exit_application_controller.h \
    file_reader.h \
    file_saver.h \
    mainwindow.h \
    open_existing_file_controller.h \
    redo_action_controller.h \
    save_as_new_file_controller.h \
    save_current_file_controller.h \
    undo_action_controller.h \
    write_code_in_c_cpp_controller.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
