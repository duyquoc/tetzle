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

#ifndef TILE_H
#define TILE_H

#include <QPoint>
#include <QRect>
#include <QXmlStreamWriter>
class Piece;

class Tile
{
public:
	Tile(int column, int row);

	QPointF bevel() const;
	int column() const;
	int row() const;
	Piece* parent() const;
	QPoint pos() const;
	QPoint gridPos() const;
	QPoint scenePos() const;

	void rotate();
	void setBevel(int bevel);
	void setPos(const QPoint& pos);
	void setParent(Piece* parent);

	static const int size = 64;

	void save(QXmlStreamWriter& xml) const;

private:
	Piece* m_parent;
	int m_column;
	int m_row;
	QPoint m_pos;
	int m_bevel;
	QPointF m_bevel_coords;
};


inline QPointF Tile::bevel() const
{
	return m_bevel_coords;
}

inline int Tile::column() const
{
	return m_column;
}

inline int Tile::row() const
{
	return m_row;
}

inline Piece* Tile::parent() const
{
	return m_parent;
}

inline QPoint Tile::pos() const
{
	return m_pos;
}

inline QPoint Tile::gridPos() const
{
	return QPoint(m_column * size, m_row * size);
}

inline void Tile::setPos(const QPoint& pos)
{
	m_pos = pos;
}

inline void Tile::setParent(Piece* parent)
{
	m_parent = parent;
}

#endif
