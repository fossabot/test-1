#-------------------------------------------------
#
# Project created by QtCreator 2014-06-25T11:33:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CODE
TEMPLATE = app

########################################
target.path = /home/root/$${TARGET}
INSTALLS += target
########################################

SOURCES += main.cpp\
        dialog.cpp \
    gpiocontrol.cpp \
    configadapter.cpp \
    detect_fuse.cpp \
    dialogsampconf.cpp \
    my_label.cpp \
    my_speaker.cpp

HEADERS  += dialog.h \
    gpiocontrol.h \
    configadapter.h \
    detect_fuse.h \
    dialogsampconf.h \
    my_label.h \
    my_speaker.h

FORMS    += dialog.ui \
    dialogsampconf.ui

#------------------------------------------------------------------------#
INCLUDEPATH += /usr/local/include/
#LIBS += -L/usr/local/lib
#LIBS += -lopencv_core
#LIBS += -lopencv_imgproc
#LIBS += -lopencv_highgui
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
#LIBS += -lopencv_features2d
#LIBS += -lopencv_calib3d
#LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann
#LIBS += -lopencv_nonfree


#INCLUDEPATH += -L`pkg-config --cflags opencv`
#LIBS += -L`pkg-config --libs opencv`




unix:!macx: LIBS += -L$$PWD/../../../OpenCV-2.4.1/build/lib/ -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ts -lopencv_video

INCLUDEPATH += $$PWD/../../../OpenCV-2.4.1/build/include
DEPENDPATH += $$PWD/../../../OpenCV-2.4.1/build/include
