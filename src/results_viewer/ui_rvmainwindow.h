/********************************************************************************
** Form generated from reading UI file 'rvmainwindow.ui'
**
** Created: Wed May 22 14:49:29 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RVMAINWINDOW_H
#define UI_RVMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "rvfileview.h"

QT_BEGIN_NAMESPACE

class Ui_RVMainWindow
{
public:
    QAction *actionOpenExperiment;
    QAction *actionSaveExperiment;
    QAction *actionConfiguration;
    QAction *actionReconstruction;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    RVFileView *file_view;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuTools;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *RVMainWindow)
    {
        if (RVMainWindow->objectName().isEmpty())
            RVMainWindow->setObjectName(QString::fromUtf8("RVMainWindow"));
        RVMainWindow->resize(602, 317);
        actionOpenExperiment = new QAction(RVMainWindow);
        actionOpenExperiment->setObjectName(QString::fromUtf8("actionOpenExperiment"));
        actionSaveExperiment = new QAction(RVMainWindow);
        actionSaveExperiment->setObjectName(QString::fromUtf8("actionSaveExperiment"));
        actionConfiguration = new QAction(RVMainWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        actionReconstruction = new QAction(RVMainWindow);
        actionReconstruction->setObjectName(QString::fromUtf8("actionReconstruction"));
        centralwidget = new QWidget(RVMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 45, 602, 253));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 2, 0, 0);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        frame = new QFrame(splitter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(3);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        file_view = new RVFileView(frame);
        file_view->setObjectName(QString::fromUtf8("file_view"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(file_view->sizePolicy().hasHeightForWidth());
        file_view->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(file_view);

        splitter->addWidget(frame);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setGeometry(QRect(0, 0, 479, 224));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setGeometry(QRect(0, 0, 479, 224));
        tabWidget->addTab(tab_2, QString());
        splitter->addWidget(tabWidget);

        horizontalLayout->addWidget(splitter);

        RVMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RVMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 602, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        RVMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RVMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 298, 602, 19));
        RVMainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(RVMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setGeometry(QRect(0, 21, 602, 24));
        RVMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuTools->menuAction());
        menuFile->addAction(actionOpenExperiment);
        menuFile->addAction(actionSaveExperiment);
        menuTools->addAction(actionConfiguration);
        toolBar->addAction(actionReconstruction);

        retranslateUi(RVMainWindow);

        QMetaObject::connectSlotsByName(RVMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RVMainWindow)
    {
        RVMainWindow->setWindowTitle(QApplication::translate("RVMainWindow", "SequenceTree - Results Viewer", 0, QApplication::UnicodeUTF8));
        actionOpenExperiment->setText(QApplication::translate("RVMainWindow", "Open Experiment...", 0, QApplication::UnicodeUTF8));
        actionSaveExperiment->setText(QApplication::translate("RVMainWindow", "Save Experiment...", 0, QApplication::UnicodeUTF8));
        actionConfiguration->setText(QApplication::translate("RVMainWindow", "Configuration...", 0, QApplication::UnicodeUTF8));
        actionReconstruction->setText(QApplication::translate("RVMainWindow", "Reconstruction", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("RVMainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("RVMainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("RVMainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("RVMainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("RVMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RVMainWindow: public Ui_RVMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RVMAINWINDOW_H
