#include <Fluid.hpp>
#include <chrono>
#include <iostream>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main(int argc, char const *argv[])
{
    Fluid fluid{100,500,100,500};
    
    for(int i = 0;i < 1000;i++)
    {
        auto t1 = high_resolution_clock::now();
        fluid.update();
        auto t2 = high_resolution_clock::now();
        std::cout << duration_cast<milliseconds>(t2 - t1).count() << std::endl;
    }
    
    return 0;
}
