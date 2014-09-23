/********************************************************************************
** Form generated from reading UI file 'stconfigurationdlg.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STCONFIGURATIONDLG_H
#define UI_STCONFIGURATIONDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_STConfigurationDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout;
    QRadioButton *mingw_button;
    QRadioButton *msvc_button;
    QRadioButton *linux_button;
    QRadioButton *mac_button;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *STConfigurationDlg)
    {
        if (STConfigurationDlg->objectName().isEmpty())
            STConfigurationDlg->setObjectName(QString::fromUtf8("STConfigurationDlg"));
        STConfigurationDlg->resize(276, 173);
        verticalLayout = new QVBoxLayout(STConfigurationDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(STConfigurationDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        vboxLayout = new QVBoxLayout(groupBox);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        mingw_button = new QRadioButton(groupBox);
        mingw_button->setObjectName(QString::fromUtf8("mingw_button"));
        mingw_button->setChecked(true);

        vboxLayout->addWidget(mingw_button);

        msvc_button = new QRadioButton(groupBox);
        msvc_button->setObjectName(QString::fromUtf8("msvc_button"));

        vboxLayout->addWidget(msvc_button);

        linux_button = new QRadioButton(groupBox);
        linux_button->setObjectName(QString::fromUtf8("linux_button"));

        vboxLayout->addWidget(linux_button);

        mac_button = new QRadioButton(groupBox);
        mac_button->setObjectName(QString::fromUtf8("mac_button"));

        vboxLayout->addWidget(mac_button);


        verticalLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(STConfigurationDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(STConfigurationDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), STConfigurationDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), STConfigurationDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(STConfigurationDlg);
    } // setupUi

    void retranslateUi(QDialog *STConfigurationDlg)
    {
        STConfigurationDlg->setWindowTitle(QApplication::translate("STConfigurationDlg", "SequenceTree Configuration", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("STConfigurationDlg", "Compile sequences using...", 0, QApplication::UnicodeUTF8));
        mingw_button->setText(QApplication::translate("STConfigurationDlg", "MinGW", 0, QApplication::UnicodeUTF8));
        msvc_button->setText(QApplication::translate("STConfigurationDlg", "MSVC", 0, QApplication::UnicodeUTF8));
        linux_button->setText(QApplication::translate("STConfigurationDlg", "Linux (g++)", 0, QApplication::UnicodeUTF8));
        mac_button->setText(QApplication::translate("STConfigurationDlg", "Mac OSX (g++)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STConfigurationDlg: public Ui_STConfigurationDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STCONFIGURATIONDLG_H
