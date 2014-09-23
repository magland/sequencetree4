/********************************************************************************
** Form generated from reading UI file 'readrawdatadlg.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_READRAWDATADLG_H
#define UI_READRAWDATADLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ReadRawDataDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *raw_data_template;
    QLabel *label_2;
    QLineEdit *raw_data;
    QToolButton *browse_raw_data_button;
    QLabel *label_3;
    QLineEdit *num_channels;
    QHBoxLayout *horizontalLayout;
    QRadioButton *siemens_va_format;
    QRadioButton *siemens_vb_format;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ReadRawDataDlg)
    {
        if (ReadRawDataDlg->objectName().isEmpty())
            ReadRawDataDlg->setObjectName(QString::fromUtf8("ReadRawDataDlg"));
        ReadRawDataDlg->resize(432, 205);
        verticalLayout = new QVBoxLayout(ReadRawDataDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ReadRawDataDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        raw_data_template = new QLineEdit(ReadRawDataDlg);
        raw_data_template->setObjectName(QString::fromUtf8("raw_data_template"));
        raw_data_template->setReadOnly(true);

        gridLayout->addWidget(raw_data_template, 0, 1, 1, 1);

        label_2 = new QLabel(ReadRawDataDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        raw_data = new QLineEdit(ReadRawDataDlg);
        raw_data->setObjectName(QString::fromUtf8("raw_data"));

        gridLayout->addWidget(raw_data, 1, 1, 1, 1);

        browse_raw_data_button = new QToolButton(ReadRawDataDlg);
        browse_raw_data_button->setObjectName(QString::fromUtf8("browse_raw_data_button"));

        gridLayout->addWidget(browse_raw_data_button, 1, 2, 1, 1);

        label_3 = new QLabel(ReadRawDataDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        num_channels = new QLineEdit(ReadRawDataDlg);
        num_channels->setObjectName(QString::fromUtf8("num_channels"));

        gridLayout->addWidget(num_channels, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        siemens_va_format = new QRadioButton(ReadRawDataDlg);
        siemens_va_format->setObjectName(QString::fromUtf8("siemens_va_format"));

        horizontalLayout->addWidget(siemens_va_format);

        siemens_vb_format = new QRadioButton(ReadRawDataDlg);
        siemens_vb_format->setObjectName(QString::fromUtf8("siemens_vb_format"));
        siemens_vb_format->setChecked(true);

        horizontalLayout->addWidget(siemens_vb_format);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 47, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(ReadRawDataDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ReadRawDataDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), ReadRawDataDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ReadRawDataDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(ReadRawDataDlg);
    } // setupUi

    void retranslateUi(QDialog *ReadRawDataDlg)
    {
        ReadRawDataDlg->setWindowTitle(QApplication::translate("ReadRawDataDlg", "Read Raw Data", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ReadRawDataDlg", "Raw data template:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ReadRawDataDlg", "Raw data:", 0, QApplication::UnicodeUTF8));
        browse_raw_data_button->setText(QApplication::translate("ReadRawDataDlg", "...", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ReadRawDataDlg", "Num. channels:", 0, QApplication::UnicodeUTF8));
        siemens_va_format->setText(QApplication::translate("ReadRawDataDlg", "Siemens VA format", 0, QApplication::UnicodeUTF8));
        siemens_vb_format->setText(QApplication::translate("ReadRawDataDlg", "Siemens VB format", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReadRawDataDlg: public Ui_ReadRawDataDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_READRAWDATADLG_H
