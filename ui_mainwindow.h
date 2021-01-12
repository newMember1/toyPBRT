/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionTraceRays;
    QAction *actionColorNormal;
    QAction *actionColorHit;
    QAction *actionColorNormalTest;
    QAction *actionColorNormalVis;
    QAction *actionColorIterator;
    QAction *actionColorRecursive;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *nxLabel;
    QSpinBox *spinBox_nx;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nyLabel;
    QSpinBox *spinBox_ny;
    QHBoxLayout *horizontalLayout;
    QLabel *nsLabel;
    QSpinBox *spinBox_ns;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *colorIter;
    QRadioButton *colorRec;
    QRadioButton *colorHit;
    QRadioButton *colorNorTest;
    QRadioButton *colorNorVis;
    QSpacerItem *verticalSpacer_2;
    QPushButton *renderBotton;
    QCheckBox *checkBox_multiThread;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QLabel *imageLabel;
    QMenuBar *menubar;
    QMenu *menuDebug;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(807, 606);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionTraceRays = new QAction(MainWindow);
        actionTraceRays->setObjectName(QString::fromUtf8("actionTraceRays"));
        actionTraceRays->setCheckable(true);
        actionColorNormal = new QAction(MainWindow);
        actionColorNormal->setObjectName(QString::fromUtf8("actionColorNormal"));
        actionColorHit = new QAction(MainWindow);
        actionColorHit->setObjectName(QString::fromUtf8("actionColorHit"));
        actionColorHit->setCheckable(true);
        actionColorHit->setEnabled(true);
        actionColorNormalTest = new QAction(MainWindow);
        actionColorNormalTest->setObjectName(QString::fromUtf8("actionColorNormalTest"));
        actionColorNormalVis = new QAction(MainWindow);
        actionColorNormalVis->setObjectName(QString::fromUtf8("actionColorNormalVis"));
        actionColorIterator = new QAction(MainWindow);
        actionColorIterator->setObjectName(QString::fromUtf8("actionColorIterator"));
        actionColorRecursive = new QAction(MainWindow);
        actionColorRecursive->setObjectName(QString::fromUtf8("actionColorRecursive"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setAutoFillBackground(false);
        horizontalLayout_5 = new QHBoxLayout(centralwidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        nxLabel = new QLabel(centralwidget);
        nxLabel->setObjectName(QString::fromUtf8("nxLabel"));

        horizontalLayout_3->addWidget(nxLabel);

        spinBox_nx = new QSpinBox(centralwidget);
        spinBox_nx->setObjectName(QString::fromUtf8("spinBox_nx"));
        spinBox_nx->setMinimum(100);
        spinBox_nx->setMaximum(4096);
        spinBox_nx->setValue(100);

        horizontalLayout_3->addWidget(spinBox_nx);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        nyLabel = new QLabel(centralwidget);
        nyLabel->setObjectName(QString::fromUtf8("nyLabel"));

        horizontalLayout_2->addWidget(nyLabel);

        spinBox_ny = new QSpinBox(centralwidget);
        spinBox_ny->setObjectName(QString::fromUtf8("spinBox_ny"));
        spinBox_ny->setMinimum(10);
        spinBox_ny->setMaximum(4096);
        spinBox_ny->setValue(100);

        horizontalLayout_2->addWidget(spinBox_ny);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        nsLabel = new QLabel(centralwidget);
        nsLabel->setObjectName(QString::fromUtf8("nsLabel"));

        horizontalLayout->addWidget(nsLabel);

        spinBox_ns = new QSpinBox(centralwidget);
        spinBox_ns->setObjectName(QString::fromUtf8("spinBox_ns"));
        spinBox_ns->setMinimum(1);
        spinBox_ns->setMaximum(10000);
        spinBox_ns->setValue(10);

        horizontalLayout->addWidget(spinBox_ns);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        colorIter = new QRadioButton(centralwidget);
        colorIter->setObjectName(QString::fromUtf8("colorIter"));
        colorIter->setCheckable(true);

        verticalLayout_2->addWidget(colorIter);

        colorRec = new QRadioButton(centralwidget);
        colorRec->setObjectName(QString::fromUtf8("colorRec"));

        verticalLayout_2->addWidget(colorRec);

        colorHit = new QRadioButton(centralwidget);
        colorHit->setObjectName(QString::fromUtf8("colorHit"));

        verticalLayout_2->addWidget(colorHit);

        colorNorTest = new QRadioButton(centralwidget);
        colorNorTest->setObjectName(QString::fromUtf8("colorNorTest"));

        verticalLayout_2->addWidget(colorNorTest);

        colorNorVis = new QRadioButton(centralwidget);
        colorNorVis->setObjectName(QString::fromUtf8("colorNorVis"));

        verticalLayout_2->addWidget(colorNorVis);


        verticalLayout->addLayout(verticalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        renderBotton = new QPushButton(centralwidget);
        renderBotton->setObjectName(QString::fromUtf8("renderBotton"));

        verticalLayout->addWidget(renderBotton);

        checkBox_multiThread = new QCheckBox(centralwidget);
        checkBox_multiThread->setObjectName(QString::fromUtf8("checkBox_multiThread"));
        checkBox_multiThread->setEnabled(true);

        verticalLayout->addWidget(checkBox_multiThread);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_4->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        horizontalLayout_4->setStretch(0, 5);
        horizontalLayout_4->setStretch(1, 1);

        horizontalLayout_5->addLayout(horizontalLayout_4);

        imageLabel = new QLabel(centralwidget);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        sizePolicy.setHeightForWidth(imageLabel->sizePolicy().hasHeightForWidth());
        imageLabel->setSizePolicy(sizePolicy);
        imageLabel->setMinimumSize(QSize(400, 400));
        imageLabel->setAutoFillBackground(true);
        imageLabel->setScaledContents(true);
        imageLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(imageLabel);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 2);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 807, 26));
        menuDebug = new QMenu(menubar);
        menuDebug->setObjectName(QString::fromUtf8("menuDebug"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuDebug->menuAction());
        menuDebug->addSeparator();
        menuDebug->addAction(actionTraceRays);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionTraceRays->setText(QApplication::translate("MainWindow", "TraceRays", nullptr));
        actionColorNormal->setText(QApplication::translate("MainWindow", "ColorNormal", nullptr));
        actionColorHit->setText(QApplication::translate("MainWindow", "ColorHitTest", nullptr));
        actionColorNormalTest->setText(QApplication::translate("MainWindow", "ColorNormalTest", nullptr));
        actionColorNormalVis->setText(QApplication::translate("MainWindow", "ColorNormalVis", nullptr));
        actionColorIterator->setText(QApplication::translate("MainWindow", "ColorIterator", nullptr));
        actionColorRecursive->setText(QApplication::translate("MainWindow", "ColorRecursive", nullptr));
        nxLabel->setText(QApplication::translate("MainWindow", "nx", nullptr));
        nyLabel->setText(QApplication::translate("MainWindow", "ny", nullptr));
        nsLabel->setText(QApplication::translate("MainWindow", "ns", nullptr));
        colorIter->setText(QApplication::translate("MainWindow", "colorIterator", nullptr));
        colorRec->setText(QApplication::translate("MainWindow", "colorRecursive", nullptr));
        colorHit->setText(QApplication::translate("MainWindow", "colorHitTest", nullptr));
        colorNorTest->setText(QApplication::translate("MainWindow", "colorNormalTest", nullptr));
        colorNorVis->setText(QApplication::translate("MainWindow", "colorNormalVis", nullptr));
        renderBotton->setText(QApplication::translate("MainWindow", "run", nullptr));
        checkBox_multiThread->setText(QApplication::translate("MainWindow", "multiThread", nullptr));
        imageLabel->setText(QString());
        menuDebug->setTitle(QApplication::translate("MainWindow", "Debug", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
