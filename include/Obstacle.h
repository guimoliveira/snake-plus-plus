#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle
{
    public:
        Obstacle(Game &game, Board &board, Position pos);
        Obstacle(Game &game, Board &board, int x, int y);
        Obstacle& operator = (const Obstacle &Obstacle);

        void draw(HDC dc) const;

    private:
        Game &mGame;
        Board &mBoard;

        Position mPos;

};

#endif // OBSTACLE_H
