#ifndef __WORKER__
#define __WORKER__

#include "raylib.h"
#include "utils.h"

class Worker
{
    private:
        Model worker_model;
        Color team;
        Shader shader;
        Vector3 position;
        Vector3 target;
        float speed;

    public:
        Worker(Color colour);
        void update(Ray mouse_ray);
        void draw();
        ~Worker();
        
};

#endif