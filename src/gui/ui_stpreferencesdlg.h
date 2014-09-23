/********************************************************************************
** Form generated from reading UI file 'stpreferencesdlg.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STPREFERENCESDLG_H
#define UI_STPREFERENCESDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_stpreferencesdlg
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *line_thickness;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *stpreferencesdlg)
    {
        if (stpreferencesdlg->objectName().isEmpty())
            stpreferencesdlg->setObjectName(QString::fromUtf8("stpreferencesdlg"));
        stpreferencesdlg->resize(231, 157);
        verticalLayout = new QVBoxLayout(stpreferencesdlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(stpreferencesdlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        line_thickness = new QSpinBox(groupBox);
        line_thickness->setObjectName(QString::fromUtf8("line_thickness"));
        line_thickness->setMinimum(1);
        line_thickness->setMaximum(10);

        horizontalLayout->addWidget(line_thickness);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 45, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(stpreferencesdlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(stpreferencesdlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), stpreferencesdlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), stpreferencesdlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(stpreferencesdlg);
    } // setupUi

    void retranslateUi(QDialog *stpreferencesdlg)
    {
        stpreferencesdlg->setWindowTitle(QApplication::translate("stpreferencesdlg", "SequenceTree Preferences", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("stpreferencesdlg", "Simulation Display", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("stpreferencesdlg", "Line thickness:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class stpreferencesdlg: public Ui_stpreferencesdlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STPREFERENCESDLG_H
