#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<fstream>
#include<sstream>

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
    std::cout<<"rendering..."<<std::endl;
    clock_t s,e;
    s=clock();
    for(int i=0;i<nx;++i)
    {
        for(int j=ny-1;j>=0;--j)
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
            pixels.push_back(c);
        }
    }
    e=clock();

    std::cout<<"write to ppm file..."<<std::endl;
    std::ofstream outFile("/home/zdxiao/Desktop/test.ppm");
    outFile<<"P3"<<std::endl<<nx<<" "<<ny<<std::endl<<255<<std::endl;
    for(int i=0;i<nx;++i)
    {
        for(int j=ny-1;j>=0;--j)
        {
            const glm::vec3 &c=pixels[(ny-1-j)+i*ny];
            img->setPixel(i,j,qRgb(c.x,c.y,c.z));
            outFile<<static_cast<int>(c.x)<<"  "<<static_cast<int>(c.y)<<"  "<<static_cast<int>(c.z)<<std::endl;
        }
    }
    outFile.close();
    std::cout<<"write file done..."<<std::endl;

    std::cout<<"rendering time is: "<<(double)(e-s)/CLOCKS_PER_SEC<<"'s"<<std::endl;

    QPixmap pixmap(QPixmap::fromImage(*img));
    int w=ui->imageLabel->width();
    int h=ui->imageLabel->height();
    pixmap = pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(pixmap);
}
