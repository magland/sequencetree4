/********************************************************************************
** Form generated from reading UI file 'stsequenceview.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STSEQUENCEVIEW_H
#define UI_STSEQUENCEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include "stclassview.h"
#include "stcompilelog.h"
#include "steventlist.h"
#include "stglobalview.h"
#include "stnodeview.h"
#include "stsimview.h"
#include "stsourceeditor.h"
#include "sttreeview.h"

QT_BEGIN_NAMESPACE

class Ui_STSequenceView
{
public:
    QHBoxLayout *hboxLayout;
    QSplitter *vsplitter2;
    QSplitter *hsplitter;
    QTabWidget *browser_tab;
    QWidget *tree_tab;
    QHBoxLayout *hboxLayout1;
    STTreeView *treeview;
    QWidget *classes_tab;
    QHBoxLayout *hboxLayout2;
    STClassView *classview;
    QWidget *global_tab;
    QHBoxLayout *horizontalLayout_2;
    STGlobalView *globalview;
    QTabWidget *tabWidget;
    QWidget *parameters_tab;
    QHBoxLayout *hboxLayout3;
    QSplitter *vsplitter;
    STNodeView *nodeview;
    STSimView *simview;
    QWidget *code_tab;
    QHBoxLayout *hboxLayout4;
    STSourceEditor *codeview;
    QWidget *notes_tab;
    QHBoxLayout *horizontalLayout;
    QTextEdit *notes;
    QWidget *reconstruction_tab;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *reconstruction;
    QTabWidget *logtabwidget;
    QWidget *compile_tab;
    QHBoxLayout *hboxLayout5;
    STCompileLog *compile_log;
    QWidget *simulate_tab;
    QHBoxLayout *hboxLayout6;
    QListWidget *simulate_log;
    QWidget *events_tab;
    QHBoxLayout *hboxLayout7;
    STEventList *event_list;

    void setupUi(QWidget *STSequenceView)
    {
        if (STSequenceView->objectName().isEmpty())
            STSequenceView->setObjectName(QString::fromUtf8("STSequenceView"));
        STSequenceView->resize(682, 561);
        hboxLayout = new QHBoxLayout(STSequenceView);
        hboxLayout->setSpacing(0);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vsplitter2 = new QSplitter(STSequenceView);
        vsplitter2->setObjectName(QString::fromUtf8("vsplitter2"));
        vsplitter2->setOrientation(Qt::Vertical);
        hsplitter = new QSplitter(vsplitter2);
        hsplitter->setObjectName(QString::fromUtf8("hsplitter"));
        hsplitter->setOrientation(Qt::Horizontal);
        browser_tab = new QTabWidget(hsplitter);
        browser_tab->setObjectName(QString::fromUtf8("browser_tab"));
        browser_tab->setTabPosition(QTabWidget::South);
        tree_tab = new QWidget();
        tree_tab->setObjectName(QString::fromUtf8("tree_tab"));
        tree_tab->setGeometry(QRect(0, 0, 239, 244));
        hboxLayout1 = new QHBoxLayout(tree_tab);
        hboxLayout1->setSpacing(0);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        treeview = new STTreeView(tree_tab);
        treeview->setObjectName(QString::fromUtf8("treeview"));

        hboxLayout1->addWidget(treeview);

        browser_tab->addTab(tree_tab, QString());
        classes_tab = new QWidget();
        classes_tab->setObjectName(QString::fromUtf8("classes_tab"));
        classes_tab->setGeometry(QRect(0, 0, 239, 244));
        hboxLayout2 = new QHBoxLayout(classes_tab);
        hboxLayout2->setSpacing(0);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        classview = new STClassView(classes_tab);
        classview->setObjectName(QString::fromUtf8("classview"));

        hboxLayout2->addWidget(classview);

        browser_tab->addTab(classes_tab, QString());
        global_tab = new QWidget();
        global_tab->setObjectName(QString::fromUtf8("global_tab"));
        global_tab->setGeometry(QRect(0, 0, 239, 244));
        horizontalLayout_2 = new QHBoxLayout(global_tab);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        globalview = new STGlobalView(global_tab);
        globalview->setObjectName(QString::fromUtf8("globalview"));

        horizontalLayout_2->addWidget(globalview);

        browser_tab->addTab(global_tab, QString());
        hsplitter->addWidget(browser_tab);
        tabWidget = new QTabWidget(hsplitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::West);
        parameters_tab = new QWidget();
        parameters_tab->setObjectName(QString::fromUtf8("parameters_tab"));
        parameters_tab->setGeometry(QRect(0, 0, 405, 263));
        hboxLayout3 = new QHBoxLayout(parameters_tab);
        hboxLayout3->setSpacing(0);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        vsplitter = new QSplitter(parameters_tab);
        vsplitter->setObjectName(QString::fromUtf8("vsplitter"));
        vsplitter->setOrientation(Qt::Vertical);
        nodeview = new STNodeView(vsplitter);
        nodeview->setObjectName(QString::fromUtf8("nodeview"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(nodeview->sizePolicy().hasHeightForWidth());
        nodeview->setSizePolicy(sizePolicy);
        vsplitter->addWidget(nodeview);
        simview = new STSimView(vsplitter);
        simview->setObjectName(QString::fromUtf8("simview"));
        simview->setMinimumSize(QSize(0, 100));
        vsplitter->addWidget(simview);

        hboxLayout3->addWidget(vsplitter);

        tabWidget->addTab(parameters_tab, QString());
        code_tab = new QWidget();
        code_tab->setObjectName(QString::fromUtf8("code_tab"));
        code_tab->setGeometry(QRect(0, 0, 405, 263));
        hboxLayout4 = new QHBoxLayout(code_tab);
        hboxLayout4->setSpacing(0);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        codeview = new STSourceEditor(code_tab);
        codeview->setObjectName(QString::fromUtf8("codeview"));

        hboxLayout4->addWidget(codeview);

        tabWidget->addTab(code_tab, QString());
        notes_tab = new QWidget();
        notes_tab->setObjectName(QString::fromUtf8("notes_tab"));
        notes_tab->setGeometry(QRect(0, 0, 405, 263));
        horizontalLayout = new QHBoxLayout(notes_tab);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        notes = new QTextEdit(notes_tab);
        notes->setObjectName(QString::fromUtf8("notes"));

        horizontalLayout->addWidget(notes);

        tabWidget->addTab(notes_tab, QString());
        reconstruction_tab = new QWidget();
        reconstruction_tab->setObjectName(QString::fromUtf8("reconstruction_tab"));
        reconstruction_tab->setGeometry(QRect(0, 0, 405, 263));
        horizontalLayout_3 = new QHBoxLayout(reconstruction_tab);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        reconstruction = new QTextEdit(reconstruction_tab);
        reconstruction->setObjectName(QString::fromUtf8("reconstruction"));

        horizontalLayout_3->addWidget(reconstruction);

        tabWidget->addTab(reconstruction_tab, QString());
        hsplitter->addWidget(tabWidget);
        vsplitter2->addWidget(hsplitter);
        logtabwidget = new QTabWidget(vsplitter2);
        logtabwidget->setObjectName(QString::fromUtf8("logtabwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logtabwidget->sizePolicy().hasHeightForWidth());
        logtabwidget->setSizePolicy(sizePolicy1);
        logtabwidget->setTabPosition(QTabWidget::South);
        compile_tab = new QWidget();
        compile_tab->setObjectName(QString::fromUtf8("compile_tab"));
        compile_tab->setGeometry(QRect(0, 0, 676, 262));
        hboxLayout5 = new QHBoxLayout(compile_tab);
        hboxLayout5->setSpacing(0);
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        compile_log = new STCompileLog(compile_tab);
        compile_log->setObjectName(QString::fromUtf8("compile_log"));

        hboxLayout5->addWidget(compile_log);

        logtabwidget->addTab(compile_tab, QString());
        simulate_tab = new QWidget();
        simulate_tab->setObjectName(QString::fromUtf8("simulate_tab"));
        simulate_tab->setGeometry(QRect(0, 0, 676, 262));
        hboxLayout6 = new QHBoxLayout(simulate_tab);
        hboxLayout6->setSpacing(0);
        hboxLayout6->setContentsMargins(0, 0, 0, 0);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        simulate_log = new QListWidget(simulate_tab);
        simulate_log->setObjectName(QString::fromUtf8("simulate_log"));

        hboxLayout6->addWidget(simulate_log);

        logtabwidget->addTab(simulate_tab, QString());
        events_tab = new QWidget();
        events_tab->setObjectName(QString::fromUtf8("events_tab"));
        events_tab->setGeometry(QRect(0, 0, 676, 262));
        hboxLayout7 = new QHBoxLayout(events_tab);
        hboxLayout7->setSpacing(0);
        hboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        event_list = new STEventList(events_tab);
        event_list->setObjectName(QString::fromUtf8("event_list"));

        hboxLayout7->addWidget(event_list);

        logtabwidget->addTab(events_tab, QString());
        vsplitter2->addWidget(logtabwidget);

        hboxLayout->addWidget(vsplitter2);


        retranslateUi(STSequenceView);

        browser_tab->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);
        logtabwidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(STSequenceView);
    } // setupUi

    void retranslateUi(QWidget *STSequenceView)
    {
        STSequenceView->setWindowTitle(QApplication::translate("STSequenceView", "Form", 0, QApplication::UnicodeUTF8));
        browser_tab->setTabText(browser_tab->indexOf(tree_tab), QApplication::translate("STSequenceView", "Tree", 0, QApplication::UnicodeUTF8));
        browser_tab->setTabText(browser_tab->indexOf(classes_tab), QApplication::translate("STSequenceView", "Classes", 0, QApplication::UnicodeUTF8));
        browser_tab->setTabText(browser_tab->indexOf(global_tab), QApplication::translate("STSequenceView", "Global", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(parameters_tab), QApplication::translate("STSequenceView", "Parameters", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(code_tab), QApplication::translate("STSequenceView", "Code", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(notes_tab), QApplication::translate("STSequenceView", "Notes", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(reconstruction_tab), QApplication::translate("STSequenceView", "Reconstruction", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        logtabwidget->setWhatsThis(QApplication::translate("STSequenceView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The <span style=\" font-weight:600;\">Parameters</span> tab shows the parameters associated with the currently selected node (in the tree view). After the sequence is compiled, the sequence events are displayed in the simulation view. This display is updated in real time as you modify sequence parameters (by double clicking and editing). Green balls indicate which parameters are editable by the user. Red balls indicate which parameters are set programmatically within the sequence.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        logtabwidget->setTabText(logtabwidget->indexOf(compile_tab), QApplication::translate("STSequenceView", "Compile", 0, QApplication::UnicodeUTF8));
        logtabwidget->setTabText(logtabwidget->indexOf(simulate_tab), QApplication::translate("STSequenceView", "Simulate", 0, QApplication::UnicodeUTF8));
        logtabwidget->setTabText(logtabwidget->indexOf(events_tab), QApplication::translate("STSequenceView", "Events", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STSequenceView: public Ui_STSequenceView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STSEQUENCEVIEW_H
