#ifndef DEFAULTEDITORWINDOW_H
#define DEFAULTEDITORWINDOW_H

#include "haiqtextedit.h"
#include "haiqtexteditgroup.h"
#include <QWidget>
#include "ui_defaulteditorwindow.h"

#include <QVBoxLayout>

class DefaultEditorWindow : public QWidget, private Ui::DefaultEditorWindow {
	
	Q_OBJECT
	
 public:
 
 	DefaultEditorWindow(QWidget *parent=0);
 	HaiQTextEdit *editor();
 	HaiQTextEditGroup *editorGroup();
 	void find(QString ttf, bool forward, bool backward);
 	void showFindPanel();
 	 void update_allTextForFind();
 	
 	 	 	
  private:

	HaiQTextEdit *m_editor;
	HaiQTextEditGroup *m_editorGroup;
	QVBoxLayout *vboxLayout;
	QString all_text_for_find;
	long m_hits;
	
 public slots:
	void slot_showReplacePanel();
	void slot_showFindPanel();
	void slot_FindNext();
 	void slot_FindPrevious();
	
 private slots:
 
 	void slot_hidePanel();
 	void slot_update_find_panel();
 	void slot_textEdited(const QString &text);
 	void slot_editor_key_pressed(QKeyEvent);
 	void slot_findInActiveProject();
 	void slot_replaceText();
 	void slot_findOptionsChanged();
 	
 protected:
	void keyPressEvent(QKeyEvent *);
	
 signals:
 	void findInActiveProject(QString);
	 	
};
#endif
