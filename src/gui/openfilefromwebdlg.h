#ifndef OpenFileFromWebDlg_H
#define OpenFileFromWebDlg_H

#ifdef QT_WEB_KIT

#include "ui_openfilefromwebdlg.h"
#include <QDebug>
#include <QWebFrame>

class OpenFileFromWebDlg : public QDialog {
	Q_OBJECT
private:
	Ui::OpenFileFromWebDlg ui;
public:
	OpenFileFromWebDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		connect(ui.webView,SIGNAL(loadStarted()),this,SLOT(slot_load_started()));
		connect(ui.webView,SIGNAL(loadFinished(bool)),this,SLOT(slot_load_finished(bool)));
		connect(ui.webView,SIGNAL(loadProgress(int)),this,SLOT(slot_load_progress(int)));
		connect(ui.webView,SIGNAL(urlChanged(QUrl)),this,SLOT(slot_url_changed()));
		
		connect(ui.back_button,SIGNAL(clicked()),ui.webView,SLOT(back()));
		connect(ui.forward_button,SIGNAL(clicked()),ui.webView,SLOT(forward()));
		
		connect(ui.cancel_button,SIGNAL(clicked()),this,SLOT(slot_cancel()));
		
		set_message("");
	}
	void setUrl(QString url) {
		set_message("Opening page: "+url);
		ui.webView->load(QUrl(url));
		
	}
	void setFileExtension(QString ext) {
		m_file_extension=ext;
	}
	QString fileText() {return m_file_text;}
	QString fileName() {return m_file_name;}
private slots:
	void slot_load_started() {
	}
	void slot_load_finished(bool ok) {
		m_file_text="";
		m_file_name="";
		QString urlstr=ui.webView->url().toString();
		ui.url->setText(urlstr);
		if (ok) {
			if (urlstr.mid(urlstr.count()-m_file_extension.count())==m_file_extension) {
				m_file_text=ui.webView->page()->currentFrame()->toPlainText();
				int lindex=urlstr.lastIndexOf("/");
				if (lindex>=0)
					m_file_name=urlstr.mid(lindex+1);
				accept();
			}
			set_message("Page loaded.");
		}
		else {
			set_message("Unable to load web page.");
		}
	}
	void slot_load_progress(int val) {
		Q_UNUSED(val);
	}
	void slot_url_changed() {
	}
	void slot_ok() {
		accept();
	}
	void slot_cancel() {
		reject();
	}
private:
	QString m_file_text;
	QString m_file_name;
	QString m_file_extension;
	
	void set_message(QString txt) {
		ui.status_message->setText(txt);
	}
};

#endif

#endif
