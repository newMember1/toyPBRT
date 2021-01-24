#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "./core/directionGenerator.h"
#include "./pdf/cosinePdf.h"
#include "./pdf/rectLightPdf.h"
#include "./materials/disneybrdfmaterial.h"
#include "./materials/simplematerial.h"
#include "./testScenes/cornellbox.h"
#include "./testScenes/glassBunny.h"
#include "./testScenes/disneyMatSpheres.h"
#include "./testScenes/refractcheck.h"

bool debugFlag=false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initPBRTResource();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initPBRTResource()
{
    std::cout<<"initPBRTResource."<<std::endl;
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(199,237,204));
    ui->imageLabel->setAutoFillBackground(true);
    ui->imageLabel->setPalette((palette));

    //initialize data
    this->cam=nullptr;
    this->img=nullptr;

    //config scene
    //auto cornellBoxObjects = cornellBox::getInstance().getAllObjects();
    //auto cornellBoxBunny = glassBunny::getInstance().getAllObjects();
    auto sphereObjects = disneyMatSpheres::getInstance().getAllObjects();
    //auto refractCheckObjects = refractCheck::getInstance().getAllObjects();
    this->scenes.reset(new scene(sphereObjects));

    //connect signal
    connect(ui->renderBotton, &QPushButton::clicked, this, &MainWindow::render);
    connect(ui->checkBox_multiThread, &QCheckBox::clicked, this, &MainWindow::enableMultiThreads);
    connect(ui->actionTraceRays, SIGNAL(triggered(bool)), this, SLOT(setShowDebugRay(bool)));
    connect(ui->colorIter, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorRec, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorHit, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorNorVis, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorNorTest, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
}

void MainWindow::enableMultiThreads()
{
    if(this->ui->checkBox_multiThread->isChecked())
        this->multiThreads=true;
    else
        this->multiThreads=false;
}

void MainWindow::setColorMode()
{
    if(ui->colorIter->isChecked())
        mode = iterator;
    else if(ui->colorRec->isChecked())
        mode = recursive;
    else if(ui->colorHit->isChecked())
        mode = hitTest;
    else if(ui->colorNorVis->isChecked())
        mode = normalVis;
    else if(ui->colorNorTest->isChecked())
        mode = normalTest;
    else
        std::cout<<"no color mode selected..."<<std::endl;
}

void MainWindow::render()
{
    //config enviorment
    int nx=ui->spinBox_nx->value();
    int ny=ui->spinBox_ny->value();
    int ns=ui->spinBox_ns->value();

    cam.reset(new fovCamera(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), 40.0, 1.0, nx, ny));
    cam.reset(new fovCamera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 45, 1, nx, ny));
    img.reset(new QImage(nx,ny,QImage::Format_RGB32));
    ray r(glm::vec3(0.0f),glm::vec3(1.0f));

    std::vector<glm::vec3> pixels;
    pixels.resize(nx*ny);
    std::vector<bool> bools(nx * ny, false);

    time_t s,e;
    time(&s);

    /* note
     * nx means row number in image
     * ny means column number in image
     * ppm file store datas row by row
    */

    /*
     * when use camera
     * remember that i and j increse from left bottom to right top
     */

    scenes->getLists()->setMode(mode);
    const int numThread=std::thread::hardware_concurrency();
    if(this->multiThreads)
    {
        std::cout<<"multiThreads can't be used in lightPdf's calculation..."<<std::endl;
        std::cout<<"multiThreads rendering..."<<std::endl;
        int taskPerThread;
        if(nx%(numThread-1) == 0)
            taskPerThread=nx/(numThread-1);
        else
            taskPerThread=nx/(numThread-1)+1;

        std::vector<std::thread> tasks;
        tasks.reserve(numThread-1);
        for(int i=0;i<numThread-1;++i)
        {
            int xs,xe;
            int ys,ye;
            xs=taskPerThread*i;
            xe=std::min(taskPerThread*(i+1),nx);
            ys=0;
            ye=ny;
            tasks.push_back(
                        std::thread(MainWindow::renderParallel,
                                    std::ref(this->cam),
                                    std::ref(this->scenes->getLists()),
                                    xs,xe,ys,ye,nx,ny,ns,
                                    std::ref(pixels),
                                    std::ref(bools)));
        }
        for(int i=0;i<numThread-1;++i)
            tasks[i].join();
    }
    else
    {
        std::cout<<"rendering..."<<std::endl;
        for(int i=0;i<nx;++i)
        {
            for(int j=0;j<ny;++j)
            {
                glm::vec3 c(0);
                for(int k = 0; k < ns; ++k)
                {
                    /*if(i == 50 && j == 50 && k == 0)
                        debugFlag=true; 
                    else
                        debugFlag=false;*/
                    cam->emitRay(j,i,r);
					if (abs(r.direc.x + 0.186332) < 0.00001 && abs(r.direc.y + 0.220472) < 0.00001 && abs(r.direc.z + 0.95743) < 0.00001)
						debugFlag = true;
					else
						debugFlag = false;
                    glm::vec3 tmp = scenes->getLists()->color(r,0);
					tmp = tmp / (tmp + vec3(1.0f));
					tmp = pow(tmp, vec3(1.0f / 2.2));
					c += tmp;
                }
                c /= ns;
                c = glm::clamp(c, glm::vec3(0), glm::vec3(1.0f));
                c *= 255.99;

                pixels[j + (nx - 1 - i) * ny] = c;
            }
        }
    }

    time(&e);
    std::cout<<"rendering time is: "<<(double)(e-s)<<"'s"<<std::endl;

    //ppm file store datas by row
    std::ofstream outFile("/home/zdxiao/Desktop/test.ppm");
    outFile<<"P3"<<std::endl<<ny<<" "<<nx<<std::endl<<255<<std::endl;
    for(int i=0;i<nx;++i)
    {
        for(int j=0;j<ny;++j)
        {
            const glm::vec3 &c=pixels[j + i * ny];
            img->setPixel(j, i, qRgb(c.x, c.y, c.z));
            outFile<<static_cast<int>(c.x)<<"  "<<static_cast<int>(c.y)<<"  "<<static_cast<int>(c.z)<<std::endl;
        }
    }
    outFile.close();

    QPixmap pixmap(QPixmap::fromImage(*img));
    int w=ui->imageLabel->width();
    int h=ui->imageLabel->height();
    pixmap = pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(pixmap);

    //display debug window
    if(showDebugRay == true)
        showDebugWindow();
}

void MainWindow::setShowDebugRay(bool flag)
{
    showDebugRay = flag;
    showDebugWindow();
}

void MainWindow::showDebugWindow()
{
    auto datas = scenes->getSceneFrameInfo();
    if(debugWindow == nullptr)
        debugWindow.reset(new oglDebugWindow(datas));
    auto vertices = scenes->getLists()->getVertices();
    auto colorDatas = scenes->getLists()->getColors();
    if(vertices.size() > 0 && colorDatas.size() > 0)
    {
        debugWindow->setRayData(vertices, colorDatas);
    }
    debugWindow->show();
}

void MainWindow::renderParallel(std::unique_ptr<cameraBase> &cam, std::unique_ptr<primitiveList> &worldList, int xs, int xe, int ys, int ye, int nx, int ny, int ns, std::vector<glm::vec3> &pixels, std::vector<bool> &bools)
{
    ray r(glm::vec3(0.0f),glm::vec3(1.0f));

    for(int i = xs; i < xe; ++i)
    {
        for(int j = ys; j < ye; ++j)
        {
            glm::vec3 c(0);
            for(int k = 0;k < ns; ++k)
            {
                cam->emitRay(j,i,r);
                c += worldList->color(r,0);
            }
            c /= ns;
            c = sqrt(c);
            c = glm::clamp(c, glm::vec3(0), glm::vec3(1.0f));
            c *= 255.99;
            pixels[j + (nx - 1 - i) * ny] = c;
            bools[j + (nx - 1 - i) * ny] = true;
        }
    }
}
