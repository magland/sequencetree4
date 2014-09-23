/********************************************************************************
** Form generated from reading UI file 'stcontrollermainwindow.ui'
**
** Created: Wed May 22 14:49:39 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STCONTROLLERMAINWINDOW_H
#define UI_STCONTROLLERMAINWINDOW_H

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
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "rawdatafiletree.h"
#include "sequencestree.h"

QT_BEGIN_NAMESPACE

class Ui_STControllerMainWindow
{
public:
    QAction *actionConfiguration;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *log_text;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *refresh_sequences_button;
    QPushButton *open_sequence_folder_button;
    QSpacerItem *horizontalSpacer_2;
    SequencesTree *sequences;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *refresh_raw_data_button;
    QPushButton *view_header_button;
    QPushButton *parse_button;
    QPushButton *open_raw_folder_button;
    QSpacerItem *horizontalSpacer;
    RawDataFileTree *raw_data_files;
    QWidget *tab_3;
    QMenuBar *menubar;
    QMenu *menuTools;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *STControllerMainWindow)
    {
        if (STControllerMainWindow->objectName().isEmpty())
            STControllerMainWindow->setObjectName(QString::fromUtf8("STControllerMainWindow"));
        STControllerMainWindow->resize(569, 600);
        actionConfiguration = new QAction(STControllerMainWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        centralwidget = new QWidget(STControllerMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 33, 569, 548));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setGeometry(QRect(0, 0, 545, 503));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        log_text = new QTextEdit(tab);
        log_text->setObjectName(QString::fromUtf8("log_text"));
        log_text->setLineWrapMode(QTextEdit::NoWrap);
        log_text->setReadOnly(true);

        horizontalLayout_2->addWidget(log_text);

        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tab_4->setGeometry(QRect(0, 0, 545, 503));
        verticalLayout_2 = new QVBoxLayout(tab_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame_2 = new QFrame(tab_4);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        refresh_sequences_button = new QPushButton(frame_2);
        refresh_sequences_button->setObjectName(QString::fromUtf8("refresh_sequences_button"));

        horizontalLayout_4->addWidget(refresh_sequences_button);

        open_sequence_folder_button = new QPushButton(frame_2);
        open_sequence_folder_button->setObjectName(QString::fromUtf8("open_sequence_folder_button"));

        horizontalLayout_4->addWidget(open_sequence_folder_button);

        horizontalSpacer_2 = new QSpacerItem(423, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(frame_2);

        sequences = new SequencesTree(tab_4);
        sequences->setObjectName(QString::fromUtf8("sequences"));

        verticalLayout_2->addWidget(sequences);

        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setGeometry(QRect(0, 0, 545, 503));
        verticalLayout = new QVBoxLayout(tab_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(tab_2);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        refresh_raw_data_button = new QPushButton(frame);
        refresh_raw_data_button->setObjectName(QString::fromUtf8("refresh_raw_data_button"));

        horizontalLayout_3->addWidget(refresh_raw_data_button);

        view_header_button = new QPushButton(frame);
        view_header_button->setObjectName(QString::fromUtf8("view_header_button"));

        horizontalLayout_3->addWidget(view_header_button);

        parse_button = new QPushButton(frame);
        parse_button->setObjectName(QString::fromUtf8("parse_button"));

        horizontalLayout_3->addWidget(parse_button);

        open_raw_folder_button = new QPushButton(frame);
        open_raw_folder_button->setObjectName(QString::fromUtf8("open_raw_folder_button"));

        horizontalLayout_3->addWidget(open_raw_folder_button);

        horizontalSpacer = new QSpacerItem(261, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addWidget(frame);

        raw_data_files = new RawDataFileTree(tab_2);
        raw_data_files->setObjectName(QString::fromUtf8("raw_data_files"));

        verticalLayout->addWidget(raw_data_files);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab_3->setGeometry(QRect(0, 0, 545, 503));
        tabWidget->addTab(tab_3, QString());

        horizontalLayout->addWidget(tabWidget);

        STControllerMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(STControllerMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 569, 21));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        STControllerMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(STControllerMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 581, 569, 19));
        STControllerMainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(STControllerMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setGeometry(QRect(0, 21, 569, 12));
        STControllerMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuTools->menuAction());
        menuTools->addAction(actionConfiguration);

        retranslateUi(STControllerMainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(STControllerMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *STControllerMainWindow)
    {
        STControllerMainWindow->setWindowTitle(QApplication::translate("STControllerMainWindow", "SequenceTree4 Controller", 0, QApplication::UnicodeUTF8));
        actionConfiguration->setText(QApplication::translate("STControllerMainWindow", "Controller Configuration...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("STControllerMainWindow", "Activity Log", 0, QApplication::UnicodeUTF8));
        refresh_sequences_button->setText(QApplication::translate("STControllerMainWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        open_sequence_folder_button->setText(QApplication::translate("STControllerMainWindow", "Open Folder", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = sequences->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("STControllerMainWindow", "1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("STControllerMainWindow", "Sequences", 0, QApplication::UnicodeUTF8));
        refresh_raw_data_button->setText(QApplication::translate("STControllerMainWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        view_header_button->setText(QApplication::translate("STControllerMainWindow", "View Header", 0, QApplication::UnicodeUTF8));
        parse_button->setText(QApplication::translate("STControllerMainWindow", "Parse", 0, QApplication::UnicodeUTF8));
        open_raw_folder_button->setText(QApplication::translate("STControllerMainWindow", "Open Folder", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = raw_data_files->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("STControllerMainWindow", "1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("STControllerMainWindow", "Raw Data", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("STControllerMainWindow", "Experiments", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("STControllerMainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("STControllerMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STControllerMainWindow: public Ui_STControllerMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STCONTROLLERMAINWINDOW_H
