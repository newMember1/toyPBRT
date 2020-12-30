#include "ogldebugwindow.h"
#include <QMouseEvent>

oglDebugWindow::oglDebugWindow(std::vector<std::vector<float>> infos)
{
    qDebug()<<"oglDebugWindow...";
    sceneVertices = infos[0];
    sceneColors = infos[1];
    sVSize = sceneVertices.size();
    sceneVertices.insert(sceneVertices.end(), sceneColors.begin(), sceneColors.end());
}

void oglDebugWindow::setRayData(std::vector<std::vector<float>> rayDatas)
{
    rayVertices = rayDatas[0];
    rayColors = rayDatas[1];
    rVSize = rayVertices.size();
    rayVertices.insert(rayVertices.end(), rayColors.begin(), rayColors.end());
    refreshRayData = true;
}

void oglDebugWindow::setRayData(std::vector<float> v, std::vector<float> c)
{
    rayVertices = v;
    rayColors = c;
    rVSize = rayVertices.size();
    rayVertices.insert(rayVertices.end(), rayColors.begin(), rayColors.end());
    refreshRayData = true;
}

void oglDebugWindow::initScene()
{
    model.scale(QVector3D(0.002, 0.002, 0.002));

    projection.perspective(20.0f, this->width() / this->height(), 0.1f, 100.0f);
    view.lookAt(QVector3D(0, 0, 10), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    //ray display shader
    rayProgram.reset(new QOpenGLShaderProgram());
    rayProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/raySimple.vert");
    rayProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/raySimple.frag");
    rayProgram->link();
    rayProgram->bind();

    rayProgram->setUniformValue(rayProgram->uniformLocation("model"), model);
    rayProgram->setUniformValue(rayProgram->uniformLocation("view"), view);
    rayProgram->setUniformValue(rayProgram->uniformLocation("projection"), projection);
    rayProgram->release();

    sceneProgram.reset(new QOpenGLShaderProgram());
    sceneProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/simple.vert");
    sceneProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/simple.frag");
    sceneProgram->link();
    sceneProgram->bind();

    sceneProgram->setUniformValue(sceneProgram->uniformLocation("model"), model);
    sceneProgram->setUniformValue(sceneProgram->uniformLocation("view"), view);
    sceneProgram->setUniformValue(sceneProgram->uniformLocation("projection"), projection);
    sceneProgram->release();

    sceneVAO.reset(new QOpenGLVertexArrayObject());
    sceneVAO->create();
    sceneVAO->bind();

    sceneVBO.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    sceneVBO->create();
    sceneVBO->bind();
    sceneVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    sceneVBO->allocate(&sceneVertices[0], sizeof(float) * sceneVertices.size());

    //shader attributes
    sceneProgram->bind();
    sceneProgram->enableAttributeArray(0);
    sceneProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
    sceneProgram->enableAttributeArray(1);
    sceneProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * sVSize, 3, sizeof(float) * 3);

    sceneVBO->release();
    sceneVAO->release();
}

void oglDebugWindow::initializeGL()
{
    qDebug()<<"initializeGL...";
    initializeOpenGLFunctions();
    printContextInformation();

    initScene();

    glClearColor(0.5f, 0.3f, 0.0f, 1.0f);
}

void oglDebugWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, width() / 2, height() / 2);
}

void oglDebugWindow::mousePressEvent(QMouseEvent *event)
{
    lastPos.setX(event->x());
    lastPos.setY(event->y());
}

void oglDebugWindow::rotateX(float radius)
{
    //handle model rotate in x direction
    //rotate around X
    QVector3D X{1, 0, 0};
    QMatrix4x4 temp;
    temp.rotate(radius, X);
    model = temp * model;
}

void oglDebugWindow::rotateY(float radius)
{
    //handle model rotate in y direction
    //rotate around Y
    QVector3D Y{0, 1, 0};
    QMatrix4x4 temp;
    temp.rotate(radius, Y);
    model = temp * model;
}

void oglDebugWindow::zoom(float delta)
{
    delta = delta / 200;
    QMatrix4x4 temp;
    QVector3D scale{1 + delta, 1 + delta, 1 + delta};
    temp.scale(scale);
    model = temp * model;
}

void oglDebugWindow::setShaderModelMatrix()
{
    sceneProgram->bind();
    sceneProgram->setUniformValue(sceneProgram->uniformLocation("model"), model);
    rayProgram->bind();
    rayProgram->setUniformValue(rayProgram->uniformLocation("model"), model);
    update();
}

void oglDebugWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    lastPos = QVector2D(event->pos());

    //process mouseEvent
    rotateX(dy / 5.0);
    rotateY(dx / 5.0);
}

void oglDebugWindow::wheelEvent(QWheelEvent *event)
{
    zoom(event->delta());
}
void oglDebugWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(refreshRayData)
    {
        rayVAO.reset(new QOpenGLVertexArrayObject());
        rayVAO->create();
        rayVAO->bind();

        rayVBO.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
        rayVBO->create();
        rayVBO->bind();
        rayVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
        rayVBO->allocate(&rayVertices[0], sizeof(float) * rVSize);

        rayProgram->enableAttributeArray(0);
        rayProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
        rayProgram->enableAttributeArray(1);
        rayProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * rVSize, 3, sizeof(float) * 3);

        rayProgram->release();
        rayVBO->release();
        rayVAO->release();
        drawRays = true;
    }

    //drawScene
    sceneVAO->bind();
    sceneProgram->bind();
    setShaderModelMatrix();
    glDrawArrays(GL_LINES, 0, sVSize / 3);
    sceneProgram->release();
    sceneVAO->release();

    if(drawRays)
    {
        rayVAO->bind();
        rayProgram->bind();
        glDrawArrays(GL_LINE_STRIP, 0, rVSize / 3);
        rayProgram->release();
        rayVAO->release();
    }
}

void oglDebugWindow::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    glType=(context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion=reinterpret_cast<const char *>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch(format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
