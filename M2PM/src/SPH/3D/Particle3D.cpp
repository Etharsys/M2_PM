#include <SPH/3D/Particle3D.hpp>

Particle::Particle(float x, float y, float z) : position(x, y, z), velocity(0.f, 0.f, 0.f), force(0.f, 0.f, 0.f), density(0.f), pressure(0.f) {}
