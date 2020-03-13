#include "SnakePlusPlus.h"

Fruit::Fruit(Game &game, Board &board, Position pos):
    mGame(game),
    mBoard(board),
    mPos(pos)
{
    mPos.round();
    board.set(mPos, Board::Fruit);

    game.addRegion(board.getCellRect(pos));
}

Fruit::Fruit(Game &game, Board &board, int x, int y):
    Fruit(game, board, {x, y})
{}

Fruit& Fruit::operator = (const Fruit &rhs){
    if (&mGame != &rhs.mGame || &mBoard != &rhs.mBoard) throw "Invalid operation";

    mPos = rhs.mPos;
    return *this;
}

void Fruit::draw(HDC dc) const{
    mGame.mFruitStyle.select(dc);

    Rect fruitRect = mBoard.getCellRect(mPos);

    Ellipse(dc, fruitRect.x1 + 1, fruitRect.y1 + 1, fruitRect.x2 - 1, fruitRect.y2 - 1);
}

const Position& Fruit::getPosition() const {
    return mPos;
}
