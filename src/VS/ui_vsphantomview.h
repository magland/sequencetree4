/********************************************************************************
** Form generated from reading UI file 'vsphantomview.ui'
**
** Created: Wed May 22 14:48:46 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSPHANTOMVIEW_H
#define UI_VSPHANTOMVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "vspropertyeditor.h"

QT_BEGIN_NAMESPACE

class Ui_VSPhantomView
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *phantom_name;
    QToolButton *select_phantom_button;
    VSPropertyEditor *property_editor;

    void setupUi(QWidget *VSPhantomView)
    {
        if (VSPhantomView->objectName().isEmpty())
            VSPhantomView->setObjectName(QString::fromUtf8("VSPhantomView"));
        VSPhantomView->resize(281, 300);
        verticalLayout = new QVBoxLayout(VSPhantomView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(VSPhantomView);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        phantom_name = new QLineEdit(VSPhantomView);
        phantom_name->setObjectName(QString::fromUtf8("phantom_name"));
        phantom_name->setMinimumSize(QSize(150, 0));
        phantom_name->setReadOnly(true);

        gridLayout->addWidget(phantom_name, 0, 1, 1, 1);

        select_phantom_button = new QToolButton(VSPhantomView);
        select_phantom_button->setObjectName(QString::fromUtf8("select_phantom_button"));

        gridLayout->addWidget(select_phantom_button, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        property_editor = new VSPropertyEditor(VSPhantomView);
        property_editor->setObjectName(QString::fromUtf8("property_editor"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(property_editor->sizePolicy().hasHeightForWidth());
        property_editor->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(property_editor);


        retranslateUi(VSPhantomView);

        QMetaObject::connectSlotsByName(VSPhantomView);
    } // setupUi

    void retranslateUi(QWidget *VSPhantomView)
    {
        VSPhantomView->setWindowTitle(QApplication::translate("VSPhantomView", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VSPhantomView", "Phantom:", 0, QApplication::UnicodeUTF8));
        select_phantom_button->setText(QApplication::translate("VSPhantomView", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VSPhantomView: public Ui_VSPhantomView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSPHANTOMVIEW_H
