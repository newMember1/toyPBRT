#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "./core/directionPdfAdaptor.h"
#include "./pdf/cosinePdf.h"
#include "./pdf/rectLightPdf.h"
#include "./materials/disneybrdfmaterial.h"
#include "./materials/simplematerial.h"
#include "./testScenes/cornellbox.h"
#include "./testScenes/glassBunny.h"
#include "./testScenes/basicMatSpheres.h"
#include "./testScenes/disneymatobjects.h"
#include "./testScenes/refractcheck.h"
#include "./testScenes/imagebasedlight.h"

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

    //connect signal
    connect(ui->renderBotton, &QPushButton::clicked, this, &MainWindow::rendering);
    connect(ui->checkBox_multiThread, &QCheckBox::clicked, this, &MainWindow::enableMultiThreads);
    connect(ui->checkBox_fog, &QCheckBox::clicked, this, &MainWindow::enableFog);
    connect(ui->actionTraceRays, SIGNAL(triggered(bool)), this, SLOT(setShowDebugRay(bool)));
    connect(ui->colorIter, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorRec, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorHit, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorNorVis, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->colorNorTest, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));
    connect(ui->test, SIGNAL(clicked(bool)), this, SLOT(setColorMode()));

    //initialize data
    this->cam=nullptr;
    this->img=nullptr;

    //config scene
    //auto cornellBoxObjects = cornellBox::getInstance().getAllObjects();
    //auto cornellBoxBunny = glassBunny::getInstance().getAllObjects();
    //auto sphereObjects = basicMatSpheres::getInstance().getAllObjects();
    //auto disneyMatBunny = disneyMatObjects::getInstance().getAllObjects();
    //auto refractCheckObjects = refractCheck::getInstance().getAllObjects();
    auto lightScenes = std::make_shared<imageBasedLight>();
	lightScenes->initResources();
    auto objects = lightScenes->getAllObjects();

    this->scenes.reset(new sceneLists(objects));
}

void MainWindow::enableMultiThreads()
{
    if(this->ui->checkBox_multiThread->isChecked())
        this->multiThreads=true;
    else
        this->multiThreads=false;
}

void MainWindow::enableFog()
{
	if (this->ui->checkBox_fog->isChecked())
		this->fog = true;
	else
		this->fog = false;
}

void MainWindow::setColorMode()
{
    if(ui->colorIter->isChecked())
        mode = colorMode::iterator;
    else if(ui->colorRec->isChecked())
        mode = colorMode::recursive;
    else if(ui->colorHit->isChecked())
        mode = colorMode::hitTest;
    else if(ui->colorNorVis->isChecked())
        mode = colorMode::normalVis;
    else if(ui->colorNorTest->isChecked())
        mode = colorMode::normalTest;
    else if(ui->test->isChecked())
        mode = colorMode::test;
    std::cout<<"no color mode selected..."<<std::endl;
}

/* note
	 * nx means row number in image
	 * ny means column number in image
	 * ppm file store datas row by row
*/
void MainWindow::rendering()
{
	//config enviorment
	nx = ui->spinBox_nx->value();
	ny = ui->spinBox_ny->value();
	nk = ui->spinBox_ns->value();

	cam.reset(new fovCamera(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), 40.0, 1.0, nx, ny));
	cam.reset(new fovCamera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 45, 1, nx, ny));
	cam.reset(new fovCamera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -50), 45, 1, nx, ny));
	img.reset(new QImage(nx, ny, QImage::Format_RGB32));
	pixels.resize(nx*ny);

	scenes->getLists()->setMode(colorMode(mode));
	scenes->getLists()->setFog(fog);

	if (multiThreads)
		this->multiThreadsRender();
	else
		this->singleThreadRender();

	gammaCorrection();
	writeToLabel();
	writeToPPM();

	//display debug window
	if (showDebugRay == true)
		showDebugWindow();
}

void MainWindow::singleThreadRender()
{
    std::cout<<"single thread render start."<<std::endl;
    time_t s,e;
    time(&s);

	ray r{glm::vec3(0.0f), glm::vec3(1.0f)};
	for (int i = 0; i < nx; ++i)
	{
		for (int j = 0; j < ny; ++j)
		{
			glm::vec3 c(0);
			for (int k = 0; k < nk; ++k)
			{
				if (i == 48 && j == 50 && k == 0)
					debugFlag = true;
				else
					debugFlag = false;
				cam->emitRay(j, i, r);
				c += scenes->getLists()->color(r, 0);
			}
			c /= nk;
			pixels[j + (nx - 1 - i) * ny] = c;
		}
	}

    time(&e);
	std::cout << "single thread render end with time: " << (double)(e - s) << "s" << std::endl;
}

void MainWindow::multiThreadsRender()
{
	std::cout << "multi thread render start." << std::endl;
	time_t s, e;
	time(&s);

	const int numThreads = std::thread::hardware_concurrency();
	if (pool == nullptr)
	{
		//init threads pool
		pool.reset(new ThreadPool(numThreads));
		pool->init();
	}

	for (int i = 0; i < nx; ++i)
	{
		for (int j = 0; j < ny; ++j)
		{
			auto future = pool->submit(MainWindow::pixelRender, std::ref(cam), std::ref(scenes->getLists()), std::ref(pixels), nx, ny, nk, i, j);
			future.get();
		}
	}

	time(&e);
	std::cout << "multi thread render end with time: " << (double)(e - s) << "s" << std::endl;
	//pool->shutdown();
}

void MainWindow::gammaCorrection()
{
	for (int i = 0; i < nx; ++i)
	{
		for (int j = 0; j < ny; ++j)
		{
			auto c = pixels[j + (nx - 1 - i) * ny];
			c = c / (c + glm::vec3(1.0f));
			c = glm::pow(c, glm::vec3(1.0f / 2.2));
			c = glm::clamp(c, glm::vec3(0), glm::vec3(1.0f));
			c *= 255.99;
			pixels[j + (nx - 1 - i) * ny] = c;
		}
	}
}

void MainWindow::writeToPPM()
{
	std::cout << "write to ppm image start." << std::endl;
	//ppm file store datas by row
	std::ofstream outFile("/home/zdxiao/Desktop/test.ppm");
	outFile << "P3" << std::endl << ny << " " << nx << std::endl << 255 << std::endl;
	for (int i = 0; i < nx; ++i)
	{
		for (int j = 0; j < ny; ++j)
		{
			const glm::vec3 &c = pixels[j + i * ny];
			outFile << static_cast<int>(c.x) << "  " << static_cast<int>(c.y) << "  " << static_cast<int>(c.z) << std::endl;
		}
	}
	outFile.close();
	std::cout << "write to ppm image end." << std::endl;
}

void MainWindow::writeToLabel()
{
	std::cout << "write to image label start." << std::endl;
	for (int i = 0; i < nx; ++i)
	{
		for (int j = 0; j < ny; ++j)
		{
			auto c = pixels[j + i * ny];
			img->setPixel(j, i, qRgb(c.x, c.y, c.z));
		}
	}

	QPixmap pixmap(QPixmap::fromImage(*img));
	int w = ui->imageLabel->width();
	int h = ui->imageLabel->height();
	pixmap = pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->imageLabel->setPixmap(pixmap);
	std::cout << "write to image label end." << std::endl;
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

void MainWindow::pixelRender(std::unique_ptr<cameraBase> & cam, std::unique_ptr<primitiveList> & priLists, std::vector<glm::vec3> & pixels, int nx, int ny, int nk, int i, int j)
{
	ray r(glm::vec3(0.0f), glm::vec3(1.0f));
	glm::vec3 c(0);
	for (int k = 0; k < nk; ++k)
	{
		cam->emitRay(j, i, r);
		c += priLists->color(r, 0);
	}
	c /= nk;
	pixels[j + (nx - 1 - i) * ny] = c;
}

void MainWindow::blockRender(std::unique_ptr<cameraBase> &cam, std::unique_ptr<primitiveList> &worldList, int xs, int xe, int ys, int ye, int nx, int ny, int ns, std::vector<glm::vec3> &pixels, std::vector<bool> &bools)
{
	ray r(glm::vec3(0.0f), glm::vec3(1.0f));

	for (int i = xs; i < xe; ++i)
	{
		for (int j = ys; j < ye; ++j)
		{
			glm::vec3 c(0);
			for (int k = 0; k < ns; ++k)
			{
				cam->emitRay(j, i, r);
				c += worldList->color(r, 0);
			}
			c /= ns;
			pixels[j + (nx - 1 - i) * ny] = c;
			bools[j + (nx - 1 - i) * ny] = true;
		}
	}
}