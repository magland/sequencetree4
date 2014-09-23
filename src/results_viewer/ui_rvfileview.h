/********************************************************************************
** Form generated from reading UI file 'rvfileview.ui'
**
** Created: Wed May 22 14:49:29 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RVFILEVIEW_H
#define UI_RVFILEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RVFileView
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *tree;
    QLineEdit *directory;
    QHBoxLayout *horizontalLayout;
    QToolButton *reload_button;
    QToolButton *save_button;
    QToolButton *open_button;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *RVFileView)
    {
        if (RVFileView->objectName().isEmpty())
            RVFileView->setObjectName(QString::fromUtf8("RVFileView"));
        RVFileView->resize(356, 300);
        verticalLayout = new QVBoxLayout(RVFileView);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tree = new QTreeWidget(RVFileView);
        tree->setObjectName(QString::fromUtf8("tree"));

        verticalLayout->addWidget(tree);

        directory = new QLineEdit(RVFileView);
        directory->setObjectName(QString::fromUtf8("directory"));

        verticalLayout->addWidget(directory);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        reload_button = new QToolButton(RVFileView);
        reload_button->setObjectName(QString::fromUtf8("reload_button"));

        horizontalLayout->addWidget(reload_button);

        save_button = new QToolButton(RVFileView);
        save_button->setObjectName(QString::fromUtf8("save_button"));

        horizontalLayout->addWidget(save_button);

        open_button = new QToolButton(RVFileView);
        open_button->setObjectName(QString::fromUtf8("open_button"));

        horizontalLayout->addWidget(open_button);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(RVFileView);

        QMetaObject::connectSlotsByName(RVFileView);
    } // setupUi

    void retranslateUi(QWidget *RVFileView)
    {
        RVFileView->setWindowTitle(QApplication::translate("RVFileView", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = tree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("RVFileView", "1", 0, QApplication::UnicodeUTF8));
        reload_button->setText(QApplication::translate("RVFileView", "Reload File List", 0, QApplication::UnicodeUTF8));
        save_button->setText(QApplication::translate("RVFileView", "Save Experiment...", 0, QApplication::UnicodeUTF8));
        open_button->setText(QApplication::translate("RVFileView", "Open Experiment...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RVFileView: public Ui_RVFileView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RVFILEVIEW_H
