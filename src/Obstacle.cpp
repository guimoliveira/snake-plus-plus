#include "SnakePlusPlus.h"

Obstacle::Obstacle(Game &game, Board &board, Position pos):
    mGame(game),
    mBoard(board),
    mPos(pos)
{
    pos.round();
    board.set(pos, Board::Obstacle);

    mGame.addRegion(mBoard.getCellRect(pos));
}

Obstacle::Obstacle(Game &game, Board &board, int x, int y):
    Obstacle(game, board, {x, y})
{}

Obstacle& Obstacle::operator = (const Obstacle &rhs){
    if (&mGame != &rhs.mGame || &mBoard != &rhs.mBoard) throw "Invalid operation";

    mPos = rhs.mPos;
    return *this;
}

void Obstacle::draw(HDC dc) const{
    mGame.mObstacleStyle.select(dc);
    SetBkColor(dc, RGB(255,255,255));

    Rect obstacleRect = mBoard.getCellRect(mPos);

    Rectangle(dc, obstacleRect.x1, obstacleRect.y1, obstacleRect.x2, obstacleRect.y2);
}
