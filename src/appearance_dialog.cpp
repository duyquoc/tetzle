/***********************************************************************
 *
 * Copyright (C) 2010, 2011 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "appearance_dialog.h"

#include "color_button.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSettings>

//-----------------------------------------------------------------------------

namespace
{

QPixmap coloredShadow(const QColor& color)
{
	QPixmap shadow = QPixmap(":/shadow.png").alphaChannel();

	QPixmap copy(shadow.size());
	copy.fill(color);
	copy.setAlphaChannel(shadow);

	return copy;
}

}

//-----------------------------------------------------------------------------

AppearanceDialog::AppearanceDialog(QWidget* parent)
	: QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	setWindowTitle(tr("Appearance"));

	// Create preview widget
	m_preview = new QLabel(this);
	m_preview->setAlignment(Qt::AlignCenter);
	m_preview->setAutoFillBackground(true);
	m_preview->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	// Create options widgets
	QGroupBox* options_group = new QGroupBox(tr("Options"), this);

	m_has_bevels = new QCheckBox(tr("Beveled borders"), options_group);
	connect(m_has_bevels, SIGNAL(stateChanged(int)), this, SLOT(updatePreview()));

	m_has_shadows = new QCheckBox(tr("Drop shadows"), options_group);
	connect(m_has_shadows, SIGNAL(stateChanged(int)), this, SLOT(updatePreview()));

	// Create colors widgets
	QGroupBox* colors_group = new QGroupBox(tr("Colors"), this);

	m_background = new ColorButton(colors_group);
	connect(m_background, SIGNAL(changed(const QColor&)), this, SLOT(updatePreview()));

	m_shadow = new ColorButton(colors_group);
	connect(m_shadow, SIGNAL(changed(const QColor&)), this, SLOT(updatePreview()));

	m_highlight = new ColorButton(colors_group);
	connect(m_highlight, SIGNAL(changed(const QColor&)), this, SLOT(updatePreview()));

	// Create buttons
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, Qt::Horizontal, this);
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttons->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()), this, SLOT(restoreDefaults()));

	// Lay out dialog
	QFormLayout* colors_layout = new QFormLayout(colors_group);
	colors_layout->addRow(tr("Background:"), m_background);
	colors_layout->addRow(tr("Shadow:"), m_shadow);
	colors_layout->addRow(tr("Highlight:"), m_highlight);

	QVBoxLayout* options_layout = new QVBoxLayout(options_group);
	options_layout->addWidget(m_has_bevels);
	options_layout->addWidget(m_has_shadows);

	QGridLayout* layout = new QGridLayout(this);
	layout->setSpacing(12);
	layout->setColumnStretch(1, 1);
	layout->setRowStretch(2, 1);

	layout->addWidget(options_group, 0, 0);
	layout->addWidget(colors_group, 1, 0);
	layout->addWidget(m_preview, 0, 1, 3, 1);
	layout->addWidget(buttons, 4, 0, 1, 2);

	// Load settings
	QSettings settings;
	m_background->setColor(settings.value("Colors/Background", "#9d8975").value<QColor>());
	m_shadow->setColor(settings.value("Colors/Shadow", Qt::black).value<QColor>());
	m_highlight->setColor(settings.value("Colors/Highlight", Qt::white).value<QColor>());
	m_has_bevels->setChecked(settings.value("Appearance/Bevels", true).toBool());
	m_has_shadows->setChecked(settings.value("Appearance/Shadows", true).toBool());
	updatePreview();
}

//-----------------------------------------------------------------------------

bool AppearanceDialog::hasBevels() const
{
	return m_has_bevels->isChecked();
}

//-----------------------------------------------------------------------------

bool AppearanceDialog::hasShadows() const
{
	return m_has_shadows->isChecked();
}

//-----------------------------------------------------------------------------

QPalette AppearanceDialog::colors() const
{
	QPalette palette;
	palette.setColor(QPalette::Base, m_background->color());
	palette.setColor(QPalette::Text, m_shadow->color());
	palette.setColor(QPalette::Highlight, m_highlight->color());
	return palette;
}

//-----------------------------------------------------------------------------

void AppearanceDialog::accept()
{
	QSettings settings;
	settings.setValue("Colors/Background", m_background->color());
	settings.setValue("Colors/Shadow", m_shadow->color());
	settings.setValue("Colors/Highlight", m_highlight->color());
	settings.setValue("Appearance/Bevels", m_has_bevels->isChecked());
	settings.setValue("Appearance/Shadows", m_has_shadows->isChecked());
	QDialog::accept();
}

//-----------------------------------------------------------------------------

void AppearanceDialog::restoreDefaults()
{
	m_background->setColor("#9d8975");
	m_shadow->setColor(Qt::black);
	m_highlight->setColor(Qt::white);
	m_has_bevels->setChecked(true);
	m_has_shadows->setChecked(true);
	updatePreview();
}

//-----------------------------------------------------------------------------

void AppearanceDialog::updatePreview()
{
	QPixmap bumpmap;
	if (m_has_bevels->isChecked()) {
		bumpmap.load(":/bumpmap.png");
	} else {
		bumpmap = QPixmap(512, 512);
		bumpmap.fill(QColor(128, 128, 128));
	}

	QPixmap pixmap(352, 256);
	pixmap.fill(m_background->color());
	{
		QPainter painter(&pixmap);

		// Draw example piece
		if (m_has_shadows->isChecked()) {
			QPixmap shadow = coloredShadow(m_shadow->color());
			for (int i = 0; i < 3; ++i) {
				painter.drawPixmap(0, i * 64, shadow);
			}
			painter.drawPixmap(64, 64, shadow);
		}
		painter.drawPixmap(32, 32, bumpmap, 288, 416, 64, 64);
		painter.drawPixmap(32, 96, bumpmap, 32, 32, 64, 64);
		painter.drawPixmap(96, 96, bumpmap, 160, 416, 64, 64);
		painter.drawPixmap(32, 160, bumpmap, 32, 416, 64, 64);

		// Draw example highlighted piece
		painter.translate(160, 0);

		if (m_has_shadows->isChecked()) {
			QPixmap highlight = coloredShadow(m_highlight->color());
			for (int i = 0; i < 3; ++i) {
				painter.drawPixmap(0, i * 64, highlight);
			}
			painter.drawPixmap(64, 64, highlight);
		}
		painter.drawPixmap(32, 32, bumpmap, 288, 416, 64, 64);
		painter.drawPixmap(32, 96, bumpmap, 32, 32, 64, 64);
		painter.drawPixmap(96, 96, bumpmap, 160, 416, 64, 64);
		painter.drawPixmap(32, 160, bumpmap, 32, 416, 64, 64);
	}

	QPalette palette = m_preview->palette();
	palette.setColor(m_preview->backgroundRole(), m_background->color());
	m_preview->setPalette(palette);

	m_preview->setPixmap(pixmap);
}

//-----------------------------------------------------------------------------
