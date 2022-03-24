#pragma once


#include <SDL/SDL.h>
#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "glm.hpp"


class UserInteractions 
{
    public:
        UserInteractions();


        void getUserInteraction();

        void applyUserInteraction(Camera &camera);
        

        bool isdone() const;

        bool stepDebug();

    private:

        bool done;
        bool stepRequest = false;

        float scaleRequest;

        int oldMouseX;
        int oldMouseY;
        int newMouseX;
        int newMouseY;
        
        float moveFront;
        float moveLeft;
        float moveUp;
        
};
