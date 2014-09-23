/********************************************************************************
** Form generated from reading UI file 'stmainwindow.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STMAINWINDOW_H
#define UI_STMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_STMainWindow
{
public:
    QAction *actionNewSequence;
    QAction *actionOpenSequence;
    QAction *actionSaveSequence;
    QAction *actionCloseSequence;
    QAction *actionSaveSequenceAs;
    QAction *actionCompile;
    QAction *actionCopyNode;
    QAction *actionPasteNode;
    QAction *actionConfiguration;
    QAction *actionAboutSequenceTree;
    QAction *actionSequenceTreeWiki;
    QAction *actionExportToScanner;
    QAction *actionOpenSequenceFromWeb;
    QAction *actionUpdateCodeFromWeb;
    QAction *actionReadRawData;
    QAction *actionUploadSequenceToWeb;
    QAction *actionExportToVirtualScanner;
    QAction *actionChainLink;
    QAction *actionUpdateExportToScanner;
    QAction *actionMRPulseTool;
    QAction *actionImportRFPulseWaveform;
    QAction *actionAddResource;
    QAction *actionPreferences;
    QAction *actionCreateParameterFile;
    QAction *actionRTST;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuCompile;
    QMenu *menuEdit;
    QMenu *menuTools;
    QMenu *menuHelp;
    QMenu *menuResources;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *STMainWindow)
    {
        if (STMainWindow->objectName().isEmpty())
            STMainWindow->setObjectName(QString::fromUtf8("STMainWindow"));
        STMainWindow->resize(562, 316);
        STMainWindow->setIconSize(QSize(36, 24));
        actionNewSequence = new QAction(STMainWindow);
        actionNewSequence->setObjectName(QString::fromUtf8("actionNewSequence"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewSequence->setIcon(icon);
        actionOpenSequence = new QAction(STMainWindow);
        actionOpenSequence->setObjectName(QString::fromUtf8("actionOpenSequence"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenSequence->setIcon(icon1);
        actionSaveSequence = new QAction(STMainWindow);
        actionSaveSequence->setObjectName(QString::fromUtf8("actionSaveSequence"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveSequence->setIcon(icon2);
        actionCloseSequence = new QAction(STMainWindow);
        actionCloseSequence->setObjectName(QString::fromUtf8("actionCloseSequence"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        actionCloseSequence->setIcon(icon3);
        actionSaveSequenceAs = new QAction(STMainWindow);
        actionSaveSequenceAs->setObjectName(QString::fromUtf8("actionSaveSequenceAs"));
        actionCompile = new QAction(STMainWindow);
        actionCompile->setObjectName(QString::fromUtf8("actionCompile"));
        actionCopyNode = new QAction(STMainWindow);
        actionCopyNode->setObjectName(QString::fromUtf8("actionCopyNode"));
        actionPasteNode = new QAction(STMainWindow);
        actionPasteNode->setObjectName(QString::fromUtf8("actionPasteNode"));
        actionConfiguration = new QAction(STMainWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        actionAboutSequenceTree = new QAction(STMainWindow);
        actionAboutSequenceTree->setObjectName(QString::fromUtf8("actionAboutSequenceTree"));
        actionSequenceTreeWiki = new QAction(STMainWindow);
        actionSequenceTreeWiki->setObjectName(QString::fromUtf8("actionSequenceTreeWiki"));
        actionExportToScanner = new QAction(STMainWindow);
        actionExportToScanner->setObjectName(QString::fromUtf8("actionExportToScanner"));
        actionOpenSequenceFromWeb = new QAction(STMainWindow);
        actionOpenSequenceFromWeb->setObjectName(QString::fromUtf8("actionOpenSequenceFromWeb"));
        actionUpdateCodeFromWeb = new QAction(STMainWindow);
        actionUpdateCodeFromWeb->setObjectName(QString::fromUtf8("actionUpdateCodeFromWeb"));
        actionReadRawData = new QAction(STMainWindow);
        actionReadRawData->setObjectName(QString::fromUtf8("actionReadRawData"));
        actionUploadSequenceToWeb = new QAction(STMainWindow);
        actionUploadSequenceToWeb->setObjectName(QString::fromUtf8("actionUploadSequenceToWeb"));
        actionExportToVirtualScanner = new QAction(STMainWindow);
        actionExportToVirtualScanner->setObjectName(QString::fromUtf8("actionExportToVirtualScanner"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/vscanner.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportToVirtualScanner->setIcon(icon4);
        actionChainLink = new QAction(STMainWindow);
        actionChainLink->setObjectName(QString::fromUtf8("actionChainLink"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/chainlink.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionChainLink->setIcon(icon5);
        actionUpdateExportToScanner = new QAction(STMainWindow);
        actionUpdateExportToScanner->setObjectName(QString::fromUtf8("actionUpdateExportToScanner"));
        actionMRPulseTool = new QAction(STMainWindow);
        actionMRPulseTool->setObjectName(QString::fromUtf8("actionMRPulseTool"));
        actionImportRFPulseWaveform = new QAction(STMainWindow);
        actionImportRFPulseWaveform->setObjectName(QString::fromUtf8("actionImportRFPulseWaveform"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/sinc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImportRFPulseWaveform->setIcon(icon6);
        actionAddResource = new QAction(STMainWindow);
        actionAddResource->setObjectName(QString::fromUtf8("actionAddResource"));
        actionPreferences = new QAction(STMainWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionCreateParameterFile = new QAction(STMainWindow);
        actionCreateParameterFile->setObjectName(QString::fromUtf8("actionCreateParameterFile"));
        actionRTST = new QAction(STMainWindow);
        actionRTST->setObjectName(QString::fromUtf8("actionRTST"));
        centralwidget = new QWidget(STMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        hboxLayout = new QHBoxLayout(centralwidget);
        hboxLayout->setSpacing(0);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        hboxLayout->addWidget(tabWidget);

        STMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(STMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 562, 27));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuCompile = new QMenu(menubar);
        menuCompile->setObjectName(QString::fromUtf8("menuCompile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuResources = new QMenu(menubar);
        menuResources->setObjectName(QString::fromUtf8("menuResources"));
        STMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(STMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        STMainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(STMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        STMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuCompile->menuAction());
        menubar->addAction(menuResources->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNewSequence);
        menuFile->addAction(actionOpenSequence);
        menuFile->addAction(actionOpenSequenceFromWeb);
        menuFile->addAction(actionCloseSequence);
        menuFile->addSeparator();
        menuFile->addAction(actionSaveSequence);
        menuFile->addAction(actionSaveSequenceAs);
        menuFile->addSeparator();
        menuFile->addAction(actionUploadSequenceToWeb);
        menuCompile->addAction(actionCompile);
        menuEdit->addAction(actionCopyNode);
        menuEdit->addAction(actionPasteNode);
        menuTools->addAction(actionConfiguration);
        menuTools->addAction(actionExportToScanner);
        menuTools->addAction(actionUpdateExportToScanner);
        menuTools->addAction(actionExportToVirtualScanner);
        menuTools->addAction(actionReadRawData);
        menuTools->addAction(actionCreateParameterFile);
        menuTools->addSeparator();
        menuTools->addAction(actionUpdateCodeFromWeb);
        menuTools->addSeparator();
        menuTools->addAction(actionChainLink);
        menuTools->addAction(actionRTST);
        menuTools->addAction(actionMRPulseTool);
        menuTools->addSeparator();
        menuTools->addAction(actionPreferences);
        menuHelp->addAction(actionAboutSequenceTree);
        menuHelp->addAction(actionSequenceTreeWiki);
        menuResources->addAction(actionAddResource);
        menuResources->addAction(actionImportRFPulseWaveform);
        toolBar->addAction(actionNewSequence);
        toolBar->addAction(actionOpenSequence);
        toolBar->addAction(actionSaveSequence);
        toolBar->addSeparator();
        toolBar->addAction(actionCompile);
        toolBar->addSeparator();
        toolBar->addAction(actionChainLink);
        toolBar->addAction(actionRTST);
        toolBar->addAction(actionMRPulseTool);
        toolBar->addAction(actionImportRFPulseWaveform);
        toolBar->addAction(actionExportToVirtualScanner);

        retranslateUi(STMainWindow);

        QMetaObject::connectSlotsByName(STMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *STMainWindow)
    {
        STMainWindow->setWindowTitle(QApplication::translate("STMainWindow", "SequenceTree 4", 0, QApplication::UnicodeUTF8));
        actionNewSequence->setText(QApplication::translate("STMainWindow", "New Sequence", 0, QApplication::UnicodeUTF8));
        actionOpenSequence->setText(QApplication::translate("STMainWindow", "Open Sequence...", 0, QApplication::UnicodeUTF8));
        actionSaveSequence->setText(QApplication::translate("STMainWindow", "Save Sequence", 0, QApplication::UnicodeUTF8));
        actionCloseSequence->setText(QApplication::translate("STMainWindow", "Close Sequence", 0, QApplication::UnicodeUTF8));
        actionSaveSequenceAs->setText(QApplication::translate("STMainWindow", "Save Sequence As...", 0, QApplication::UnicodeUTF8));
        actionCompile->setText(QApplication::translate("STMainWindow", "Compile", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCompile->setToolTip(QApplication::translate("STMainWindow", "Compile the pulse sequence", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCopyNode->setText(QApplication::translate("STMainWindow", "Copy node", 0, QApplication::UnicodeUTF8));
        actionPasteNode->setText(QApplication::translate("STMainWindow", "Paste node...", 0, QApplication::UnicodeUTF8));
        actionConfiguration->setText(QApplication::translate("STMainWindow", "Compiler Configuration...", 0, QApplication::UnicodeUTF8));
        actionAboutSequenceTree->setText(QApplication::translate("STMainWindow", "About SequenceTree4...", 0, QApplication::UnicodeUTF8));
        actionSequenceTreeWiki->setText(QApplication::translate("STMainWindow", "SequenceTree Wiki...", 0, QApplication::UnicodeUTF8));
        actionExportToScanner->setText(QApplication::translate("STMainWindow", "Export to Scanner...", 0, QApplication::UnicodeUTF8));
        actionOpenSequenceFromWeb->setText(QApplication::translate("STMainWindow", "Open Sequence from Web...", 0, QApplication::UnicodeUTF8));
        actionUpdateCodeFromWeb->setText(QApplication::translate("STMainWindow", "Update code from web...", 0, QApplication::UnicodeUTF8));
        actionReadRawData->setText(QApplication::translate("STMainWindow", "Read raw data...", 0, QApplication::UnicodeUTF8));
        actionUploadSequenceToWeb->setText(QApplication::translate("STMainWindow", "Upload Sequence to Web...", 0, QApplication::UnicodeUTF8));
        actionExportToVirtualScanner->setText(QApplication::translate("STMainWindow", "Export to Virtual Scanner...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExportToVirtualScanner->setToolTip(QApplication::translate("STMainWindow", "Export pulse sequence to Virtual Scanner", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionChainLink->setText(QApplication::translate("STMainWindow", "ChainLink...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionChainLink->setToolTip(QApplication::translate("STMainWindow", "Launch ChainLink - data processing GUI", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionUpdateExportToScanner->setText(QApplication::translate("STMainWindow", "Enable/Update Export to Scanner...", 0, QApplication::UnicodeUTF8));
        actionMRPulseTool->setText(QApplication::translate("STMainWindow", "MR Pulse Tool...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionMRPulseTool->setToolTip(QApplication::translate("STMainWindow", "Open MR Pulse Tool - RF pulse design program", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionImportRFPulseWaveform->setText(QApplication::translate("STMainWindow", "Import RF Pulse Waveform...", 0, QApplication::UnicodeUTF8));
        actionAddResource->setText(QApplication::translate("STMainWindow", "Add Resource...", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("STMainWindow", "Preferences...", 0, QApplication::UnicodeUTF8));
        actionCreateParameterFile->setText(QApplication::translate("STMainWindow", "Create parameter file...", 0, QApplication::UnicodeUTF8));
        actionRTST->setText(QApplication::translate("STMainWindow", "RTST...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("STMainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("STMainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("STMainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuCompile->setTitle(QApplication::translate("STMainWindow", "Sequence", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("STMainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("STMainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("STMainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuResources->setTitle(QApplication::translate("STMainWindow", "Resources", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("STMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STMainWindow: public Ui_STMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STMAINWINDOW_H
