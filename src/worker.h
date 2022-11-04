#ifndef __WORKER__
#define __WORKER__

#include "raylib.h"

class Worker
{
    private:
        Model worker_model;
        Color team;
        Shader shader;
        Vector3 position;

    public:
        Worker(Color colour);
        void update();
        void draw();
        ~Worker();
        
};

#endif