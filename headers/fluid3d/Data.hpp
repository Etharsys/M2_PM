#pragma once

#include <vector>



namespace fluid3d
{

    struct FluidData
    {

        /**
         * @brief constructor of fluid data, the grid created is as a cube
         * @arg _N : the length of the side of the cube, then the size of the grid is N*N*N
         * @arg _diff : the diffusion fluid indice
         * @arg _visc : the viscosity fluid indice
         * @arg _dt : the delta time ellapsed
         */
        explicit FluidData(int _N, float _diff, float _visc, float _dt);

        /**
         * @brief destoy grids 
         */
        ~FluidData();

        /**
         * @brief handle boundaries of the grid, the sixth sides and the sixth corners
         * @arg b : bound id
         * @arg x : a pointer to a grid to set boundaries  
         */
        void set_bnd(int b, float *x);

        /**
         * @brief : linear solver for the use of diffusing and projection
         * @arg b : bound id
         * @arg x : a pointer to a grid to solve
         * @arg x0 : a pointer to the previous state of x
         * @arg a : a scalar coefficient
         * @arg c : 6 * a (6 directions in 3D)
         */
        void lin_solve(int b, float *x, float *x0, float a, float c);

        /**
         * @brief apply a fluid diffusion ('speading' effect)
         * @arg b : bound id
         * @arg x : a pointer to a grid to apply diffusion
         * @arg x0 : a pointer to the previous state of x 
         */
        void diffuse (int b, float *x, float *x0, float environment);

        /**
         * @brief apply the fluid velocities to the grid
         * @arg b : bound id
         * @arg d : a pointer to a grid to apply velocities
         * @arg d0 : a pointer to the previous state of d
         * @arg velocX : a pointer to the velocities grid on x axis
         * @arg velocY : a pointer to the velocities grid on y axis
         * @arg velocZ : a pointer to the velocities grid on z axis
         */
        void advect (int b, float *d, float *d0, float *velocX, float *velocY, float *velocZ);

        /**
         * @brief modify the fluid velocities
         * @arg velocX : a pointer to the velocities grid on x axis
         * @arg velocY : a pointer to the velocities grid on y axis
         * @arg velocZ : a pointer to the velocities grid on z axis
         * @arg p : a pointer to a grid to modify velocities
         * @arg div : a pointer to the previous state of p
         */
        void project(float *velocX, float *velocY, float *velocZ, float *p, float *div);

        /**
         * @brief do a step on all grid for simulation
         */
        void FluidDataStep();

        /**
         * @brief add density to a location on the density grid
         * @arg x, y, z : the location to add density
         * @arg amount : the density amount to add 
         */
        void FluidDataAddDensity(int x, int y, int z, float amount);

        /**
         * @brief add velocity to a location on the density grid
         * @arg x, y, z : the location to add velocity
         * @arg amountX : the density on x axis amount to add 
         * @arg amountY : the density on y axis amount to add 
         * @arg amountZ : the density on z axis amount to add 
         */
        void FluidDataAddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ);

        /**
         * @brief flat the density grid
         * @return a vector
         */
        std::vector<float> flat() const;


    private:
        int N;

        // environment
        float dt, diff, visc;
        int iter = 4;

        // velocities
        float *Vx, *Vy, *Vz, *Vx0, *Vy0, *Vz0;

        // densities
        float *s, *density;

    };
}