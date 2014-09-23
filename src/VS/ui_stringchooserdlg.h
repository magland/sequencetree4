/********************************************************************************
** Form generated from reading UI file 'stringchooserdlg.ui'
**
** Created: Wed May 22 14:48:46 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STRINGCHOOSERDLG_H
#define UI_STRINGCHOOSERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_stringchooserdlg
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *caption_label;
    QListWidget *string_list;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *stringchooserdlg)
    {
        if (stringchooserdlg->objectName().isEmpty())
            stringchooserdlg->setObjectName(QString::fromUtf8("stringchooserdlg"));
        stringchooserdlg->resize(373, 219);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stringchooserdlg->sizePolicy().hasHeightForWidth());
        stringchooserdlg->setSizePolicy(sizePolicy);
        hboxLayout = new QHBoxLayout(stringchooserdlg);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        caption_label = new QLabel(stringchooserdlg);
        caption_label->setObjectName(QString::fromUtf8("caption_label"));

        vboxLayout->addWidget(caption_label);

        string_list = new QListWidget(stringchooserdlg);
        string_list->setObjectName(QString::fromUtf8("string_list"));

        vboxLayout->addWidget(string_list);

        buttonBox = new QDialogButtonBox(stringchooserdlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        hboxLayout->addLayout(vboxLayout);


        retranslateUi(stringchooserdlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), stringchooserdlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), stringchooserdlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(stringchooserdlg);
    } // setupUi

    void retranslateUi(QDialog *stringchooserdlg)
    {
        stringchooserdlg->setWindowTitle(QApplication::translate("stringchooserdlg", "Dialog", "no need to translate this string", QApplication::UnicodeUTF8));
        caption_label->setText(QApplication::translate("stringchooserdlg", "Here is where the caption goes...", "no need to translate this string", QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class stringchooserdlg: public Ui_stringchooserdlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRINGCHOOSERDLG_H
