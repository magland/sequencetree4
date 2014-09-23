/********************************************************************************
** Form generated from reading UI file 'stpastedialog.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STPASTEDIALOG_H
#define UI_STPASTEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_STPasteDialog
{
public:
    QVBoxLayout *vboxLayout;
    QRadioButton *values_button;
    QRadioButton *structure_button;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *STPasteDialog)
    {
        if (STPasteDialog->objectName().isEmpty())
            STPasteDialog->setObjectName(QString::fromUtf8("STPasteDialog"));
        STPasteDialog->resize(269, 112);
        vboxLayout = new QVBoxLayout(STPasteDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        values_button = new QRadioButton(STPasteDialog);
        values_button->setObjectName(QString::fromUtf8("values_button"));
        values_button->setChecked(true);

        vboxLayout->addWidget(values_button);

        structure_button = new QRadioButton(STPasteDialog);
        structure_button->setObjectName(QString::fromUtf8("structure_button"));

        vboxLayout->addWidget(structure_button);

        buttonBox = new QDialogButtonBox(STPasteDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(STPasteDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), STPasteDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), STPasteDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(STPasteDialog);
    } // setupUi

    void retranslateUi(QDialog *STPasteDialog)
    {
        STPasteDialog->setWindowTitle(QApplication::translate("STPasteDialog", "Paste Node", 0, QApplication::UnicodeUTF8));
        values_button->setText(QApplication::translate("STPasteDialog", "Paste values only", 0, QApplication::UnicodeUTF8));
        structure_button->setText(QApplication::translate("STPasteDialog", "Paste values and structure", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STPasteDialog: public Ui_STPasteDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STPASTEDIALOG_H
