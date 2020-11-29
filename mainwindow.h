#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include "./camera/simplecamera.h"
#include "./camera/fovCamera.h"
#include "./primitive/sphere.h"
#include "./primitive/rectangle.h"
#include "./primitive/model.h"
#include "./core/primitiveList.h"
#include "./core/scene.h"
#include<unordered_map>
#include "./debugger/ogldebugwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    static void renderParallel(std::unique_ptr<cameraBase> &cam,std::unique_ptr<primitiveList> &worldList,int xs,int xe,int ys,int ye,int nx,int ny,int ns,std::vector<glm::vec3> &pixels, std::vector<bool> &bools);
public slots:
    void render();
    void setShowDebugRay(bool flag);
    void showDebugWindow();
    void enableMultiThreads();
    void setColorMode();

private:
    void initPBRTResource();
    bool multiThreads=false;

    std::vector<float> debugVertices;
    std::vector<float> debugColors;

    enum colorMode
    {
        iterator,
        recursive,
        hitTest,
        normalVis,
        normalTest
    };
    colorMode mode{iterator};

    bool showDebugRay = false;
    std::vector<std::vector<float>> debugDatas;
    std::unique_ptr<oglDebugWindow> debugWindow = nullptr;
    Ui::MainWindow *ui;
    std::unique_ptr<QImage> img;
    std::unique_ptr<cameraBase> cam;
    std::shared_ptr<scene> scenes;
};
#endif // MAINWINDOW_H
