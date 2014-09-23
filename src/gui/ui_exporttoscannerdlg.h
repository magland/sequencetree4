/********************************************************************************
** Form generated from reading UI file 'exporttoscannerdlg.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTTOSCANNERDLG_H
#define UI_EXPORTTOSCANNERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExportToScannerDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *sequence_name;
    QLabel *label_2;
    QLineEdit *export_directory;
    QLabel *label_3;
    QComboBox *scanner;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExportToScannerDlg)
    {
        if (ExportToScannerDlg->objectName().isEmpty())
            ExportToScannerDlg->setObjectName(QString::fromUtf8("ExportToScannerDlg"));
        ExportToScannerDlg->resize(562, 143);
        verticalLayout = new QVBoxLayout(ExportToScannerDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ExportToScannerDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        sequence_name = new QLineEdit(ExportToScannerDlg);
        sequence_name->setObjectName(QString::fromUtf8("sequence_name"));

        gridLayout->addWidget(sequence_name, 0, 1, 1, 1);

        label_2 = new QLabel(ExportToScannerDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        export_directory = new QLineEdit(ExportToScannerDlg);
        export_directory->setObjectName(QString::fromUtf8("export_directory"));

        gridLayout->addWidget(export_directory, 1, 1, 1, 1);

        label_3 = new QLabel(ExportToScannerDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        scanner = new QComboBox(ExportToScannerDlg);
        scanner->setObjectName(QString::fromUtf8("scanner"));

        gridLayout->addWidget(scanner, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(ExportToScannerDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ExportToScannerDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExportToScannerDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExportToScannerDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExportToScannerDlg);
    } // setupUi

    void retranslateUi(QDialog *ExportToScannerDlg)
    {
        ExportToScannerDlg->setWindowTitle(QApplication::translate("ExportToScannerDlg", "Export to Scanner", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ExportToScannerDlg", "Sequence name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ExportToScannerDlg", "Export directory:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ExportToScannerDlg", "Scanner:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExportToScannerDlg: public Ui_ExportToScannerDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTTOSCANNERDLG_H
