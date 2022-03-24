#include "UserInteraction.hpp"

UserInteractions::UserInteractions()
{
    done = false;
    scaleRequest = 0.;
    moveFront = 0;
    moveLeft = 0;
    moveUp = 0;
    newMouseX = oldMouseX = 0;
    newMouseY = oldMouseY = 0;
}

void UserInteractions::getUserInteraction()
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
                            case SDLK_SPACE:
                                stepRequest = true;
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
                        //oldMouseX = newMouseX;
                        //oldMouseY = newMouseY; 
                        newMouseX = event.motion.xrel;
                        newMouseY = event.motion.yrel;
                        break;

                    default : break;
                }
            }
        }

void UserInteractions::applyUserInteraction(Camera &camera)
{
    if (scaleRequest != 0)
    {
        camera.zoom(scaleRequest);
        scaleRequest = 0;
    }
    if (newMouseX != oldMouseX)
    {
        camera.rotateLeft(-newMouseX);
        oldMouseX = newMouseX;
    }
    if (newMouseY != oldMouseY)
    {
        camera.rotateUp(-newMouseY);
        oldMouseY = newMouseY;
    }
    //camera.moveLeft(moveLeft);
    //camera.moveUp(moveUp);
    //camera.moveFront(moveFront);
}

bool UserInteractions::isdone() const{
    return done;
}

bool UserInteractions::stepDebug() {
    if (stepRequest)
    {
        stepRequest = false;
        return true;
    }
    return false; 
}