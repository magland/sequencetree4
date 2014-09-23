#include "vsphantomview.h"

void VSPhantomView::setPhantom(VSPhantom *P) {
	m_phantom=P;
	updateView();
}

void VSPhantomView::updateView() {
	ui.phantom_name->setText("");
	if (!m_phantom) return;
	ui.phantom_name->setText(m_phantom->name());
	ui.property_editor->setObject(m_phantom);
}

