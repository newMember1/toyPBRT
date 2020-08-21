QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console debug
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /home/zdxiao/Desktop/GLM

SOURCES += \
    accelerate/bvh.cpp \
    core/pdfGenerate.cpp \
    core/primitiveList.cpp \
    main.cpp \
    mainwindow.cpp \
    materials/disneybrdfmaterial.cpp \
    modelLoader/objloader.cpp \
    primitive/model.cpp \
    primitive/rectangle.cpp \
    primitive/sphere.cpp \
    primitive/triangle.cpp

HEADERS += \
    3rdparty/tiny_obj_loader.h \
    accelerate/bvh.h \
    camera/simplecamera.h \
    collision/aabb.h \
    core/baseStructure.h \
    core/cameraBase.h \
    core/materialBase.h \
    core/onb.h \
    core/pdfBase.h \
    core/pdfGenerate.h \
    core/primitiveBase.h \
    core/primitiveList.h \
    core/texture.h \
    mainwindow.h \
    materials/disneybrdfmaterial.h \
    modelLoader/objloader.h \
    models/tiny_obj_loader.h \
    pdf/cosinePdf.h \
    pdf/rectLightPdf.h \
    primitive/model.h \
    primitive/rectangle.h \
    primitive/sphere.h \
    primitive/triangle.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
