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

#ifndef BOARD_H
#define BOARD_H

#include <QGLWidget>
#include <QHash>
class QLabel;
class Piece;
class Tile;

class Board : public QGLWidget
{
	Q_OBJECT
public:
	Board(QWidget* parent = 0);
	~Board();

	void removePiece(Piece* piece);

	QList<Piece*> findCollidingPieces(Piece* piece) const;
	Piece* findCollidingPiece(Piece* piece) const;

	int id() const
		{ return m_id; }
	int tileSize() const
		{ return m_tile_size; }
	int margin() const
		{ return 7.0f / m_scale; }

public slots:
	void newGame(const QString& image, int difficulty);
	void openGame(int id);
	void saveGame();
	void retrievePieces();
	void zoomIn();
	void zoomOut();
	void zoomFit();
	void zoom(int value);
	void showOverview();
	void hideOverview();

signals:
	void overviewShown();
	void overviewHidden();
	void statusMessage(const QString& message);
	void retrievePiecesAvailable(bool available);
	void zoomInAvailable(bool available);
	void zoomOutAvailable(bool available);
	void zoomChanged(int zoom);
	void zoomRangeChanged(int min, int max);
	void finished();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	virtual bool eventFilter(QObject* watched, QEvent* event);
	virtual void hideEvent(QHideEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

private:
	void startScrolling();
	void stopScrolling();
	void grabPiece();
	void releasePieces();
	void rotatePiece();
	void selectPieces();

	void loadImage();
	void generateSuccessImage();
	void updateCursor();
	QPoint mapCursorPosition() const;
	QPoint mapPosition(const QPoint& position) const;
	void draw(Tile* tile, const QPoint& pos, float depth) const;
	void updateCompleted();
	Tile* tileAt(const QPoint& pos, bool include_active = true) const;
	Tile* tileUnderCursor(bool include_active = true);
	void finishGame();
	void cleanup();

	int m_id;
	int m_difficulty;
	bool m_letterbox;
	QString m_image_path;
	int m_image_width;
	int m_image_height;
	int m_tile_size;
	QLabel* m_overview;

	GLuint m_success;
	GLuint m_image;
	GLuint m_bumpmap;
	float m_image_ts;
	float m_bumpmap_ts;
	QPointF m_corners[4][4];
	QSize m_success_size;

	QList<Piece*> m_pieces;
	QHash<Piece*, Tile*> m_active_tiles;
	int m_total_pieces;
	int m_completed;

	QPoint m_pos;
	QPoint m_cursor_pos;
	QPoint m_select_pos;
	int m_scale_level;
	int m_scale_level_min;
	int m_scale_level_max;
	float m_scale;
	bool m_scrolling;
	bool m_selecting;
	bool m_finished;

	int m_action_key;
	Qt::MouseButton m_action_button;
};

#endif // BOARD_H
