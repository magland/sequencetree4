/********************************************************************************
** Form generated from reading UI file 'openfilefromwebdlg.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENFILEFROMWEBDLG_H
#define UI_OPENFILEFROMWEBDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_OpenFileFromWebDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *back_button;
    QToolButton *forward_button;
    QLineEdit *url;
    QWebView *webView;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancel_button;
    QLabel *status_message;

    void setupUi(QDialog *OpenFileFromWebDlg)
    {
        if (OpenFileFromWebDlg->objectName().isEmpty())
            OpenFileFromWebDlg->setObjectName(QString::fromUtf8("OpenFileFromWebDlg"));
        OpenFileFromWebDlg->resize(397, 323);
        verticalLayout = new QVBoxLayout(OpenFileFromWebDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        back_button = new QToolButton(OpenFileFromWebDlg);
        back_button->setObjectName(QString::fromUtf8("back_button"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        back_button->setIcon(icon);

        horizontalLayout->addWidget(back_button);

        forward_button = new QToolButton(OpenFileFromWebDlg);
        forward_button->setObjectName(QString::fromUtf8("forward_button"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        forward_button->setIcon(icon1);

        horizontalLayout->addWidget(forward_button);

        url = new QLineEdit(OpenFileFromWebDlg);
        url->setObjectName(QString::fromUtf8("url"));
        url->setReadOnly(true);

        horizontalLayout->addWidget(url);


        verticalLayout->addLayout(horizontalLayout);

        webView = new QWebView(OpenFileFromWebDlg);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl("about:blank"));

        verticalLayout->addWidget(webView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        cancel_button = new QPushButton(OpenFileFromWebDlg);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));

        horizontalLayout_2->addWidget(cancel_button);


        verticalLayout->addLayout(horizontalLayout_2);

        status_message = new QLabel(OpenFileFromWebDlg);
        status_message->setObjectName(QString::fromUtf8("status_message"));
        status_message->setFrameShape(QFrame::Panel);
        status_message->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(status_message);


        retranslateUi(OpenFileFromWebDlg);

        QMetaObject::connectSlotsByName(OpenFileFromWebDlg);
    } // setupUi

    void retranslateUi(QDialog *OpenFileFromWebDlg)
    {
        OpenFileFromWebDlg->setWindowTitle(QApplication::translate("OpenFileFromWebDlg", "Open file from web...", 0, QApplication::UnicodeUTF8));
        back_button->setText(QApplication::translate("OpenFileFromWebDlg", "...", 0, QApplication::UnicodeUTF8));
        forward_button->setText(QApplication::translate("OpenFileFromWebDlg", "...", 0, QApplication::UnicodeUTF8));
        cancel_button->setText(QApplication::translate("OpenFileFromWebDlg", "Cancel", 0, QApplication::UnicodeUTF8));
        status_message->setText(QApplication::translate("OpenFileFromWebDlg", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OpenFileFromWebDlg: public Ui_OpenFileFromWebDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENFILEFROMWEBDLG_H
