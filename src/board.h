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

#ifndef BOARD_H
#define BOARD_H

#include <QGLWidget>
#include <QHash>
class AppearanceDialog;
class Message;
class Overview;
class Piece;
class Tile;

class Board : public QGLWidget
{
	Q_OBJECT
public:
	Board(QWidget* parent = 0);
	~Board();

	Piece* findCollidingPiece(Piece* piece) const;
	void removePiece(Piece* piece);

	int id() const;
	int margin() const;
	QRect marginRect(const QRect& rect) const;
	float tileTextureSize() const;
	const QPointF* corners(int rotation) const;
	void setAppearance(const AppearanceDialog& dialog);
	void updateSceneRectangle(Piece* piece);

public slots:
	void newGame(const QString& image, int difficulty);
	void openGame(int id);
	void saveGame();
	void retrievePieces();
	void zoomIn();
	void zoomOut();
	void zoomFit();
	void zoom(int level);
	void toggleOverview();

signals:
	void completionChanged(int value);
	void overviewToggled(bool visible);
	void retrievePiecesAvailable(bool available);
	void showMessage(const QString& message);
	void clearMessage();
	void zoomInAvailable(bool available);
	void zoomOutAvailable(bool available);
	void zoomChanged(int level, float factor);
	void finished();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

private:
	void startScrolling();
	void stopScrolling();
	void scroll(const QPoint& delta);
	void togglePiecesUnderCursor();
	void moveCursor(const QPoint& delta);
	void grabPiece();
	void releasePieces();
	void rotatePiece();
	void selectPieces();

	void loadImage();
	void updateCursor();
	QPoint mapCursorPosition() const;
	QPoint mapPosition(const QPoint& position) const;
	void updateCompleted();
	void updateSceneRectangle();
	void updateStatusMessage(const QString& message);
	Piece* pieceUnderCursor();
	void finishGame();
	void cleanup();

private:
	int m_id;
	int m_difficulty;
	QString m_image_path;
	Overview* m_overview;
	Message* m_message;
	bool m_has_bevels;
	bool m_has_shadows;

	GLuint m_image;
	GLuint m_bumpmap_image;
	GLuint m_shadow_image;
	float m_image_ts;
	QPointF m_corners[4][4];

	int m_columns;
	int m_rows;
	QList<Piece*> m_pieces;
	QList<Piece*> m_active_pieces;
	QRect m_scene;
	int m_total_pieces;
	int m_completed;

	QPoint m_pos;
	QPoint m_cursor_pos;
	QPoint m_select_pos;
	int m_scale_level;
	float m_scale;
	bool m_scrolling;
	bool m_selecting;
	bool m_finished;

	int m_action_key;
	Qt::MouseButton m_action_button;
};


inline int Board::id() const
{
	return m_id;
}

inline int Board::margin() const
{
	return 16;
}

inline QRect Board::marginRect(const QRect& rect) const
{
	return rect.adjusted(-margin(), -margin(), margin(), margin());
}

inline float Board::tileTextureSize() const
{
	return m_image_ts;
}

inline const QPointF* Board::corners(int rotation) const
{
	return m_corners[rotation];
}

#endif
