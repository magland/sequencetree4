#ifndef STAboutBox_H
#define STAboutBox_H

#ifdef QT_WEB_KIT

#include "ui_staboutbox.h"
#include <QWebView>
#include "stcommon.h"
#include "stmetaclass.h"
#include "stmetasequence.h"
#include <QDesktopServices>

class STAboutBox : public QDialog {
	Q_OBJECT
private:
	Ui::STAboutBox ui;
public:
	STAboutBox(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		QString html=read_text_file(ST_ROOT_DIR+"/templates/aboutbox.html");
		html.replace("$Version$",get_ST_version());
		ui.webView->setHtml(html);
		ui.webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
		
		connect(ui.webView,SIGNAL(linkClicked(QUrl)),this,SLOT(slot_link_clicked(QUrl)));
	}
private slots:
	void slot_link_clicked(QUrl url) {
		QDesktopServices::openUrl(url);
	}
};
#endif

#endif
