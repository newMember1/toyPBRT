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
#include "./core/scenelists.h"
#include "./debugger/ogldebugwindow.h"
#include "./3rdparty/thread_pool/ThreadPool.h"
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
	static void pixelRender(std::unique_ptr<cameraBase> & cam, std::unique_ptr<primitiveList> & scenes, std::vector<glm::vec3> & pixels, int nx, int ny, int nk, int i, int j);
    static void blockRender(std::unique_ptr<cameraBase> &cam,std::unique_ptr<primitiveList> &worldList,int xs,int xe,int ys,int ye,int nx,int ny,int ns,std::vector<glm::vec3> &pixels);

public slots:
	void rendering();
    void setShowDebugRay(bool flag);
    void showDebugWindow();
    void enableMultiThreads();
	void enableFog();
    void setColorMode();

private:
    void initPBRTResource();
	void singleThreadRender();
	void multiThreadsRender();
	void gammaCorrection();
	void writeToPPM();
	void writeToLabel();
    bool multiThreads = false;
	int nx, ny, nk;

    std::vector<float> debugVertices;
    std::vector<float> debugColors;

    colorMode mode = colorMode::iterator;

	//for debug mode
	bool fog = false;
    bool showDebugRay = false;
    std::vector<std::vector<float>> debugDatas;
    std::unique_ptr<oglDebugWindow> debugWindow = nullptr;
    Ui::MainWindow *ui;
    std::unique_ptr<QImage> img;
    std::unique_ptr<cameraBase> cam;
    std::shared_ptr<sceneLists> scenes;
	std::vector<glm::vec3> pixels;

	//for multi thread rendering, we should at least n cams, n directionGenerator for each thread
	std::unique_ptr<primitiveList[]> listScenes;
	std::unique_ptr<directionPdfAdaptor[]> direcAdaptors;
	std::unique_ptr<ThreadPool> pool = nullptr;
};
#endif // MAINWINDOW_H
