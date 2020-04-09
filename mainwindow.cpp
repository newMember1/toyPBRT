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
    palette.setColor(QPalette::Background, Qt::yellow);
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
    auto sphereBall=std::make_shared<sphere>(1.0f,glm::vec3(0.0f,0.0f,-1.0f),materials["disneyMat"]);
    auto ball=std::dynamic_pointer_cast<primitiveBase>(std::move(sphereBall));
    allObjects.push_back(std::move(ball));

    auto lightBall=std::make_shared<sphere>(100,glm::vec3(0.0f,0.0f,105.f),materials["whiteLight"]);
    auto light=std::dynamic_pointer_cast<primitiveBase>(std::move(lightBall));
    allObjects.push_back(std::move(light));

    worldList.reset(new primitiveList(allObjects));
    std::cout<<"loadObjects done..."<<std::endl<<std::endl;
}

void MainWindow::render()
{
    //config enviorment
    std::cout<<"render start..."<<std::endl;
    clock_t s,e;
    s=clock();
    int nx=ui->spinBox_nx->value();
    int ny=ui->spinBox_ny->value();
    int ns=ui->spinBox_ns->value();

    cam.reset(new simpleCamera(glm::vec3(0,0,3),1,nx,ny));
    img.reset(new QImage(nx,ny,QImage::Format_RGB32));
    ray r(glm::vec3(0.0f),glm::vec3(1.0f));

    for(int i=0;i<nx;++i)
    {
        for(int j=ny-1;j>=0;--j)
        {
            if(i==nx*0.5&&j==ny*0.5)
                debugFlag=true;
            else
                debugFlag=false;
            glm::vec3 c(0);
            for(int k=0;k<ns;++k)
            {
                cam->emitRay(i,j,r);
                c+=worldList->color(r,0);
            }
            c/=ns;
            c*=255.99;
            img->setPixel(i,j,qRgb(c.x,c.y,c.z));
        }
    }

    e=clock();
    std::cout<<"rendering time is: "<<(double)(e-s)/CLOCKS_PER_SEC<<"'s"<<std::endl;
    std::ofstream outFile("./renderPics/test.ppm");
    outFile<<"p3"<<std::endl<<nx<<" "<<ny<<std::endl<<255<<std::endl;

    QPixmap pixmap(QPixmap::fromImage(*img));
    int w=ui->imageLabel->width();
    int h=ui->imageLabel->height();
    pixmap = pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(pixmap);
}
