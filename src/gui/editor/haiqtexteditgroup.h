#ifndef haiqtexteditgroup_H
#define haiqtexteditgroup_H

#include "haiqtextedit.h"
#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QTextCursor>
#include <QPixmap>

struct HaiQEditPanelVisibleIcon {
	QRect rect;
	haiq_marker marker;
	int line_number;
};

class HaiQEditPanel : public QWidget
{
	Q_OBJECT
public:
	HaiQTextEdit *textedit;
	HaiQEditPanel(HaiQTextEdit *editor);
	virtual ~HaiQEditPanel();
	void set_font(const QFont &font_in);
	/// returns the current editor's line number
	int currentLine();
	/// sets the current editor's line number
	void setCurrentLine(int line);
	/// returns whether the numbering panel displays marker icons.
	bool hasBlockIcons();
	/// specifies whether the numbering panel displays marker icons.
	void setBlockIcons(bool);
	
protected:
	void paintEvent ( QPaintEvent * event );
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	//void mouseDoubleClickEvent ( QMouseEvent * event );
signals:
	void markerClicked(QPoint pos, haiq_marker marker, int line_number);
	/// signal emited when the user adds a breakpoint. Connects with the Editor Interface.
	void addBreakPoint(int);
private slots:
	void slot_markers_changed();
private:
	/// this property holds the current editor's line number.
	int m_currentLine;
	/// this property holds whether the numbering panel will display icons for markers.
	bool m_hasBlockIcons;
	
	int m_num_marker_columns;
	QList<HaiQEditPanelVisibleIcon> m_visible_icons;
	
	
	void draw_icon(QPainter &painter, const haiq_marker &marker, int line_number, int xpos, int ypos, const QPixmap &pixmap);
	int visible_icon_index_at(QPoint pos);
};

class HaiQTextEditGroup : public QWidget
{
	Q_OBJECT
	HaiQTextEdit *editor;
public:
	HaiQEditPanel *panel;
	HaiQTextEditGroup(HaiQTextEdit *editor_in,QWidget *parent=0);
	virtual ~HaiQTextEditGroup();
	void set_font(QFont font);
	///  specifies whether the numbering panel is displayed or not.
	void showPanel(bool);
	void displayMarkerIcons(bool);
	/// returns true if the numbering panel is displayed.
	bool isPanelVisible();
	/// updates the numbering panel.
	void updatePanel();

private slots:
	void slot_cursorPositionChanged();

signals:
	/// signal emited when a breakpoint is added.
	void addBreakPoint(int);
	/// signal emited when a marker needs to be highlighted.
	void highlightMarker(QTextCursor);
	
	void markerClicked(QPoint pos, haiq_marker marker, int line_number);

};

#endif
