QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O3

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
    3rdparty/stb_image.cpp \
    accelerate/bvh.cpp \
    core/directionGenerator.cpp \
    core/primitiveList.cpp \
    core/scene.cpp \
    debugger/ogldebugwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    materials/dielectricMaterial.cpp \
    materials/disneybrdfmaterial.cpp \
    materials/simplematerial.cpp \
    modelLoader/objloader.cpp \
    primitive/cube.cpp \
    primitive/model.cpp \
    primitive/rectangle.cpp \
    primitive/sphere.cpp \
    primitive/triangle.cpp \
    testScenes/cornellbox.cpp \
    testScenes/glassBunny.cpp

HEADERS += \
    3rdparty/stb_image.h \
    3rdparty/tiny_obj_loader.h \
    accelerate/bvh.h \
    camera/fovCamera.h \
    camera/simplecamera.h \
    collision/aabb.h \
    core/baseStructure.h \
    core/cameraBase.h \
    core/directionGenerator.h \
    core/hitRecord.h \
    core/materialBase.h \
    core/onb.h \
    core/pdfBase.h \
    core/primitiveBase.h \
    core/primitiveList.h \
    core/scene.h \
    core/texture.h \
    debugger/ogldebugwindow.h \
    debugger/raytracer.h \
    mainwindow.h \
    materials/dielectricMaterial.h \
    materials/disneybrdfmaterial.h \
    materials/simplematerial.h \
    modelLoader/objloader.h \
    models/tiny_obj_loader.h \
    pdf/cosinePdf.h \
    pdf/randomPdf.h \
    pdf/rectLightPdf.h \
    primitive/cube.h \
    primitive/model.h \
    primitive/rectangle.h \
    primitive/sphere.h \
    primitive/triangle.h \
    testScenes/cornellbox.h \
    testScenes/glassBunny.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/raySimple.frag \
    shaders/raySimple.vert \
    shaders/simple.frag \
    shaders/simple.vert

RESOURCES += \
    resource.qrc
