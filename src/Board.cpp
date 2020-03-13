#include "SnakePlusPlus.h"

Board::Board(Game &_game, int _width, int _height, int _size):
    width(_width), height(_height), size(_size),
    finalWidth(width * size), finalHeight(height * size),
    length(width * height),
    mGame(_game),
    mCells(new Cell[length])
{
    for(int i = 0; i < length; ++i){
        mCells[i] = Cell::Empty;
    }
}

Board::~Board()
{
    delete []mCells;
}

void Board::draw(HDC dc) const{
    mGame.mBoardStyle.select(dc);
    SetBkColor(dc, 0);

    Rectangle(dc, 0, 0, finalWidth, finalHeight);
}

Board::Cell Board::at(int x, int y) const{
    assert(x >= 0 && x < width && y >= 0 && y < height);
    return mCells[x + y * width];
}

Board::Cell Board::at(Position pos) const{
    pos.round();
    return at((int)pos.x, (int)pos.y);
}

void Board::set(int x, int y, Cell cell){
    assert(x >= 0 && x < width && y >= 0 && y < height);

    mCells[x + y * width] = cell;
}

void Board::set(Position pos, Cell cell){
    pos.round();
    set((int)pos.x, (int)pos.y, cell);
}

Rect Board::getCellRect(float x, float y) const {
    return {int(x * size), int(y * size),
            int((1 + x) * size), int((1 + y) * size)};
}

Rect Board::getCellRect(Position pos) const {
    return getCellRect(pos.x, pos.y);
}

Position Board::getCellPos(float x, float y) const {
    return {(x + 0.5f) * size, (y + 0.5f) * size};
}

Position Board::getCellPos(Position pos) const{
    return getCellPos(pos.x, pos.y);
}

