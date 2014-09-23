/********************************************************************************
** Form generated from reading UI file 'vssequenceview.ui'
**
** Created: Wed May 22 14:48:46 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSSEQUENCEVIEW_H
#define UI_VSSEQUENCEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "stglobalview.h"

QT_BEGIN_NAMESPACE

class Ui_VSSequenceView
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *sequence_name;
    QLabel *label_2;
    QLineEdit *duration;
    QLabel *label_3;
    QLineEdit *num_blocks;
    STGlobalView *global_view;

    void setupUi(QWidget *VSSequenceView)
    {
        if (VSSequenceView->objectName().isEmpty())
            VSSequenceView->setObjectName(QString::fromUtf8("VSSequenceView"));
        VSSequenceView->resize(437, 300);
        verticalLayout = new QVBoxLayout(VSSequenceView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(VSSequenceView);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        sequence_name = new QLineEdit(VSSequenceView);
        sequence_name->setObjectName(QString::fromUtf8("sequence_name"));
        sequence_name->setReadOnly(true);

        gridLayout->addWidget(sequence_name, 0, 1, 1, 3);

        label_2 = new QLabel(VSSequenceView);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        duration = new QLineEdit(VSSequenceView);
        duration->setObjectName(QString::fromUtf8("duration"));
        duration->setReadOnly(true);

        gridLayout->addWidget(duration, 1, 1, 1, 1);

        label_3 = new QLabel(VSSequenceView);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 2, 1, 1);

        num_blocks = new QLineEdit(VSSequenceView);
        num_blocks->setObjectName(QString::fromUtf8("num_blocks"));
        num_blocks->setReadOnly(true);

        gridLayout->addWidget(num_blocks, 1, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        global_view = new STGlobalView(VSSequenceView);
        global_view->setObjectName(QString::fromUtf8("global_view"));

        verticalLayout->addWidget(global_view);


        retranslateUi(VSSequenceView);

        QMetaObject::connectSlotsByName(VSSequenceView);
    } // setupUi

    void retranslateUi(QWidget *VSSequenceView)
    {
        VSSequenceView->setWindowTitle(QApplication::translate("VSSequenceView", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VSSequenceView", "Sequence:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("VSSequenceView", "Duration:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VSSequenceView", "Num. blocks:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VSSequenceView: public Ui_VSSequenceView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSSEQUENCEVIEW_H
