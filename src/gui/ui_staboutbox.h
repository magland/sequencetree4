/********************************************************************************
** Form generated from reading UI file 'staboutbox.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STABOUTBOX_H
#define UI_STABOUTBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_STAboutBox
{
public:
    QHBoxLayout *horizontalLayout;
    QWebView *webView;

    void setupUi(QDialog *STAboutBox)
    {
        if (STAboutBox->objectName().isEmpty())
            STAboutBox->setObjectName(QString::fromUtf8("STAboutBox"));
        STAboutBox->resize(602, 300);
        horizontalLayout = new QHBoxLayout(STAboutBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        webView = new QWebView(STAboutBox);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl("about:blank"));

        horizontalLayout->addWidget(webView);


        retranslateUi(STAboutBox);

        QMetaObject::connectSlotsByName(STAboutBox);
    } // setupUi

    void retranslateUi(QDialog *STAboutBox)
    {
        STAboutBox->setWindowTitle(QApplication::translate("STAboutBox", "About SequenceTree4", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class STAboutBox: public Ui_STAboutBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STABOUTBOX_H
