//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_GRAPHICSCONTEXT_H
#define SHIBAENGINE_GRAPHICSCONTEXT_H

class GraphicsContext {
public:
    GraphicsContext();
    ~GraphicsContext();

    void Init();
    void SwapBuffers();
    void Shutdown();
};

#endif //SHIBAENGINE_GRAPHICSCONTEXT_H