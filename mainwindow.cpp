#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

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
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::gray);
    ui->imageLabel->setAutoFillBackground(true);
    ui->imageLabel->setPalette((palette));

    //connect signal
    connect(ui->renderBotton,&QPushButton::clicked,this,&MainWindow::render);
}

void MainWindow::render()
{
    //config enviorment

    std::cout<<"render..."<<std::endl;
}
