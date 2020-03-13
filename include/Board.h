#ifndef BOARD_H
#define BOARD_H

class Board
{
    public:
        Board(Game &game, int width, int height, int size);
        ~Board();

        const int width;
        const int height;

        const int size;

        const int finalWidth;
        const int finalHeight;

        const int length;

        enum Cell{
            Empty,
            Snake,
            Obstacle,
            Fruit
        };

        void draw(HDC dc) const;

        Cell at(int x, int y) const;
        Cell at(Position pos) const;

        void set(int x, int y, Cell cell);
        void set(Position pos, Cell cell);

        Rect getCellRect(float x, float y) const;
        Rect getCellRect(Position pos) const;

        Position getCellPos(float x, float y) const;
        Position getCellPos(Position pos) const;

    private:
        Game &mGame;
        Cell* mCells;

};

#endif // BOARD_H
