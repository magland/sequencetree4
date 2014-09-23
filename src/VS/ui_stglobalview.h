/********************************************************************************
** Form generated from reading UI file 'stglobalview.ui'
**
** Created: Wed May 22 14:48:46 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STGLOBALVIEW_H
#define UI_STGLOBALVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "stglobalview.h"

QT_BEGIN_NAMESPACE

class Ui_STGlobalView
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *button_frame;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QToolButton *delete_button;
    QToolButton *up_button;
    QToolButton *down_button;
    QSpacerItem *verticalSpacer;
    STGlobalViewTree *tree;

    void setupUi(QWidget *STGlobalView)
    {
        if (STGlobalView->objectName().isEmpty())
            STGlobalView->setObjectName(QString::fromUtf8("STGlobalView"));
        STGlobalView->resize(400, 300);
        horizontalLayout_2 = new QHBoxLayout(STGlobalView);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        button_frame = new QFrame(STGlobalView);
        button_frame->setObjectName(QString::fromUtf8("button_frame"));
        button_frame->setFrameShape(QFrame::StyledPanel);
        button_frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(button_frame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        delete_button = new QToolButton(button_frame);
        delete_button->setObjectName(QString::fromUtf8("delete_button"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/cross.png"), QSize(), QIcon::Normal, QIcon::Off);
        delete_button->setIcon(icon);

        verticalLayout->addWidget(delete_button);

        up_button = new QToolButton(button_frame);
        up_button->setObjectName(QString::fromUtf8("up_button"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/up.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        up_button->setIcon(icon1);

        verticalLayout->addWidget(up_button);

        down_button = new QToolButton(button_frame);
        down_button->setObjectName(QString::fromUtf8("down_button"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/down.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        down_button->setIcon(icon2);

        verticalLayout->addWidget(down_button);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_2->addWidget(button_frame);

        tree = new STGlobalViewTree(STGlobalView);
        tree->setObjectName(QString::fromUtf8("tree"));

        horizontalLayout_2->addWidget(tree);


        retranslateUi(STGlobalView);

        QMetaObject::connectSlotsByName(STGlobalView);
    } // setupUi

    void retranslateUi(QWidget *STGlobalView)
    {
        STGlobalView->setWindowTitle(QApplication::translate("STGlobalView", "Form", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        delete_button->setToolTip(QApplication::translate("STGlobalView", "Delete currently selected global parameter.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        delete_button->setText(QApplication::translate("STGlobalView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        up_button->setToolTip(QApplication::translate("STGlobalView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Move selected global parameter </span><span style=\" font-size:8pt; font-weight:600;\">up</span><span style=\" font-size:8pt;\"> in the list.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        up_button->setText(QApplication::translate("STGlobalView", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        down_button->setToolTip(QApplication::translate("STGlobalView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Move selected global parameter <span style=\" font-weight:600;\">down</span> in the list.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        down_button->setText(QApplication::translate("STGlobalView", "...", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = tree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("STGlobalView", "1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STGlobalView: public Ui_STGlobalView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STGLOBALVIEW_H
