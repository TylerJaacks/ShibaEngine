#ifndef SHIBAENGINE_APP_H
#define SHIBAENGINE_APP_H

#include "Engine.h"

class App {
public:
    App();
    ~App();

    void Run() const;

private:
    Engine* m_engine;
};

#endif //SHIBAENGINE_APP_H