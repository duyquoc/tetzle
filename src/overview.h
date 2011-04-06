/***********************************************************************
 *
 * Copyright (C) 2008, 2010, 2011 Graeme Gott <graeme@gottcode.org>
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

#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QGraphicsView>

class Overview : public QGraphicsView
{
	Q_OBJECT
public:
	Overview(QWidget* parent = 0);

	void load(const QImage& image);
	void reset();

signals:
	void toggled(bool visible);

protected:
	virtual void hideEvent(QHideEvent* event);
	virtual void moveEvent(QMoveEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void showEvent(QShowEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

private:
	void setPixmap(const QPixmap& pixmap);
	void zoomIn();
	void zoomOut();
	void zoom(int level);

private:
	QGraphicsPixmapItem* m_pixmap;
	int m_min_scale_level;
	int m_scale_level;
	bool m_default;
};

#endif
