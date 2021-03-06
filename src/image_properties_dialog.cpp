/***********************************************************************
 *
 * Copyright (C) 2008, 2010, 2011, 2014 Graeme Gott <graeme@gottcode.org>
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

#include "image_properties_dialog.h"

#include "tag_manager.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>

//-----------------------------------------------------------------------------

ImagePropertiesDialog::ImagePropertiesDialog(const QIcon& icon, const QString& name, TagManager* manager, const QString& image, QWidget* parent)
	: QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	m_image(image),
	m_manager(manager)
{
	setWindowTitle(tr("Image Properties"));

	QLabel* preview = new QLabel(this);
	preview->setAlignment(Qt::AlignCenter);
	preview->setPixmap(icon.pixmap(74,74));
	preview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// Add name
	m_name = new QLineEdit(name, this);

	// Add tags
	m_tags = new QListWidget(this);
	m_tags->setSortingEnabled(true);
	QStringList tags = m_manager->tags();
	for (const QString& tag : tags) {
		QListWidgetItem* item = new QListWidgetItem(tag, m_tags);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
		item->setCheckState(m_manager->images(tag).contains(image) ? Qt::Checked : Qt::Unchecked);
	}
	if (m_tags->count() > 0) {
		QListWidgetItem* item = m_tags->item(0);
		item->setSelected(true);
		m_tags->setCurrentItem(item);
	}

	// Add dialog buttons
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(buttons, &QDialogButtonBox::accepted, this, &ImagePropertiesDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &ImagePropertiesDialog::reject);

	// Layout dialog
	QFormLayout* layout = new QFormLayout(this);
	layout->addRow(preview);
	layout->addRow(tr("Name:"), m_name);
	layout->addRow(tr("Tags:"), m_tags);
	layout->addRow(buttons);

	// Resize dialog
	resize(QSettings().value("ImageProperties/Size", sizeHint()).toSize());
}

//-----------------------------------------------------------------------------

QString ImagePropertiesDialog::name() const
{
	return m_name->text();
}

//-----------------------------------------------------------------------------

void ImagePropertiesDialog::accept()
{
	QStringList tags;
	int count = m_tags->count();
	for (int i = 0; i < count; ++i) {
		if (m_tags->item(i)->checkState() == Qt::Checked) {
			tags.append(m_tags->item(i)->text());
		}
	}
	m_manager->setImageTags(m_image, tags);
	QDialog::accept();
}

//-----------------------------------------------------------------------------

void ImagePropertiesDialog::hideEvent(QHideEvent* event)
{
	QSettings().setValue("ImageProperties/Size", size());
	QDialog::hideEvent(event);
}

//-----------------------------------------------------------------------------
