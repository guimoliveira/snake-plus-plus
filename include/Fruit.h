#ifndef FRUIT_H
#define FRUIT_H

class Fruit
{
    public:
        Fruit(Game &game, Board &board, Position pos);
        Fruit(Game &game, Board &board, int x, int y);
        Fruit& operator = (const Fruit &fruit);

        void draw(HDC dc) const;

        const Position& getPosition() const;

    private:
        Game  &mGame;
        Board &mBoard;

        Position mPos;

};

#endif // FRUIT_H
