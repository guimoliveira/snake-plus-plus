#include "SnakePlusPlus.h"

Snake::Snake(Game &game, Board &board, Position pos, int bodySize, COLORREF color):
    mGame(game),
    mBoard(board),
    mPos(pos),
    mColor(color)
{
    if(bodySize < 1) bodySize = 1;

    for (int i = 0; i <= bodySize; i++, pos.x--){
        mBoard.set(pos, Board::Snake);
        mBody.push_back(pos);
    }
}

Snake& Snake::operator = (const Snake& rhs){
    if (&mGame != &rhs.mGame || &mBoard != &rhs.mBoard) throw "Invalid operation";

    mAlive = rhs.mAlive;
    mSize = rhs.mSize;
    mPos = rhs.mPos;
    mSpeed = rhs.mSpeed;
    mDirection = rhs.mDirection;
    mNextDirection = rhs.mNextDirection;
    mBody = rhs.mBody;
    mColor = rhs.mColor;

    return *this;
}

bool Snake::isAlive() const {
    return mAlive;
}

void Snake::draw(HDC dc) const {
    SetBkColor(dc, mColor);

    mGame.mSnakeHeadStyle.select(dc);

    Position lastPos = mPos;
    Position boardPos = mBoard.getCellPos(mPos);

    MoveToEx(dc, (int)boardPos.x, (int)boardPos.y, NULL);
    LineTo(dc, (int)boardPos.x, (int)boardPos.y);

    mGame.mSnakeBodyStyle.select(dc);

    for(auto it = mBody.begin(); it != mBody.end(); ++it){
        boardPos = mBoard.getCellPos(*it);

        if(lastPos.dist(*it) > 2) MoveToEx(dc, (int)boardPos.x, (int)boardPos.y, NULL);

        LineTo(dc, (int)boardPos.x, (int)boardPos.y);
        lastPos = *it;
    }
}

void Snake::update(int dt, bool _think){
    if (!mAlive) return;

    mGame.addRegion(mBoard.getCellRect(mPos));

    if (mDirection != Direction::Stopped && mSpeed < 15) {
        mSpeed += (0.07f * dt)/1000;
    }

    float dpos = (mSpeed * dt)/1000;
    if (dpos > 1) dpos = 1;

    switch (mDirection) {
        case Left:  mPos.x -= dpos; break;
        case Right: mPos.x += dpos; break;
        case Up:    mPos.y -= dpos; break;
        case Down:  mPos.y += dpos; break;
        case Stopped:
            if(_think) think(); else mDirection = mNextDirection;
            return;
            break;
    }

    mGame.addRegion(mBoard.getCellRect(mPos));

    if (mBody[0].dist(mPos) < 1) return;

    mPos.round();

    if (mPos.x > mBoard.width - 1) mPos.x = 0;
    else if (mPos.x < 0) mPos.x = mBoard.width - 1;

    if (mPos.y > mBoard.height - 1) mPos.y = 0;
    else if (mPos.y < 0) mPos.y = mBoard.height - 1;

    mGame.addRegion(mBoard.getCellRect(mPos));

    bool grow = false;

    switch (mBoard.at(mPos)) {
        case Board::Fruit:
            //if (!mGame.mGameOver) Beep(1000, 20);
            mSize++;

            grow = true;

            mGame.removeFruit(mPos);
            mGame.spawnObstacles();

            break;
        case Board::Obstacle:
        case Board::Snake:
            //Beep(700, 40);

            mSpeed = 0;
            mAlive = false;
            mPos = mBody[0];

            return;
            break;
        default:;

    }

    if (_think) think(); else mDirection = mNextDirection;

    Position lastPos = mPos;

    for (auto it = mBody.begin(); it != mBody.end(); ++it){
        const Position tempPos = *it;
        *it = lastPos;
        it->round();
        lastPos = tempPos;
    }

    mGame.addRegion(mBoard.getCellRect(lastPos));
    mGame.addRegion(mBoard.getCellRect(*(mBody.end() - 1)));

    if (grow) {
        mBody.push_back(lastPos);
    } else {
        mBoard.set(lastPos, Board::Empty);
    }

    mBoard.set(mPos, Board::Snake);
}

void Snake::die(){
    for (auto it = mBody.begin(); it != mBody.end(); ++it){
        mGame.addFruit(*it);
    }
}

void Snake::setDirection(Direction direction){
    if((direction == Left || direction == Right) &&
       (mDirection == Left || mDirection == Right)) return;

    if((direction == Up || direction == Down) &&
       (mDirection == Up || mDirection == Down)) return;

    mNextDirection = direction;
}

Position Snake::getNextPos(Position pos, Direction direction) const {
    pos.round();

    switch (direction) {
        case Direction::Left:       pos.x--;                    break;
        case Direction::Right:      pos.x++;                    break;
        case Direction::Up:         pos.y--;                    break;
        case Direction::Down:       pos.y++;                    break;
        case Direction::Stopped:;
    }

    if (pos.x > mBoard.width - 1) pos.x = 0;
    else if (pos.x < 0) pos.x = mBoard.width - 1;

    if (pos.y > mBoard.height - 1) pos.y = 0;
    else if (pos.y < 0) pos.y = mBoard.height - 1;

    return pos;
}

void Snake::think(){
    if(mDirection == Stopped) mDirection = Right;

    Position fruitPos = mGame.getClosestFruit(mPos);

    if ((int)fruitPos.x == std::round(mPos.x) && mDirection != Up && mDirection != Down) {
        if (fruitPos.y > mPos.y) mDirection = Down; else mDirection = Up;
    }
    if ((int)fruitPos.y == std::round(mPos.y) && mDirection != Left && mDirection != Right){
        if (fruitPos.x > mPos.x) mDirection = Right; else mDirection = Left;
    }

    Position nextPos = getNextPos(mPos, mDirection);
    Board::Cell nextCell = mBoard.at(nextPos.x, nextPos.y);

    if (nextCell != Board::Empty && nextCell != Board::Cell::Fruit) {
        for (Direction d = Left; d <= Down; d = Direction(d + 1)) {
            Position nextPos = getNextPos(mPos, d);
            Board::Cell cell = mBoard.at(nextPos.x, nextPos.y);

            if(cell == Board::Cell::Empty || cell == Board::Cell::Fruit) {
                mDirection = d;
                break;
            }
        }
    }
}
