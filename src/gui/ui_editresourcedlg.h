/********************************************************************************
** Form generated from reading UI file 'editresourcedlg.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITRESOURCEDLG_H
#define UI_EDITRESOURCEDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditResourceDlg
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *double_list;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EditResourceDlg)
    {
        if (EditResourceDlg->objectName().isEmpty())
            EditResourceDlg->setObjectName(QString::fromUtf8("EditResourceDlg"));
        EditResourceDlg->resize(400, 300);
        verticalLayout = new QVBoxLayout(EditResourceDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        double_list = new QTextEdit(EditResourceDlg);
        double_list->setObjectName(QString::fromUtf8("double_list"));

        verticalLayout->addWidget(double_list);

        buttonBox = new QDialogButtonBox(EditResourceDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EditResourceDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), EditResourceDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EditResourceDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(EditResourceDlg);
    } // setupUi

    void retranslateUi(QDialog *EditResourceDlg)
    {
        EditResourceDlg->setWindowTitle(QApplication::translate("EditResourceDlg", "Edit Resource", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditResourceDlg: public Ui_EditResourceDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITRESOURCEDLG_H
