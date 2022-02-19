#include <Fluid.hpp>
#include <chrono>
#include <iostream>
#include <omp.h>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

/*
* test performance without rendering 
*/

int main(int argc, char const *argv[])
{
    Fluid fluid{1000};
    
    for(int i = 0;i < 100;i++)
    {
        auto t1 = high_resolution_clock::now();
        fluid.update();
        auto t2 = high_resolution_clock::now();
        std::cout << 1000./duration_cast<milliseconds>(t2 - t1).count()  << std::endl;
    }
    
    return 0;
}
