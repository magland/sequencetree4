#include "sequencestree.h"

SequencesTree::SequencesTree(QWidget *parent) : QTreeWidget(parent) 
{
}

SequencesTree::~SequencesTree()
{
}

void SequencesTree::initialize() {
	setColumnCount(2);
	QStringList labels; labels << "Sequence" << "Found";
	setHeaderLabels(labels);	
}

void SequencesTree::addSequence(QString sequence_fname) {
	SequencesTreeItem *item=new SequencesTreeItem(sequence_fname);
	addTopLevelItem(item);
}

