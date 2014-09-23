/********************************************************************************
** Form generated from reading UI file 'stnodeview.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STNODEVIEW_H
#define UI_STNODEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "stparametertree.h"

QT_BEGIN_NAMESPACE

class Ui_STNodeView
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    STParameterTree *parameters;

    void setupUi(QWidget *STNodeView)
    {
        if (STNodeView->objectName().isEmpty())
            STNodeView->setObjectName(QString::fromUtf8("STNodeView"));
        STNodeView->resize(400, 300);
        vboxLayout = new QVBoxLayout(STNodeView);
        vboxLayout->setSpacing(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(20);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

        vboxLayout->addLayout(hboxLayout);

        parameters = new STParameterTree(STNodeView);
        parameters->setObjectName(QString::fromUtf8("parameters"));

        vboxLayout->addWidget(parameters);


        retranslateUi(STNodeView);

        QMetaObject::connectSlotsByName(STNodeView);
    } // setupUi

    void retranslateUi(QWidget *STNodeView)
    {
        STNodeView->setWindowTitle(QApplication::translate("STNodeView", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = parameters->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("STNodeView", "1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STNodeView: public Ui_STNodeView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STNODEVIEW_H
