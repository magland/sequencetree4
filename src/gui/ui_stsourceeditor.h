/********************************************************************************
** Form generated from reading UI file 'stsourceeditor.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STSOURCEEDITOR_H
#define UI_STSOURCEEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "defaulteditorwindow.h"

QT_BEGIN_NAMESPACE

class Ui_STSourceEditor
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabWidget;
    QWidget *header_tab;
    QHBoxLayout *hboxLayout;
    DefaultEditorWindow *header;
    QWidget *source_tab;
    QHBoxLayout *hboxLayout1;
    DefaultEditorWindow *source;
    QFrame *frame;
    QHBoxLayout *hboxLayout2;
    QComboBox *class_box;
    QToolButton *save_changes_button;
    QPushButton *reload_button;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *STSourceEditor)
    {
        if (STSourceEditor->objectName().isEmpty())
            STSourceEditor->setObjectName(QString::fromUtf8("STSourceEditor"));
        STSourceEditor->resize(442, 419);
        vboxLayout = new QVBoxLayout(STSourceEditor);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        tabWidget = new QTabWidget(STSourceEditor);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        header_tab = new QWidget();
        header_tab->setObjectName(QString::fromUtf8("header_tab"));
        header_tab->setGeometry(QRect(0, 0, 418, 343));
        hboxLayout = new QHBoxLayout(header_tab);
        hboxLayout->setSpacing(0);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        header = new DefaultEditorWindow(header_tab);
        header->setObjectName(QString::fromUtf8("header"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(header->sizePolicy().hasHeightForWidth());
        header->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(header);

        tabWidget->addTab(header_tab, QString());
        source_tab = new QWidget();
        source_tab->setObjectName(QString::fromUtf8("source_tab"));
        source_tab->setGeometry(QRect(0, 0, 418, 343));
        hboxLayout1 = new QHBoxLayout(source_tab);
        hboxLayout1->setSpacing(0);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        source = new DefaultEditorWindow(source_tab);
        source->setObjectName(QString::fromUtf8("source"));
        sizePolicy.setHeightForWidth(source->sizePolicy().hasHeightForWidth());
        source->setSizePolicy(sizePolicy);

        hboxLayout1->addWidget(source);

        tabWidget->addTab(source_tab, QString());

        vboxLayout->addWidget(tabWidget);

        frame = new QFrame(STSourceEditor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        hboxLayout2 = new QHBoxLayout(frame);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        class_box = new QComboBox(frame);
        class_box->setObjectName(QString::fromUtf8("class_box"));

        hboxLayout2->addWidget(class_box);

        save_changes_button = new QToolButton(frame);
        save_changes_button->setObjectName(QString::fromUtf8("save_changes_button"));
        QFont font;
        font.setPointSize(10);
        save_changes_button->setFont(font);

        hboxLayout2->addWidget(save_changes_button);

        reload_button = new QPushButton(frame);
        reload_button->setObjectName(QString::fromUtf8("reload_button"));

        hboxLayout2->addWidget(reload_button);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);


        vboxLayout->addWidget(frame);


        retranslateUi(STSourceEditor);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(STSourceEditor);
    } // setupUi

    void retranslateUi(QWidget *STSourceEditor)
    {
        STSourceEditor->setWindowTitle(QApplication::translate("STSourceEditor", "ST Source Editor", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(header_tab), QApplication::translate("STSourceEditor", "Header", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(source_tab), QApplication::translate("STSourceEditor", "Source", 0, QApplication::UnicodeUTF8));
        save_changes_button->setText(QApplication::translate("STSourceEditor", "...", 0, QApplication::UnicodeUTF8));
        reload_button->setText(QApplication::translate("STSourceEditor", "Reload from disk", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STSourceEditor: public Ui_STSourceEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STSOURCEEDITOR_H
