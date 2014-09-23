/********************************************************************************
** Form generated from reading UI file 'stsimview.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STSIMVIEW_H
#define UI_STSIMVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "stsimblockview.h"

QT_BEGIN_NAMESPACE

class Ui_STSimView
{
public:
    QVBoxLayout *vboxLayout;
    STSimBlockView *blockview;
    QScrollBar *scrollbar;

    void setupUi(QWidget *STSimView)
    {
        if (STSimView->objectName().isEmpty())
            STSimView->setObjectName(QString::fromUtf8("STSimView"));
        STSimView->resize(400, 300);
        vboxLayout = new QVBoxLayout(STSimView);
        vboxLayout->setSpacing(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        blockview = new STSimBlockView(STSimView);
        blockview->setObjectName(QString::fromUtf8("blockview"));

        vboxLayout->addWidget(blockview);

        scrollbar = new QScrollBar(STSimView);
        scrollbar->setObjectName(QString::fromUtf8("scrollbar"));
        scrollbar->setOrientation(Qt::Horizontal);

        vboxLayout->addWidget(scrollbar);


        retranslateUi(STSimView);

        QMetaObject::connectSlotsByName(STSimView);
    } // setupUi

    void retranslateUi(QWidget *STSimView)
    {
        STSimView->setWindowTitle(QApplication::translate("STSimView", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STSimView: public Ui_STSimView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STSIMVIEW_H
