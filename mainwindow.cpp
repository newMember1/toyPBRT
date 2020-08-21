#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "./core/pdfGenerate.h"
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
    this->worldList.reset(new primitiveList());
    initWorld();

    //connect signal
    connect(ui->renderBotton,&QPushButton::clicked,this,&MainWindow::render);
    connect(ui->radioButton_multiThread,&QRadioButton::clicked,this,&MainWindow::enableMultiThreads);
}

void MainWindow::initWorld()
{
    std::cout<<"initWorld."<<std::endl;
    loadTextures();
    loadMaterials();
    loadObjects();
}

void MainWindow::loadTextures()
{
    std::cout<<"loadTextures."<<std::endl;

    auto red = std::make_shared<constTexture>(glm::vec3(1.0f, 0, 0));
    auto redBase = std::dynamic_pointer_cast<texture>(std::move(red));
    textures["redTex"] = redBase;

    auto green = std::make_shared<constTexture>(glm::vec3(0, 1.0f, 0));
    auto greenBase = std::dynamic_pointer_cast<texture>(std::move(green));
    textures["greenTex"] = greenBase;

    auto blue = std::make_shared<constTexture>(glm::vec3(0, 0, 1.0f));
    auto blueBase = std::dynamic_pointer_cast<texture>(std::move(blue));
    textures["blueTex"] = blueBase;

    auto gold = std::make_shared<constTexture>(glm::vec3(1.022f,0.782f,0.344f));
    auto goldBase = std::dynamic_pointer_cast<texture>(std::move(gold));
    textures["goldTex"] = goldBase;

    auto white = std::make_shared<constTexture>(glm::vec3(1.0f));
    auto whiteBase = std::dynamic_pointer_cast<texture>(std::move(white));
    textures["whiteTex"] = whiteBase;

    std::cout<<"loadTextures done."<<std::endl<<std::endl;
}

void MainWindow::loadMaterials()
{
    std::cout<<"loadMaterials."<<std::endl;

    auto red = std::make_shared<disneyBRDFMaterial>(textures["redTex"]);
    auto redBase = std::dynamic_pointer_cast<materialBase>(std::move(red));
    materials["redMat"] = redBase;

    auto green = std::make_shared<disneyBRDFMaterial>(textures["greenTex"]);
    auto greenBase = std::dynamic_pointer_cast<materialBase>(std::move(green));
    materials["greenMat"] = greenBase;

    auto blue = std::make_shared<disneyBRDFMaterial>(textures["blueTex"]);
    auto blueBase = std::dynamic_pointer_cast<materialBase>(std::move(blue));
    materials["blueMat"] = blueBase;

    auto gold = std::make_shared<disneyBRDFMaterial>(textures["goldTex"]);
    auto goldBase = std::dynamic_pointer_cast<materialBase>(std::move(gold));
    materials["goldMat"] = goldBase;

    auto whiteLight = std::make_shared<disneyBRDFMaterial>(textures["whiteTex"]);
    auto whiteLightBase = std::dynamic_pointer_cast<materialBase>(std::move(whiteLight));
    whiteLightBase->isLight = true;
    materials["whiteLightMat"] = whiteLightBase;

    std::cout<<"loadMaterials done."<<std::endl<<std::endl;
}

void MainWindow::loadNonLightObjects()
{
    auto deer = std::make_shared<model>("/home/zdxiao/Desktop/models/deer/deer.obj", materials["goldMat"]);
    auto deerBase = std::dynamic_pointer_cast<primitiveBase>(std::move(deer));
    objects["deer"] = deerBase;
}

void MainWindow::loadLightObjects()
{
    auto planeLight = std::make_shared<rectangle>(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2, 2, materials["whiteLightMat"]);
    auto planeLightBase = std::dynamic_pointer_cast<primitiveBase>(std::move(planeLight));
    objects["planeLight"] = planeLightBase;

    pdfGenerate::getInstance().setLightPdf(planeLight);
}

void MainWindow::configObjectPosition()
{
    objects["deer"]->setRotate(glm::vec3(0, 1, 0), 120);
    objects["deer"]->handleMatrix();
}

void MainWindow::loadObjects()
{
    std::cout<<"loadObjects."<<std::endl;
    loadNonLightObjects();
    loadLightObjects();
    configObjectPosition();
    worldList.reset(new primitiveList(objects));
    std::cout<<"loadObjects done."<<std::endl<<std::endl;
}

void MainWindow::enableMultiThreads()
{
    if(this->ui->radioButton_multiThread->isChecked())
        this->multiThreads=true;
    else
        this->multiThreads=false;
}

void MainWindow::render()
{
    //config enviorment
    int nx=ui->spinBox_nx->value();
    int ny=ui->spinBox_ny->value();
    int ns=ui->spinBox_ns->value();

    cam.reset(new simpleCamera(glm::vec3(0,0,3),glm::vec3(0,0,1),nx,ny));
    img.reset(new QImage(nx,ny,QImage::Format_RGB32));
    ray r(glm::vec3(0.0f),glm::vec3(1.0f));

    std::vector<glm::vec3> pixels;
    pixels.reserve(nx*ny);
    time_t s,e;
    time(&s);

    const int numThread=std::thread::hardware_concurrency();
    if(this->multiThreads)
    {
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
                                    std::ref(this->worldList),
                                    xs,xe,ys,ye,nx,ny,ns,
                                    std::ref(pixels)));
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
                if(i==50&&j==97)
                    debugFlag=true;
                else
                    debugFlag=false;

                glm::vec3 c(0);
                for(int k=0;k<ns;++k)
                {
                    cam->emitRay(i,j,r);
                    c+=glm::clamp(worldList->colorIterator(r,10),glm::vec3(0),glm::vec3(1));
                }
                c/=ns;
                c*=255.99;
                glm::clamp(c, glm::vec3(0), glm::vec3(255));
                pixels[j+i*ny]=c;
            }
        }
    }
    time(&e);
    std::cout<<"rendering time is: "<<(double)(e-s)<<"'s"<<std::endl;

    std::ofstream outFile("/home/zdxiao/Desktop/test.ppm");
    outFile<<"P3"<<std::endl<<nx<<" "<<ny<<std::endl<<255<<std::endl;
    for(int j=0;j<ny;++j)
    {
        for(int i=0;i<nx;++i)
        {
            const glm::vec3 &c=pixels[(ny-1-j)+(nx-1-i)*ny];
            img->setPixel(i,j,qRgb(c.x,c.y,c.z));
            outFile<<static_cast<int>(c.x)<<"  "<<static_cast<int>(c.y)<<"  "<<static_cast<int>(c.z)<<std::endl;
        }
    }
    outFile.close();

    QPixmap pixmap(QPixmap::fromImage(*img));
    int w=ui->imageLabel->width();
    int h=ui->imageLabel->height();
    pixmap = pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(pixmap);
}

void MainWindow::renderParallel(std::unique_ptr<cameraBase> &cam, std::unique_ptr<primitiveList> &worldList, int xs, int xe, int ys, int ye, int nx, int ny, int ns, std::vector<glm::vec3> &pixels)
{
    ray r(glm::vec3(0.0f),glm::vec3(1.0f));
    for(int i=xs;i<xe;++i)
    {
        for(int j=ys;j<ye;++j)
        {
            glm::vec3 c(0);
            for(int k=0;k<ns;++k)
            {
                cam->emitRay(i,j,r);
                c+=glm::clamp(worldList->colorIterator(r,10),glm::vec3(0),glm::vec3(1));
            }
            c/=ns;
            c*=255.99;
            glm::clamp(c, glm::vec3(0), glm::vec3(255));
            pixels[j+i*ny]=c;
        }
    }
}
