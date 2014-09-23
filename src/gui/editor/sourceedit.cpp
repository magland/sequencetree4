#include "sourceedit.h"
#include <QtPlugin>
#include <QDebug>
#include <QApplication>
#include <QScrollBar>
#include <QPainter>
#include <QTextLayout>

Q_EXPORT_PLUGIN2(sourceedit,SourceEdit)

SourceEditorInterface::SourceEditorInterface() {
	highlighter=0;
	setProperty("editor name","SourceEditorInterface");
}

void SourceEditorInterface::setTagsToHighlight(const QSet<QString> &tagnames) {
	tags_to_highlight=tagnames;
	if (highlighter) {
		highlighter->rehighlight();
	}
}

void SourceEditorInterface::slot_highlightMarker(QTextCursor cursor){
	bool hold_modified=the_editor->document()->isModified();
	QColor col=qRgb(255,255,255);
	QTextBlock block=cursor.block();
	defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
	QTextBlockFormat format=cursor.blockFormat();
	if (block_data) {
		if (block_data->has_marker(MARKER_TYPE_DEBUG_POSITION))
			col=qRgb(0,255,0);
		else if (block_data->has_marker(MARKER_TYPE_BUILD_ERROR))
			col=qRgb(255,210,210);
		else if (block_data->has_marker(MARKER_TYPE_BUILD_WARNING))
			col=qRgb(210,210,255);
		else if (block_data->has_marker(MARKER_TYPE_FIND_STRING))
			col=qRgb(210,255,210);
		else if (block_data->has_marker(MARKER_TYPE_TEMPORARY))
			col=qRgb(255,255,180);
		else if (block_data->has_marker(MARKER_TYPE_BREAKPOINT))
			col=qRgb(255,180,180);
	}
	format.setBackground(QBrush(col));
	cursor.setBlockFormat(format);
	m_editorGroup->updatePanel();
	the_editor->document()->setModified(hold_modified);
}



void SourceEdit::initialize(HaiQCoreInterface *core_in) {
	core=core_in;
	/*HaiQActionList *list=core->mainWindowManager->actionList("Tools Menu");
	QAction *Act;
        Act=new QAction(tr("Source editor settings..."),this); ///////////////////////////////////////////////
        connect(Act,SIGNAL(triggered()),this,SLOT(slot_source_editor_settings()));
        list->addAction(Act);
        list->update();*/
        
        m_settings.load(HAIQ_SETTINGS_DIR+"/sourcedit.ini");
        connect(&m_settings,SIGNAL(settingsChanged()),this,SLOT(slot_settings_changed()));

            core->settingsManager->addObjectProperty("Fonts","Source Editor",&m_settings,"font");
        for (int j=0; j<m_settings.metaObject()->propertyCount(); j++) {
        	QString name=m_settings.metaObject()->property(j).name();
        	core->settingsManager->addObjectProperty("Source Editor",name,&m_settings,name);
        }
        
        core->signalManager->connectSignal(SIGNAL(qtKeywordsLoaded()),this,SLOT(slot_qt_keywords_loaded()));
        core->signalManager->connectSignal(SIGNAL(gotoPreviousBookmark()),this,SLOT(slot_goto_previous_bookmark()));
        core->signalManager->connectSignal(SIGNAL(gotoNextBookmark()),this,SLOT(slot_goto_next_bookmark()));
}

HaiQEditorInterface *SourceEdit::newInterface() {
	SourceEditorInterface *ret=new SourceEditorInterface;
	CppHighlighter *highlighter=new CppHighlighter;
	highlighter->tags_to_highlight=&(ret->tags_to_highlight);
	ret->highlighter=highlighter;
	highlighter->addUserKeywords(QSet<QString>::fromList(core->settingsManager->property("temporary","qt_keywords").toStringList()));
	highlighter->setDocument(ret->textedit_window()->document());
	core->signalManager->registerSignal(ret->editorWindow(),SIGNAL(findInActiveProject(QString)));

	ret->applySettings(&m_settings);
	
	connect(ret,SIGNAL(context_menu(QPoint)),this,SLOT(slot_editor_context_menu(QPoint)));
	core->signalManager->registerSignal(ret->editor_group(),SIGNAL(markerClicked(QPoint,haiq_marker,int)));
	
	ret->editor_group()->showPanel(true);
	ret->editor_group()->displayMarkerIcons(true);
	
	return ret;
}

QStringList SourceEdit::fileTypes() {
	QStringList ret;
	ret << "c" << "cpp" << "cc" << "h" << "hpp";
	return ret;
}

void SourceEditorInterface::applySettings(SourceEditSettings *settings) {
	DefaultEditorInterface::applySettings(settings);
	
	QMap<QString,QTextCharFormat> formats;
	QTextCharFormat format;
	
        highlighter->do_highlight_tags=settings->highlightTagsInBold();

        the_editor->setTabStopWidth(settings->tabStopWidth());
	
        format.setBackground(settings->backgroundColor());
        QPalette pal = the_editor->palette();
        pal.setColor(QPalette::Window, settings->backgroundColor());
        pal.setColor(QPalette::Base, settings->backgroundColor());
	the_editor->setPalette( pal );
	formats["background"]=format;
	
	format.setForeground(settings->cppKeywordColor());
	formats["cpp_keyword"]=format;

        format.setForeground(settings->qtKeywordColor());
	formats["qt_keyword"]=format;
	
	format.setForeground(settings->operatorColor());
	formats["operator"]=format;
	
	format.setForeground(settings->numberColor());
	formats["number"]=format;
	
	format.setForeground(settings->escapeCharColor());
	formats["escape_char"]=format;
	
	format.setForeground(settings->macroColor());
	formats["macro"]=format;
	
	format.setForeground(settings->commentColor());
	formats["comment"]=format;
	
	format.setForeground(settings->stringColor());
	formats["string"]=format;

        format.setForeground(settings->functionNameColor());
	formats["function_name"]=format;
		
	highlighter->set_text_formats(formats);
	highlighter->rehighlight();
}

/*void SourceEdit::apply_settings(SourceEditorInterface *E) {
	SourceEditSettings settings(core->settingsManager);
	settings.load();
	E->highlighter->set_text_formats(settings.get_text_formats());
	E->highlighter->do_highlight_tags=settings.value("highlight_tags_in_bold").toBool();
	int point_size=settings.value("font_point_size").toInt();
	QString family=settings.value("font_family").toString();
	QFont font(family,point_size);
	E->setFont(font);
	E->setTabStopWidth(settings.value("tab_stop_width").toInt());
	E->highlighter->rehighlight();
}*/

/*void SourceEdit::slot_source_editor_settings() {
	SourceEditSettingsDlg dlg(core->settingsManager);
	if (dlg.exec()==QDialog::Accepted) {
		QStringList paths=core->workspaceManager->openFilePaths();
		foreach (QString path,paths) {
			HaiQEditorInterface *E=core->workspaceManager->editor(path);
			if (E) {
				if (E->property("editor name").toString()=="SourceEditorInterface") {
					SourceEditorInterface *E2=(SourceEditorInterface *)E;
					apply_settings(E2);
				}
			}
		}
	}
}*/

void SourceEdit::slot_settings_changed() {
	QStringList paths=core->workspaceManager->openFilePaths();
	foreach (QString path,paths) {
		HaiQEditorInterface *editor=core->workspaceManager->editor(path);
		SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
		if (editor2) {
			editor2->applySettings(&m_settings);
		}
	}
	m_settings.save(HAIQ_SETTINGS_DIR+"/sourceedit.ini");
}


void SourceEdit::slot_qt_keywords_loaded() {
	QStringList paths=core->workspaceManager->openFilePaths();
	foreach (QString path,paths) {
		HaiQEditorInterface *editor=core->workspaceManager->editor(path);
		SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
		if (editor2) {
			editor2->highlighter->addUserKeywords(QSet<QString>::fromList(core->settingsManager->property("temporary","qt_keywords").toStringList()));			
		}
	}
}

void SourceEdit::activate(HaiQEditorInterface *interface) {
}

void SourceEdit::deactivate() {
}

void SourceEdit::slot_goto_previous_bookmark() {
	HaiQEditorInterface *editor=core->workspaceManager->currentEditor();
	SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
	if (!editor2) return;
	editor2->goto_previous_bookmark();
}

void SourceEdit::slot_goto_next_bookmark() {
	HaiQEditorInterface *editor=core->workspaceManager->currentEditor();
	SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
	if (!editor2) return;
	editor2->goto_next_bookmark();
}

void SourceEdit::slot_delete_end_of_line() {
	HaiQEditorInterface *editor=core->workspaceManager->currentEditor();
	SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
	if (!editor2) return;
	editor2->delete_end_of_line();
}

void SourceEdit::slot_delete_end_of_word() {
	HaiQEditorInterface *editor=core->workspaceManager->currentEditor();
	SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
	if (!editor2) return;
	editor2->delete_end_of_word();
}

void SourceEdit::slot_duplicate_line() {
	HaiQEditorInterface *editor=core->workspaceManager->currentEditor();
	SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
	if (!editor2) return;
	editor2->duplicate_line();
}

void SourceEdit::slot_delete_current_line() {
	HaiQEditorInterface *editor=core->workspaceManager->currentEditor();
	SourceEditorInterface *editor2=qobject_cast<SourceEditorInterface *>(editor);
	if (!editor2) return;
	editor2->delete_current_line();
}

void SourceEditorInterface::goto_previous_bookmark() {
	the_editor->goto_previous_bookmark();
}
void SourceEditorInterface::goto_next_bookmark() {
	the_editor->goto_next_bookmark();
}

void SourceEditorInterface::delete_end_of_line() {
	the_editor->delete_end_of_line();
}
void SourceEditorInterface::delete_end_of_word() {
	the_editor->delete_end_of_word();
}

void SourceEditorInterface::duplicate_line() {
	the_editor->duplicate_line();
}

void SourceEditorInterface::delete_current_line() {
	the_editor->delete_current_line();
}

