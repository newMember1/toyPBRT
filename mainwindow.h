#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QImage>
#include<QPixmap>
#include"./camera/simplecamera.h"
#include"./primitive/sphere.h"
#include"./core/primitiveList.h"
#include"./materials/disneybrdfmaterial.h"

#include<unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    void render();
private:
    void initPBRTResource();
    void initWorld();
    void loadTextures();
    void loadMaterials();
    void loadObjects();
    Ui::MainWindow *ui;
    std::unique_ptr<QImage> img;
    std::unique_ptr<cameraBase> cam;
    std::unique_ptr<primitiveList> worldList;

    std::unordered_map<std::string,std::shared_ptr<texture>> textures;
    std::unordered_map<std::string,std::shared_ptr<materialBase>> materials;
};

#endif // MAINWINDOW_H
