#pragma once


#include <SDL/SDL.h>
#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "glm.hpp"


class UserInteractions 
{
    public:
        UserInteractions()
        {
            done = false;
            scaleRequest = 0.;
            moveFront = 0;
            moveLeft = 0;
            moveUp = 0;
        }


        void getUserInteraction()
        {
            SDL_Delay(10);
            SDL_Event event;
            while(SDL_PollEvent(&event)) 
            {
                switch (event.type)
                {
                    case SDL_QUIT : 
                        done = true; 
                        break;

                    case SDL_KEYDOWN :
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE: 
                                done = true;
                                break;
                            case SDLK_z:
                                moveFront = .1;
                                break;
                            case SDLK_s:
                                moveFront = -.1;
                                break;
                            case SDLK_q:
                                moveLeft = .1;
                                break;
                            case SDLK_d:
                                moveLeft = -.1;
                                break;
                            case SDLK_DOWN:
                                moveUp = -.1;
                                break;
                            case SDLK_UP:
                                moveUp = .1;
                                break;
                            default : break;
                        }
                        break;

                    case SDL_KEYUP :
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_z:
                                moveFront = 0;
                                break;
                            case SDLK_s:
                                moveFront = 0;
                                break;
                            case SDLK_q:
                                moveLeft = 0;
                                break;
                            case SDLK_d:
                                moveLeft = 0;
                                break;
                            case SDLK_DOWN:
                                moveUp = 0;
                                break;
                            case SDLK_UP:
                                moveUp = 0;
                                break;
                            default : break;
                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN : 
                        switch (event.button.button)
                        {
                            case SDL_BUTTON_WHEELUP : 
                                scaleRequest = .1;
                                break;
                            case SDL_BUTTON_WHEELDOWN : 
                                scaleRequest = -.1;
                                break;
                            default : break;
                        }
                        break;

                    case SDL_MOUSEMOTION :
                        /*std::cout << newMouseX << "," << newMouseY << " | " 
                            << oldMouseX << "," << oldMouseY << " | "
                            << event.motion.xrel << "," << event.motion.yrel << std::endl;*/
                        oldMouseX = newMouseX;
                        oldMouseY = newMouseY; 
                        newMouseX = event.motion.xrel;
                        newMouseY = event.motion.yrel;
                        break;

                    default : break;
                }
            }
        }

        void applyUserInteraction(Camera &camera)
        {
            if (scaleRequest != 0)
            {
                //std::cerr << "Zooming camera of " << scaleRequest << std::endl;
                camera.zoom(scaleRequest);
                scaleRequest = 0;
            }
            if (newMouseX != oldMouseX)
            {
                //std::cerr << "Rotating camera on x axis of " << newMouseX << std::endl;
                camera.rotateLeft(-newMouseX);
                oldMouseX = newMouseX;
            }
            if (newMouseY != oldMouseY)
            {
                //std::cerr << "Rotating camera on y axis of " << newMouseY << std::endl;
                camera.rotateUp(-newMouseY);
                oldMouseY = newMouseY;
            }
            camera.moveLeft(moveLeft);
            camera.moveUp(moveUp);
            camera.moveFront(moveFront);
            //player.move(glm::vec3(moveLeft, moveUp, moveFront));
        }
        

        bool isdone() const{
            return done;
        }

    private:

        bool done;

        float scaleRequest;

        int oldMouseX;
        int oldMouseY;
        int newMouseX;
        int newMouseY;
        
        float moveFront;
        float moveLeft;
        float moveUp;
        
};
