#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<thread>

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
    std::cout<<"initPBRTResource..."<<std::endl;
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
    std::cout<<"initWorld..."<<std::endl;
    loadTextures();
    loadMaterials();
    loadObjects();
}

void MainWindow::loadTextures()
{
    std::cout<<"loadTextures..."<<std::endl;
    auto cTex=std::make_shared<constTexture>(glm::vec3(0.7f,0.8f,0.9f));
    auto tex=std::dynamic_pointer_cast<texture>(std::move(cTex));
    textures["constMat"]=tex;

    auto lTex=std::make_shared<constTexture>(glm::vec3(0.8f));
    auto whiteLight=std::dynamic_pointer_cast<texture>(std::move(lTex));
    textures["whiteLight"]=whiteLight;
    std::cout<<"loadTextures done..."<<std::endl<<std::endl;
}

void MainWindow::loadMaterials()
{
    std::cout<<"loadMaterials..."<<std::endl;
    auto disMat=std::make_shared<disneyBRDFMaterial>(textures["constMat"]);
    auto mat=std::dynamic_pointer_cast<materialBase>(std::move(disMat));
    materials["disneyMat"]=mat;

    auto lightMat=std::make_shared<disneyBRDFMaterial>(textures["whiteLight"]);
    auto whiteLight=std::dynamic_pointer_cast<materialBase>(std::move(lightMat));
    whiteLight->isLight=true;
    materials["whiteLight"]=whiteLight;
    std::cout<<"loadMaterials done..."<<std::endl<<std::endl;
}

void MainWindow::loadObjects()
{
    std::cout<<"loadObjects..."<<std::endl;
    std::vector<std::shared_ptr<primitiveBase>> allObjects;
//    auto sphereBall=std::make_shared<sphere>(1.0f,glm::vec3(0.0f,0.0f,-1.0f),materials["disneyMat"]);
//    auto ball=std::dynamic_pointer_cast<primitiveBase>(std::move(sphereBall));
//    allObjects.push_back(std::move(ball));

//    auto lightBall=std::make_shared<sphere>(100,glm::vec3(0.0f,0.0f,105.f),materials["whiteLight"]);
//    auto light=std::dynamic_pointer_cast<primitiveBase>(std::move(lightBall));
//    allObjects.push_back(std::move(light));

    auto gun=std::make_shared<model>("/home/zdxiao/Desktop/models/dragon/Dragon/Dragon.obj",materials["disneyMat"]);
    auto gunBase=std::dynamic_pointer_cast<primitiveBase>(std::move(gun));
    allObjects.push_back(gunBase);
    worldList.reset(new primitiveList(allObjects));
    std::cout<<"loadObjects done..."<<std::endl<<std::endl;
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

    cam.reset(new simpleCamera(glm::vec3(2.5,0,3),glm::vec3(1,0,1),nx,ny));
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
        if(nx%(numThread-1))
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
                if(i==nx*0.4&&j==ny*0.4)
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
                pixels[j+i*ny]=c;
            }
        }
    }
    time(&e);
    std::cout<<"rendering time is: "<<(double)(e-s)<<"'s"<<std::endl;

    std::ofstream outFile("/home/zdxiao/Desktop/test.ppm");
    outFile<<"P3"<<std::endl<<nx<<" "<<ny<<std::endl<<255<<std::endl;
    for(int i=0;i<nx;++i)
    {
        for(int j=0;j<ny;++j)
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
            pixels[j+i*ny]=c;
        }
    }
}
