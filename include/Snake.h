#ifndef SNAKE_H
#define SNAKE_H

class Snake
{
    public:
        Snake(Game &game, Board &board, Position pos, int bodySize, COLORREF color);

        Snake& operator = (const Snake& s);

        enum Direction {
            Stopped,
            Left,
            Right,
            Up,
            Down
        };

        bool isAlive() const;

        void draw(HDC dc) const;
        void update(int dt, bool think = false);
        void die();

        void setDirection(Direction direction);

    private:
        Game  &mGame;
        Board &mBoard;

        Position mPos;
        COLORREF mColor;

        bool  mAlive = true;
        int   mSize;
        float mSpeed = 4.0f;

        std::vector<Position> mBody;

        Direction mDirection = Stopped;
        Direction mNextDirection = Stopped;

        Position getNextPos(Position pos, Direction direction) const;

        void think();
};

#endif // SNAKE_H
