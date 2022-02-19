#include "3Dcube.hpp"

using namespace glimac;


int display(char* path, int width, int height, int depth, std::vector<float> to_display) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    FilePath applicationPath(path);
    CubeProgram cubeProgram(applicationPath);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    Cube sphere(1);

    TrackBallCamera cam;
    //std::cout << sphere.getVertexCount() << std::endl;
    //std::cout << sphere.getDataPointer() << std::endl;

    GLuint vbo;
    glGenBuffers(1,&vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount()*sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
     
     GLuint vao;
     glGenVertexArrays(1,&vao);
     glBindVertexArray(vao);
     
     glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);
     
     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     
     glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, position)));
     glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, normal)) );
     glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, texCoords)) );

     
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     
     glBindVertexArray(0);

    
    glEnable(GL_DEPTH_TEST); 
    

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f),(float)(800./600.),0.1f,100.f);
    glm::mat4 MVMatrix = cam.getViewMatrix();
    glm::mat4 NormalMatrix;

    cubeProgram.m_Program.use();


    // Application loop:
    bool done = false;
    SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            switch(e.type) {
                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_z:
                        cam.moveFront(0.1);
                        break;
                    case SDLK_s:
                        cam.moveFront(-0.1);
                        break;
                    case SDLK_d:
                        cam.rotateLeft(1.);
                        break;
                    case SDLK_q:
                        cam.rotateLeft(-1.);
                        break;
                    case SDLK_a:
                        cam.rotateUp(-1.);
                        break;
                    case SDLK_e:
                        cam.rotateUp(1.);
                        break;
                    case SDLK_SPACE:
                        done = true;
                    default:
                        break;
                    }
                    break;
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // Update the display
        glClearColor(0.5,0.5,0.5,0.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
         glBindVertexArray(vao);
         
         
         
         
         for (int z = 0; z < depth ; z++){
             for (int y = 0; y < height ; y++){
                 for (int x = 0; x < width ; x++){
                    if (to_display.at(x + (y*width) + (z*width*height))>0){
                        MVMatrix = cam.getViewMatrix(); 
                        MVMatrix = glm::translate(MVMatrix, glm::vec3(x,y,z));
                        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
                        glUniformMatrix4fv(cubeProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix*MVMatrix));
                        glUniformMatrix4fv(cubeProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
                        glUniformMatrix4fv(cubeProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
                        glUniformMatrix3fv(cubeProgram.uColor,1,GL_FALSE, glm::value_ptr(glm::vec3(to_display.at(x + (y*width) + (z*width*height)),0.,to_display.at(x + (y*width) + (z*width*height)))));
                        //std::cout << colorList[i] << std::endl;
                        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
                    }
                 }
             }
         }
         glBindVertexArray(0);
        windowManager.swapBuffers();
    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}
