#ifndef GAME_H
#define GAME_H

class Game {

    friend class Board;
    friend class Snake;
    friend class Fruit;
    friend class Obstacle;

    public:
        Game(HINSTANCE instance, bool fullscreen, int size = 0, int cellsX = 0, int cellsY = 0, int positionX = 0, int positionY = 0);
        ~Game();

        int run();

    private:

        static LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        static bool isKeyPressed(int key);

        enum TimerId {
            Update,
            SpawnSnake,
            SpawnFruits,
            RemoveDead
        };

        HINSTANCE mInstance;
        HWND      mWindow;
        HRGN      mRegion;

        bool mGameOver = false;
        bool mRunning = false;

        int mWidth;
        int mHeight;

        Timer  mTimer;
        Board* mBoard = nullptr;

        Style mBoardStyle;
        Style mSnakeHeadStyle;
        Style mSnakeBodyStyle;
        Style mFruitStyle;
        Style mObstacleStyle;

        std::vector<Snake>    mSnakes;
        std::vector<Fruit>    mFruits;
        std::vector<Obstacle> mObstacles;

        void update();
        void draw();

        void addRegion(Rect region);
        void clearRegion();
        void fillRegion();

        void spawnObstacles();
        void spawnFruits();
        void spawnSnake();

        void removeDead();

        const std::vector<Fruit>& getFruits();

        void addFruit(Position pos);
        void removeFruit(Position pos);

        Position getClosestFruit(Position pos) const;

};

#endif // GAME_H
