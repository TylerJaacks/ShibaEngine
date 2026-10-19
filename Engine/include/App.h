#ifndef SHIBAENGINE_APP_H
#define SHIBAENGINE_APP_H

#include <Engine.h>
#include <Game.h>

class App {
public:
    explicit App(Game* game);
    ~App();

    void Run() const;

private:
    Game* m_game;
    Engine* m_engine;
};

#endif //SHIBAENGINE_APP_H