#ifndef HaiQTextEdit_H
#define HaiQTextEdit_H

#include <QTextEdit>
#include <QTimer>
#include <QTime>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QTextBlockUserData>
#include <QKeyEvent>
#include <QHash>
#include <QTextBlock>
#include <QList>

#include "haiqmarker.h"

class defaultedit_blockdata : public QTextBlockUserData
{
public:
	QList<haiq_marker> markers;
	QObject *data;

	defaultedit_blockdata() {data=0;}
	virtual ~defaultedit_blockdata() {
		if (data) delete data;
	}

	void add_marker(const haiq_marker &marker);
	bool has_marker(int marker_type);
	haiq_marker get_marker(int marker_type);
	void remove_marker(const QString &marker_id);
	void remove_markers_of_type(int marker_type);
};


class HaiQTextEdit : public QTextEdit
{
	Q_OBJECT
private:
	bool has_temporary_marker;
	QHash<QString,haiq_marker> markers;
	bool do_emit_modification_changed;
public:
	QHash<QString,QTextBlock> marker_blocks;
	int num_tab_spaces;
	bool emit_key_only_mode;
	QFont current_font;
	QList<QTextBlock> blocks_highlighted_for_braces;
	QTimer braces_highlighting_timer;
	QTime last_keypress;

	HaiQTextEdit(QWidget *parent=0);
	virtual ~HaiQTextEdit();
	void set_font(QFont font);
	void process_key(QKeyEvent event);
	void process_backspace();
	void process_tab(QKeyEvent event);
	void highlight_braces(const QString &keytxt,bool backward);
	void do_marker_highlighting(QTextCursor &cursor);
	QString get_selected_text() {
		return textCursor().selection().toPlainText();
	}
	void select_current_word();
	void goto_previous_bookmark();
	void goto_next_bookmark();
        void delete_end_of_line();
        void delete_end_of_word();
        void duplicate_line();
        void delete_current_line();

public:
	QList<haiq_marker> get_markers();
	void add_marker(haiq_marker marker);
	void remove_marker(const QString &marker_id);
	void remove_markers_of_type(int marker_type);
	void goto_marker(QString id);
	void toggle_marker(int marker_type,QString path);


protected:
	void mouseDoubleClickEvent ( QMouseEvent * event );
	void keyReleaseEvent ( QKeyEvent * event );
	void keyPressEvent ( QKeyEvent * event );
	void insertFromMimeData ( const QMimeData * source );
	void contextMenuEvent(QContextMenuEvent *event);
private:
	QString tab_text(QTextCursor cursor);
	void indent_to_previous_line();
	void scrollContentsBy ( int dx, int dy );
public slots:
	void slot_clear_braces_highlighting();
signals:
	void double_clicked();
	void key_pressed(QKeyEvent);
	void scrolled();
	void context_menu(QPoint);
	void highlightMarker(QTextCursor);
	void markersChanged();
};

///Return true if this character can be used in a keyword (i.e. function name)
bool is_keyword_character(char c);

QStringList split_into_symbols(const QString &str); //breaks a string into symbols, very simple lexer
QStringList split_into_symbols_after_removing_comments(const QString &str); //just as the name implies 
bool is_initial_identifier_character(QChar c);
bool is_identifier_character(QChar c);
bool is_initial_number_character(QChar c);
bool is_number_character(QChar c);



#endif
