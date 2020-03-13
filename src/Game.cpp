#include "SnakePlusPlus.h"

static const COLORREF snakeColors[7] = {RGB(200,0,0),
                                        RGB(0,200,0),
                                        RGB(0,0,200),
                                        RGB(200,200,0),
                                        RGB(0,200,200),
                                        RGB(200,0,200),
                                        RGB(200,200,200)};

LRESULT CALLBACK Game::wndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Game* game = (Game*)GetWindowLongPtr(hwnd, GWL_USERDATA);
    if (game == nullptr) return DefWindowProc (hwnd, message, wParam, lParam);

    switch (message) {
        case WM_EXITSIZEMOVE:
            game->fillRegion();

            break;
        case WM_DESTROY:
            game->mRunning = false;

            break;
        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE) game->mRunning = false;

            break;
        case WM_TIMER:
            switch ((TimerId) wParam) {
                case TimerId::Update:      game->update();      break;
                case TimerId::SpawnSnake:  game->spawnSnake();  break;
                case TimerId::SpawnFruits: game->spawnFruits(); break;
                case TimerId::RemoveDead:  game->removeDead();  break;
            }

            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

bool Game::isKeyPressed(int key) {
    return HIBYTE(GetAsyncKeyState(key));
}

Game::Game(HINSTANCE instance, bool fullscreen, int size, int cellsX, int cellsY, int positionX, int positionY):
    mInstance(instance)
{

    WNDCLASSEX wincl = {
        sizeof(WNDCLASSEX),
        CS_DBLCLKS,
        wndProc,
        0, 0,
        mInstance,
        LoadIcon(NULL, IDI_APPLICATION),
        LoadCursor(NULL, IDC_ARROW),
        CreateSolidBrush(0),
        NULL,
        L"Snake++",
        LoadIcon(NULL, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&wincl)) throw "Falha ao registrar classe.";

    DWORD style;

    if (size <= 0) size = 20;

    if (fullscreen) {
        mWidth = GetSystemMetrics(SM_CXSCREEN);
        mHeight = GetSystemMetrics(SM_CYSCREEN);
        cellsX = mWidth / size;
        cellsY = mHeight / size;
        positionX = positionY = 0;
        style = WS_POPUP;
    } else {
        if (cellsX <= 0 || cellsY <= 0) cellsX = cellsY = 10;
        mWidth = cellsX * size + 2 * GetSystemMetrics(SM_CXFIXEDFRAME);
        mHeight = cellsY * size + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CXFIXEDFRAME);
        style = WS_SYSMENU;
    }

    mWindow = CreateWindowEx (
           0,
           L"Snake++",
           L"Snake++",
           style,
           positionX, positionY,
           mWidth, mHeight,
           NULL, NULL,
           mInstance, NULL
    );

    SetWindowLongPtr(mWindow, GWL_USERDATA, (LONG)this);

    mRegion = CreateRectRgn(0, 0, mWidth, mHeight);
    mBoard = new Board(*this, cellsX, cellsY, size);

    srand(time(NULL));

    LOGBRUSH lb;
    lb.lbColor = RGB(0, 0, 0);
    lb.lbStyle = BS_HATCHED;
    lb.lbHatch = HS_DIAGCROSS;

    mBoardStyle = {CreatePen(PS_SOLID, 4, 0), CreateHatchBrush(HS_CROSS, RGB(50, 50, 50))};
    mSnakeHeadStyle = {ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND, size, &lb, 0, NULL), CreateSolidBrush(0)};
    mSnakeBodyStyle = {ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND, size * 0.7, &lb, 0, NULL), CreateSolidBrush(0)};
    mFruitStyle = {CreatePen(PS_SOLID, 2, RGB(255, 0, 0)), CreateSolidBrush(RGB(230, 0, 0))};
    mObstacleStyle = {CreatePen(PS_DOT, 1, RGB(150, 150, 150)), CreateSolidBrush(RGB(150, 100, 100))};

}

Game::~Game(){
    DeleteObject(mRegion);
    delete mBoard;
}

int Game::run() {

    ShowWindow (mWindow, 1);

    mSnakes.push_back({*this, *mBoard, {mBoard->width/2, mBoard->height/2}, 4, snakeColors[rand() % 7]});
    spawnFruits();

    SetTimer(mWindow, (int) TimerId(TimerId::Update), 10, (TIMERPROC) NULL);
    SetTimer(mWindow, (int) TimerId(TimerId::SpawnSnake), 10000, (TIMERPROC) NULL);
    SetTimer(mWindow, (int) TimerId(TimerId::SpawnFruits), 5000, (TIMERPROC) NULL);
    SetTimer(mWindow, (int) TimerId(TimerId::RemoveDead), 2000, (TIMERPROC) NULL);

    mRunning = true;

    MSG msg;

    while (mRunning && GetMessage (&msg, NULL, 0, 0)) {
        DispatchMessage(&msg);
    }

    return msg.wParam;

}

void Game::update(){

    const int dt = (mTimer.tick())/6;

    if (isKeyPressed(VK_LEFT))       mSnakes[0].setDirection(Snake::Left);
    else if(isKeyPressed(VK_RIGHT))  mSnakes[0].setDirection(Snake::Right);
    else if(isKeyPressed(VK_UP))     mSnakes[0].setDirection(Snake::Up);
    else if(isKeyPressed(VK_DOWN))   mSnakes[0].setDirection(Snake::Down);

    for(int i = 0; i < 6; i++){

        if (!mGameOver) {
            mSnakes[0].update(dt);

            for(auto it = mSnakes.begin() + 1; it != mSnakes.end(); ++it){
                it->update(dt, true);
            }
        } else {
            for(auto it = mSnakes.begin(); it != mSnakes.end(); ++it){
                it->update(dt*3, true);
            }
        }
    }

    draw();
}

void Game::draw(){
    HDC dc = GetDC(mWindow);

    SelectClipRgn(dc, mRegion);

    mBoard->draw(dc);

    for(auto it = mFruits.begin(); it != mFruits.end(); ++it){
        it->draw(dc);
    }

    for(auto it = mSnakes.begin(); it != mSnakes.end(); ++it){
        it->draw(dc);
    }

    for(auto it = mObstacles.begin(); it != mObstacles.end(); ++it){
        it->draw(dc);
    }

    clearRegion();

    ReleaseDC(mWindow, dc);
}

void Game::addRegion(Rect region){
    HRGN tempRegion = CreateRectRgn(region.x1, region.y1, region.x2, region.y2);
    CombineRgn(mRegion, mRegion, tempRegion, RGN_OR);
    DeleteObject(tempRegion);
}

void Game::clearRegion(){
    SetRectRgn(mRegion, 0, 0, 0, 0);
}

void Game::fillRegion(){
    SetRectRgn(mRegion, 0, 0, mWidth, mHeight);
}

void Game::spawnObstacles(){
    int q = rand() % 2;

    for(int i = 0; i < q; i++){
        int x, y;
        do{
            x = rand() % mBoard->width;
            y = rand() % mBoard->height;
        } while (mBoard->at(x, y) != Board::Cell::Empty);

        mObstacles.push_back({*this, *mBoard, {x, y}});
    }
}

void Game::spawnFruits(){
    int q = rand() % 6;

    for(int i = 0; i < q; i++){
        int x, y;
        do{
            x = rand() % mBoard->width;
            y = rand() % mBoard->height;
        } while (mBoard->at(x, y) != Board::Cell::Empty);

        addFruit({x, y});
    }
}

void Game::spawnSnake(){
    int x, y;
    do{
        x = rand() % (mBoard->width-1) +1;
        y = rand() % mBoard->height;
    } while (mBoard->at(x, y) != Board::Empty ||
             mBoard->at(x - 1, y) != Board::Empty);

    mSnakes.push_back({*this, *mBoard, {x, y}, 1, snakeColors[rand() % 7]});
}

const std::vector<Fruit>& Game::getFruits(){
    return mFruits;
}

void Game::addFruit(Position pos){
    mFruits.push_back({*this, *mBoard, pos});
}

void Game::removeFruit(Position pos){
    for (auto it = mFruits.begin(); it != mFruits.end(); ++it) {
        if (it->getPosition() == pos) {
            mFruits.erase(it);
            break;
        }
    }
}

Position Game::getClosestFruit(Position pos) const {
    if (mFruits.empty()) return {mBoard->width/2, mBoard->height/2};

    auto best = mFruits.begin();
    for(auto it = mFruits.begin(); it != mFruits.end(); ++it) {
        if(it->getPosition().dist(pos) < best->getPosition().dist(pos)){
            best = it;
        }
    }
    return best->getPosition();
}

void Game::removeDead(){
    for(auto it = mSnakes.begin(); it != mSnakes.end(); ){
        if (!it->isAlive()) {
            if(it == mSnakes.begin()) mGameOver = true;

            it->die();
            it = mSnakes.erase(it);
        } else it++;
    }
    if (mSnakes.size() == 0) PostQuitMessage(0);
}
