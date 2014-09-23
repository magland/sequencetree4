#ifndef sourceedit_H
#define sourceedit_H

#include "../shared/defaulteditorinterface.h"
#include "haiqcoreinterface.h"
#include "haiqsignalmanagerinterface.h"
#include <QDebug>
#include <QObject>
#include <QTextEdit>
#include "cpphighlighter.h"
#include <QSettings>
#include <QApplication>
#include "haiqcommon.h"
#include "haiqinterfaces.h"
//#include "SourceEditSettingsDlg.h"
#include "sourceeditsettings.h"


class SourceEditorInterface : public DefaultEditorInterface {
	Q_OBJECT
public:
	CppHighlighter *highlighter;
	QSet<QString> tags_to_highlight;
	SourceEditorInterface();	
	void setTagsToHighlight(const QSet<QString> &tagnames);
	void applySettings(SourceEditSettings *settings);
	void goto_previous_bookmark();
	void goto_next_bookmark();
        void delete_end_of_line();
        void delete_end_of_word();
        void duplicate_line();
        void delete_current_line();
	
private slots:
	void slot_highlightMarker(QTextCursor cursor);
};


class SourceEdit : public QObject, public HaiQEditorPlugin {
    Q_OBJECT
        Q_INTERFACES(HaiQEditorPlugin)
	HaiQCoreInterface *core;
	SourceEditSettings m_settings;
public:
	SourceEdit() {}
	virtual ~SourceEdit() {}
	void initialize(HaiQCoreInterface *core_in);
	
	void activate(HaiQEditorInterface *interface);
	void deactivate();
	
	QString name() {return "SourceEdit";}
	QString description() {return "Source editor";}
	QString authors() {return "J Magland";}	
	QString lastModified() {return "";}
	double loadOrder() {return 100;};
	HaiQEditorInterface *newInterface();
	QStringList fileTypes();
	//void apply_settings(SourceEditorInterface *E);
	
private slots:
    //void slot_source_editor_settings();
    void slot_settings_changed();
    void slot_editor_context_menu(QPoint pos) {
		DefaultEditorInterface *interface=qobject_cast<DefaultEditorInterface *>(sender());
		if (!interface) return;
		QMenu *menu=interface->textedit_window()->createStandardContextMenu();
		
		QList<QAction *> actions=menu->actions();
		QAction *first_action=0;
		if (actions.count()>0) first_action=actions[0];
		
		HaiQActionList *action_list=core->mainWindowManager->actionList("Text Editor Context Menu");
		for (int j=0; j<action_list->actionCount(); j++) {
			if (first_action) menu->insertAction(first_action,action_list->action(j));
			else menu->addAction(action_list->action(j));
		}
		if (first_action) menu->insertSeparator(first_action);
		
		
 		menu->popup(pos);
	}
	void slot_qt_keywords_loaded();
	void slot_goto_previous_bookmark();
	void slot_goto_next_bookmark();
        void slot_delete_end_of_line();
        void slot_delete_end_of_word();
        void slot_duplicate_line();
        void slot_delete_current_line();
};



#endif
