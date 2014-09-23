/********************************************************************************
** Form generated from reading UI file 'defaulteditorwindow.ui'
**
** Created: Wed May 22 14:48:08 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFAULTEDITORWINDOW_H
#define UI_DEFAULTEDITORWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DefaultEditorWindow
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *editor_frame;
    QFrame *frameFind;
    QHBoxLayout *hboxLayout;
    QToolButton *toolClose;
    QLineEdit *editFind;
    QLabel *num_hits;
    QLabel *wrapText;
    QToolButton *toolPrevious;
    QToolButton *toolNext;
    QCheckBox *checkCase;
    QCheckBox *checkWholeWords;
    QToolButton *toolFindInProject;
    QSpacerItem *spacerItem;
    QFrame *frameReplace;
    QHBoxLayout *hboxLayout1;
    QLabel *replaceLabel;
    QLineEdit *editReplace;
    QToolButton *toolReplace;
    QCheckBox *checkReplaceAll;
    QCheckBox *checkSelectedText;
    QSpacerItem *spacerItem1;

    void setupUi(QWidget *DefaultEditorWindow)
    {
        if (DefaultEditorWindow->objectName().isEmpty())
            DefaultEditorWindow->setObjectName(QString::fromUtf8("DefaultEditorWindow"));
        DefaultEditorWindow->resize(581, 480);
        vboxLayout = new QVBoxLayout(DefaultEditorWindow);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        editor_frame = new QFrame(DefaultEditorWindow);
        editor_frame->setObjectName(QString::fromUtf8("editor_frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editor_frame->sizePolicy().hasHeightForWidth());
        editor_frame->setSizePolicy(sizePolicy);
        editor_frame->setFrameShape(QFrame::NoFrame);
        editor_frame->setFrameShadow(QFrame::Plain);

        vboxLayout->addWidget(editor_frame);

        frameFind = new QFrame(DefaultEditorWindow);
        frameFind->setObjectName(QString::fromUtf8("frameFind"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frameFind->sizePolicy().hasHeightForWidth());
        frameFind->setSizePolicy(sizePolicy1);
        frameFind->setFrameShape(QFrame::NoFrame);
        frameFind->setFrameShadow(QFrame::Plain);
        hboxLayout = new QHBoxLayout(frameFind);
        hboxLayout->setSpacing(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        toolClose = new QToolButton(frameFind);
        toolClose->setObjectName(QString::fromUtf8("toolClose"));
        toolClose->setAutoRaise(true);

        hboxLayout->addWidget(toolClose);

        editFind = new QLineEdit(frameFind);
        editFind->setObjectName(QString::fromUtf8("editFind"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(editFind->sizePolicy().hasHeightForWidth());
        editFind->setSizePolicy(sizePolicy2);
        editFind->setMinimumSize(QSize(100, 0));

        hboxLayout->addWidget(editFind);

        num_hits = new QLabel(frameFind);
        num_hits->setObjectName(QString::fromUtf8("num_hits"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(num_hits->sizePolicy().hasHeightForWidth());
        num_hits->setSizePolicy(sizePolicy3);
        num_hits->setMinimumSize(QSize(30, 0));
        num_hits->setMaximumSize(QSize(30, 16777215));
        num_hits->setAlignment(Qt::AlignCenter);

        hboxLayout->addWidget(num_hits);

        wrapText = new QLabel(frameFind);
        wrapText->setObjectName(QString::fromUtf8("wrapText"));
        sizePolicy3.setHeightForWidth(wrapText->sizePolicy().hasHeightForWidth());
        wrapText->setSizePolicy(sizePolicy3);
        wrapText->setMinimumSize(QSize(22, 22));
        wrapText->setMaximumSize(QSize(22, 22));
        wrapText->setScaledContents(true);

        hboxLayout->addWidget(wrapText);

        toolPrevious = new QToolButton(frameFind);
        toolPrevious->setObjectName(QString::fromUtf8("toolPrevious"));
        toolPrevious->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolPrevious->setAutoRaise(true);

        hboxLayout->addWidget(toolPrevious);

        toolNext = new QToolButton(frameFind);
        toolNext->setObjectName(QString::fromUtf8("toolNext"));
        toolNext->setMinimumSize(QSize(0, 0));
        toolNext->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolNext->setAutoRaise(true);
        toolNext->setArrowType(Qt::NoArrow);

        hboxLayout->addWidget(toolNext);

        checkCase = new QCheckBox(frameFind);
        checkCase->setObjectName(QString::fromUtf8("checkCase"));

        hboxLayout->addWidget(checkCase);

        checkWholeWords = new QCheckBox(frameFind);
        checkWholeWords->setObjectName(QString::fromUtf8("checkWholeWords"));

        hboxLayout->addWidget(checkWholeWords);

        toolFindInProject = new QToolButton(frameFind);
        toolFindInProject->setObjectName(QString::fromUtf8("toolFindInProject"));

        hboxLayout->addWidget(toolFindInProject);

        spacerItem = new QSpacerItem(10, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addWidget(frameFind);

        frameReplace = new QFrame(DefaultEditorWindow);
        frameReplace->setObjectName(QString::fromUtf8("frameReplace"));
        sizePolicy1.setHeightForWidth(frameReplace->sizePolicy().hasHeightForWidth());
        frameReplace->setSizePolicy(sizePolicy1);
        frameReplace->setFrameShape(QFrame::NoFrame);
        frameReplace->setFrameShadow(QFrame::Plain);
        hboxLayout1 = new QHBoxLayout(frameReplace);
        hboxLayout1->setSpacing(0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        replaceLabel = new QLabel(frameReplace);
        replaceLabel->setObjectName(QString::fromUtf8("replaceLabel"));

        hboxLayout1->addWidget(replaceLabel);

        editReplace = new QLineEdit(frameReplace);
        editReplace->setObjectName(QString::fromUtf8("editReplace"));
        editReplace->setMaximumSize(QSize(160, 16777215));

        hboxLayout1->addWidget(editReplace);

        toolReplace = new QToolButton(frameReplace);
        toolReplace->setObjectName(QString::fromUtf8("toolReplace"));
        QFont font;
        font.setPointSize(7);
        font.setBold(true);
        font.setWeight(75);
        font.setStrikeOut(false);
        font.setKerning(false);
        toolReplace->setFont(font);

        hboxLayout1->addWidget(toolReplace);

        checkReplaceAll = new QCheckBox(frameReplace);
        checkReplaceAll->setObjectName(QString::fromUtf8("checkReplaceAll"));

        hboxLayout1->addWidget(checkReplaceAll);

        checkSelectedText = new QCheckBox(frameReplace);
        checkSelectedText->setObjectName(QString::fromUtf8("checkSelectedText"));

        hboxLayout1->addWidget(checkSelectedText);

        spacerItem1 = new QSpacerItem(10, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);


        vboxLayout->addWidget(frameReplace);


        retranslateUi(DefaultEditorWindow);

        QMetaObject::connectSlotsByName(DefaultEditorWindow);
    } // setupUi

    void retranslateUi(QWidget *DefaultEditorWindow)
    {
        DefaultEditorWindow->setWindowTitle(QApplication::translate("DefaultEditorWindow", "Form", 0, QApplication::UnicodeUTF8));
        toolClose->setText(QString());
        num_hits->setText(QApplication::translate("DefaultEditorWindow", "000", 0, QApplication::UnicodeUTF8));
        wrapText->setText(QString());
#ifndef QT_NO_TOOLTIP
        toolPrevious->setToolTip(QApplication::translate("DefaultEditorWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\">Find previous [PgUp]</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolPrevious->setText(QString());
        toolPrevious->setShortcut(QApplication::translate("DefaultEditorWindow", "PgUp", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolNext->setToolTip(QApplication::translate("DefaultEditorWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\">Find next [PgDown]</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolNext->setText(QString());
        toolNext->setShortcut(QApplication::translate("DefaultEditorWindow", "PgDown", 0, QApplication::UnicodeUTF8));
        checkCase->setText(QApplication::translate("DefaultEditorWindow", "Case Sensitive", 0, QApplication::UnicodeUTF8));
        checkWholeWords->setText(QApplication::translate("DefaultEditorWindow", "Whole words", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolFindInProject->setToolTip(QApplication::translate("DefaultEditorWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Find in active project.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolFindInProject->setText(QApplication::translate("DefaultEditorWindow", "...", 0, QApplication::UnicodeUTF8));
        replaceLabel->setText(QApplication::translate("DefaultEditorWindow", "Replace With:", 0, QApplication::UnicodeUTF8));
        toolReplace->setText(QApplication::translate("DefaultEditorWindow", "REPLACE", 0, QApplication::UnicodeUTF8));
        checkReplaceAll->setText(QApplication::translate("DefaultEditorWindow", "Replace all entries", 0, QApplication::UnicodeUTF8));
        checkSelectedText->setText(QApplication::translate("DefaultEditorWindow", "Replace in selected text only", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DefaultEditorWindow: public Ui_DefaultEditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFAULTEDITORWINDOW_H
