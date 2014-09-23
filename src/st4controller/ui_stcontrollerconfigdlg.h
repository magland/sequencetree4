/********************************************************************************
** Form generated from reading UI file 'stcontrollerconfigdlg.ui'
**
** Created: Wed May 22 14:49:39 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STCONTROLLERCONFIGDLG_H
#define UI_STCONTROLLERCONFIGDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_STControllerConfigDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *seq_bin_dir;
    QLabel *label_2;
    QLineEdit *raw_data_dir;
    QLabel *label_3;
    QLineEdit *timer_interval;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *STControllerConfigDlg)
    {
        if (STControllerConfigDlg->objectName().isEmpty())
            STControllerConfigDlg->setObjectName(QString::fromUtf8("STControllerConfigDlg"));
        STControllerConfigDlg->resize(468, 166);
        verticalLayout = new QVBoxLayout(STControllerConfigDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(STControllerConfigDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        seq_bin_dir = new QLineEdit(STControllerConfigDlg);
        seq_bin_dir->setObjectName(QString::fromUtf8("seq_bin_dir"));

        gridLayout->addWidget(seq_bin_dir, 0, 1, 1, 1);

        label_2 = new QLabel(STControllerConfigDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        raw_data_dir = new QLineEdit(STControllerConfigDlg);
        raw_data_dir->setObjectName(QString::fromUtf8("raw_data_dir"));

        gridLayout->addWidget(raw_data_dir, 1, 1, 1, 1);

        label_3 = new QLabel(STControllerConfigDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        timer_interval = new QLineEdit(STControllerConfigDlg);
        timer_interval->setObjectName(QString::fromUtf8("timer_interval"));

        gridLayout->addWidget(timer_interval, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 34, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(STControllerConfigDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(STControllerConfigDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), STControllerConfigDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), STControllerConfigDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(STControllerConfigDlg);
    } // setupUi

    void retranslateUi(QDialog *STControllerConfigDlg)
    {
        STControllerConfigDlg->setWindowTitle(QApplication::translate("STControllerConfigDlg", "ST4 Controller Configuration", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("STControllerConfigDlg", "Seq. binary dir.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("STControllerConfigDlg", "Raw data dir.", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("STControllerConfigDlg", "Timer interval (sec)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STControllerConfigDlg: public Ui_STControllerConfigDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STCONTROLLERCONFIGDLG_H
