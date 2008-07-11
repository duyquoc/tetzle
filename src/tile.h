/***********************************************************************
 *
 * Copyright (C) 2008 Graeme Gott <graeme@gottcode.org>
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

#ifndef TILE_H
#define TILE_H

#include <QPoint>
#include <QRect>
#include <QXmlStreamWriter>

class Board;
class Piece;

class Tile
{
public:
	Tile(int column, int row, const QPoint& pos, Board* board);

	int column() const
		{ return m_column; }
	int row() const
		{ return m_row; }
	Piece* parent() const
		{ return m_parent; }
	QPoint pos() const
		{ return m_pos; }

	void setPos(const QPoint& pos)
		{ m_pos = pos; }
	void setParent(Piece* parent)
		{ m_parent = parent; }

	QPoint scenePos() const;
	QRect boundingRect() const
		{ return m_rect.translated(scenePos()); }
	QRect rect() const
		{ return m_rect; }

	void save(QXmlStreamWriter& xml, bool scene_pos = false) const;

private:
	int m_column;
	int m_row;
	QPoint m_pos;
	QRect m_rect;

	Piece* m_parent;
	Board* m_board;
};

#endif // TILE_H
