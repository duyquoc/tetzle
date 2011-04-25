/***********************************************************************
 *
 * Copyright (C) 2011 Graeme Gott <graeme@gottcode.org>
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

#include "thumbnail_delegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QListWidget>
#include <QPainter>

//-----------------------------------------------------------------------------

namespace
{
	enum ItemsRole
	{
		SmallDisplayRole = Qt::UserRole
	};
}

//-----------------------------------------------------------------------------

ThumbnailDelegate::ThumbnailDelegate(QListWidget* list)
	: QStyledItemDelegate(list),
	m_list(list),
	m_small_font_metrics(m_small_font)
{
	setFont(list->font());
	list->installEventFilter(this);
}

//-----------------------------------------------------------------------------

void ThumbnailDelegate::paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 option = opt;
	initStyleOption(&option, index);

	painter->save();

	// Find colors
	QIcon::Mode mode = QIcon::Normal;
	QPalette::ColorGroup cg = (option.state & QStyle::State_HasFocus) ? QPalette::Normal : QPalette::Inactive;
	QColor color = option.palette.color(cg, QPalette::Text);
	if (option.state & QStyle::State_Selected) {
		mode = QIcon::Selected;
		color = option.palette.color(cg, QPalette::HighlightedText);
	}

	// Calculate element locations
	Qt::Alignment alignment = Qt::AlignVCenter;
	int line_height = option.fontMetrics.lineSpacing();
	int small_line_height = m_small_font_metrics.lineSpacing();
	QRect thumbnail_rect, text_rect, small_text_rect;
	if (option.decorationPosition == QStyleOptionViewItem::Top) {
		// Centered text with icon on top
		alignment |= Qt::AlignHCenter;
		thumbnail_rect.setRect(option.rect.left() + 2 + (option.rect.width() - 74) / 2, option.rect.top() + 2, 74, 74);
		int text_width = option.rect.width() - 4;
		text_rect.setRect(option.rect.left() + 2, thumbnail_rect.bottom() + 1, text_width, line_height);
		small_text_rect.setRect(text_rect.left(), text_rect.bottom() + 1, text_width, small_line_height);
	} else if (option.direction == Qt::LeftToRight) {
		// Left aligned
		alignment |= Qt::AlignLeft;
		thumbnail_rect.setRect(option.rect.left() + 5, option.rect.top() + 2 + (option.rect.height() - 74) / 2, 74, 74);
		int text_height = line_height + small_line_height;
		int text_width = option.rect.width() - 4 - thumbnail_rect.width();
		text_rect.setRect(thumbnail_rect.right() + 7, option.rect.top() + 2 + (option.rect.height() - text_height) / 2, text_width, line_height);
		small_text_rect.setRect(text_rect.left(), text_rect.bottom() + 1, text_width, small_line_height);
	} else {
		// Right aligned
		alignment |= Qt::AlignRight;
		thumbnail_rect.setRect(option.rect.right() - 80, option.rect.top() + 2 + (option.rect.height() - 74) / 2, 74, 74);
		int text_height = line_height + small_line_height;
		int text_width = thumbnail_rect.left() - 6;
		text_rect.setRect(2, option.rect.top() + 2 + (option.rect.height() - text_height) / 2, text_width, line_height);
		small_text_rect.setRect(2, text_rect.bottom() + 1, text_width, small_line_height);
	}

	// Draw background
	QStyle* style = option.widget ? option.widget->style() : QApplication::style();
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

	// Draw decoration
	const QPixmap thumbnail = option.icon.pixmap(74, 74, mode);
	painter->drawPixmap(thumbnail_rect, thumbnail);

	// Draw text
	painter->setFont(option.font);
	QString text = painter->fontMetrics().elidedText(index.data(Qt::DisplayRole).toString(), option.textElideMode, text_rect.width(), option.displayAlignment);
	painter->setPen(color);
	painter->drawText(text_rect, alignment, text);

	// Draw small text
	painter->setFont(m_small_font);
	text = painter->fontMetrics().elidedText(index.data(SmallDisplayRole).toString(), option.textElideMode, small_text_rect.width(), option.displayAlignment);
	color.setAlphaF(0.6);
	painter->setPen(color);
	painter->drawText(small_text_rect, alignment, text);

	painter->restore();
}

//-----------------------------------------------------------------------------

QSize ThumbnailDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(index);
	int text_height = option.fontMetrics.lineSpacing() + m_small_font_metrics.lineSpacing();
	if (option.decorationPosition == QStyleOptionViewItem::Top) {
		return QSize(150, 78 + text_height);
	} else {
		return QSize(option.rect.width(), qMax(78, text_height + 4));
	}
}

//-----------------------------------------------------------------------------

bool ThumbnailDelegate::eventFilter(QObject* object, QEvent* event)
{
	if (object == m_list) {
		if (event->type() == QEvent::FontChange) {
			setFont(m_list->font());
		}
		return false;
	} else {
		return QStyledItemDelegate::eventFilter(object, event);
	}
}

//-----------------------------------------------------------------------------

void ThumbnailDelegate::setFont(const QFont& font)
{
	QFontInfo info(font);
	m_small_font = QFont(info.family(), info.pointSize() - 2);
	m_small_font.setItalic(true);
	m_small_font_metrics = QFontMetrics(m_small_font);
}

//-----------------------------------------------------------------------------