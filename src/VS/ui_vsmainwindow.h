/********************************************************************************
** Form generated from reading UI file 'vsmainwindow.ui'
**
** Created: Wed May 22 14:48:46 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSMAINWINDOW_H
#define UI_VSMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "stcompilelog.h"
#include "vsphantomview.h"
#include "vssequenceview.h"

QT_BEGIN_NAMESPACE

class Ui_VSMainWindow
{
public:
    QAction *actionOpenSequence;
    QAction *actionScan;
    QAction *actionConfiguration;
    QAction *actionSelectPhantom;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_5;
    QSplitter *vsplitter;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_4;
    QSplitter *hsplitter;
    VSSequenceView *sequence_view;
    VSPhantomView *phantom_view;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *FOV_shift;
    QLabel *label_2;
    QLineEdit *min_term_size;
    QLabel *label_3;
    QLineEdit *max_num_terms;
    QLabel *label_4;
    QLineEdit *noise_level;
    QLabel *label_9;
    QLineEdit *b1_scale;
    QLabel *label_8;
    QLineEdit *b0_inhomogeneity;
    QLabel *label_5;
    QLineEdit *realtime_output;
    QSpacerItem *verticalSpacer;
    QTabWidget *logwidget;
    QWidget *compile_tab;
    QHBoxLayout *horizontalLayout;
    STCompileLog *compile_log;
    QWidget *simulate_tab;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *simulate_log;
    QMenuBar *menubar;
    QMenu *menuSequence;
    QMenu *menuTools;
    QMenu *menuPhantom;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *VSMainWindow)
    {
        if (VSMainWindow->objectName().isEmpty())
            VSMainWindow->setObjectName(QString::fromUtf8("VSMainWindow"));
        VSMainWindow->resize(818, 630);
        actionOpenSequence = new QAction(VSMainWindow);
        actionOpenSequence->setObjectName(QString::fromUtf8("actionOpenSequence"));
        actionScan = new QAction(VSMainWindow);
        actionScan->setObjectName(QString::fromUtf8("actionScan"));
        actionConfiguration = new QAction(VSMainWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        actionSelectPhantom = new QAction(VSMainWindow);
        actionSelectPhantom->setObjectName(QString::fromUtf8("actionSelectPhantom"));
        centralwidget = new QWidget(VSMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_5 = new QHBoxLayout(centralwidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        vsplitter = new QSplitter(centralwidget);
        vsplitter->setObjectName(QString::fromUtf8("vsplitter"));
        vsplitter->setOrientation(Qt::Vertical);
        frame = new QFrame(vsplitter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        hsplitter = new QSplitter(frame);
        hsplitter->setObjectName(QString::fromUtf8("hsplitter"));
        hsplitter->setOrientation(Qt::Horizontal);
        sequence_view = new VSSequenceView(hsplitter);
        sequence_view->setObjectName(QString::fromUtf8("sequence_view"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sequence_view->sizePolicy().hasHeightForWidth());
        sequence_view->setSizePolicy(sizePolicy);
        sequence_view->setMinimumSize(QSize(200, 100));
        sequence_view->setMaximumSize(QSize(10000, 16777215));
        hsplitter->addWidget(sequence_view);
        phantom_view = new VSPhantomView(hsplitter);
        phantom_view->setObjectName(QString::fromUtf8("phantom_view"));
        phantom_view->setMinimumSize(QSize(200, 0));
        hsplitter->addWidget(phantom_view);
        widget = new QWidget(hsplitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        FOV_shift = new QLineEdit(groupBox);
        FOV_shift->setObjectName(QString::fromUtf8("FOV_shift"));
        FOV_shift->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(FOV_shift, 0, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        min_term_size = new QLineEdit(groupBox);
        min_term_size->setObjectName(QString::fromUtf8("min_term_size"));
        min_term_size->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(min_term_size, 1, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        max_num_terms = new QLineEdit(groupBox);
        max_num_terms->setObjectName(QString::fromUtf8("max_num_terms"));
        max_num_terms->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(max_num_terms, 2, 2, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        noise_level = new QLineEdit(groupBox);
        noise_level->setObjectName(QString::fromUtf8("noise_level"));
        noise_level->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(noise_level, 3, 2, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 4, 0, 1, 1);

        b1_scale = new QLineEdit(groupBox);
        b1_scale->setObjectName(QString::fromUtf8("b1_scale"));

        gridLayout->addWidget(b1_scale, 4, 2, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 5, 0, 1, 1);

        b0_inhomogeneity = new QLineEdit(groupBox);
        b0_inhomogeneity->setObjectName(QString::fromUtf8("b0_inhomogeneity"));

        gridLayout->addWidget(b0_inhomogeneity, 5, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        realtime_output = new QLineEdit(widget);
        realtime_output->setObjectName(QString::fromUtf8("realtime_output"));

        verticalLayout->addWidget(realtime_output);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        hsplitter->addWidget(widget);

        horizontalLayout_4->addWidget(hsplitter);

        vsplitter->addWidget(frame);
        logwidget = new QTabWidget(vsplitter);
        logwidget->setObjectName(QString::fromUtf8("logwidget"));
        logwidget->setTabPosition(QTabWidget::South);
        compile_tab = new QWidget();
        compile_tab->setObjectName(QString::fromUtf8("compile_tab"));
        horizontalLayout = new QHBoxLayout(compile_tab);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        compile_log = new STCompileLog(compile_tab);
        compile_log->setObjectName(QString::fromUtf8("compile_log"));

        horizontalLayout->addWidget(compile_log);

        logwidget->addTab(compile_tab, QString());
        simulate_tab = new QWidget();
        simulate_tab->setObjectName(QString::fromUtf8("simulate_tab"));
        horizontalLayout_2 = new QHBoxLayout(simulate_tab);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        simulate_log = new QListWidget(simulate_tab);
        simulate_log->setObjectName(QString::fromUtf8("simulate_log"));

        horizontalLayout_2->addWidget(simulate_log);

        logwidget->addTab(simulate_tab, QString());
        vsplitter->addWidget(logwidget);

        horizontalLayout_5->addWidget(vsplitter);

        VSMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(VSMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 818, 27));
        menuSequence = new QMenu(menubar);
        menuSequence->setObjectName(QString::fromUtf8("menuSequence"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuPhantom = new QMenu(menubar);
        menuPhantom->setObjectName(QString::fromUtf8("menuPhantom"));
        VSMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(VSMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        VSMainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(VSMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        VSMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuSequence->menuAction());
        menubar->addAction(menuPhantom->menuAction());
        menubar->addAction(menuTools->menuAction());
        menuSequence->addAction(actionOpenSequence);
        menuTools->addAction(actionConfiguration);
        menuPhantom->addAction(actionSelectPhantom);
        toolBar->addAction(actionScan);

        retranslateUi(VSMainWindow);

        logwidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(VSMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *VSMainWindow)
    {
        VSMainWindow->setWindowTitle(QApplication::translate("VSMainWindow", "SequenceTree - Virtual Scanner", 0, QApplication::UnicodeUTF8));
        actionOpenSequence->setText(QApplication::translate("VSMainWindow", "Open Sequence...", 0, QApplication::UnicodeUTF8));
        actionScan->setText(QApplication::translate("VSMainWindow", "Scan", 0, QApplication::UnicodeUTF8));
        actionConfiguration->setText(QApplication::translate("VSMainWindow", "Configuration...", 0, QApplication::UnicodeUTF8));
        actionSelectPhantom->setText(QApplication::translate("VSMainWindow", "Select phantom...", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("VSMainWindow", "Scanner", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VSMainWindow", "FOV shift (mm):", 0, QApplication::UnicodeUTF8));
        FOV_shift->setText(QApplication::translate("VSMainWindow", "(0,0,0)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("VSMainWindow", "Min. term size:", 0, QApplication::UnicodeUTF8));
        min_term_size->setText(QApplication::translate("VSMainWindow", "0.0001", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VSMainWindow", "Max. num. terms:", 0, QApplication::UnicodeUTF8));
        max_num_terms->setText(QApplication::translate("VSMainWindow", "300", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("VSMainWindow", "Noise level:", 0, QApplication::UnicodeUTF8));
        noise_level->setText(QApplication::translate("VSMainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("VSMainWindow", "B1 scale:", 0, QApplication::UnicodeUTF8));
        b1_scale->setText(QApplication::translate("VSMainWindow", "1", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("VSMainWindow", "B0 inhom (Hz/mm):", 0, QApplication::UnicodeUTF8));
        b0_inhomogeneity->setText(QApplication::translate("VSMainWindow", "(0,0,0)", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("VSMainWindow", "Real time output directory:", 0, QApplication::UnicodeUTF8));
        logwidget->setTabText(logwidget->indexOf(compile_tab), QApplication::translate("VSMainWindow", "Compile", 0, QApplication::UnicodeUTF8));
        logwidget->setTabText(logwidget->indexOf(simulate_tab), QApplication::translate("VSMainWindow", "Simulate", 0, QApplication::UnicodeUTF8));
        menuSequence->setTitle(QApplication::translate("VSMainWindow", "Sequence", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("VSMainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuPhantom->setTitle(QApplication::translate("VSMainWindow", "Phantom", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("VSMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VSMainWindow: public Ui_VSMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSMAINWINDOW_H
