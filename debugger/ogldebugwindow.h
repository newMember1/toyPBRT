#ifndef OGLDEBUGWINDOW_H
#define OGLDEBUGWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <memory>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class oglDebugWindow : public QOpenGLWidget,
        protected QOpenGLFunctions
{
public:
    oglDebugWindow(std::vector<std::vector<float>> sceneDatas);
    void setRayData(std::vector<std::vector<float>> rayDatas);
    void setRayData(std::vector<float> v, std::vector<float> c);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * evnet) override;

private:
    bool refreshRayData = false;
    bool drawRays = false;

    QVector2D lastPos;

    void initScene();
    void createBuffer();
    void printContextInformation();

    void rotateX(float radius);
    void rotateY(float radius);
    void setShaderModelMatrix();

    int sVSize = 0;
    std::unique_ptr<QOpenGLShaderProgram> sceneProgram = nullptr;
    std::unique_ptr<QOpenGLBuffer> sceneVBO = nullptr;
    std::unique_ptr<QOpenGLVertexArrayObject> sceneVAO = nullptr;
    std::vector<float> sceneVertices;
    std::vector<float> sceneColors;

    int rVSize = 0;
    std::unique_ptr<QOpenGLShaderProgram> rayProgram = nullptr;
    std::unique_ptr<QOpenGLBuffer> rayVBO = nullptr;
    std::unique_ptr<QOpenGLVertexArrayObject> rayVAO = nullptr;
    std::vector<float> rayVertices;
    std::vector<float> rayColors;

    QMatrix4x4 model, view, projection;
};

#endif // OGLDEBUGWINDOW_H
