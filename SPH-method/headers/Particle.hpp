#pragma once

#include <eigen3/Eigen/Dense>

class Particle
{
    public :

	Particle();

	Particle(float x, float y);

	friend bool operator==(const Particle &p1, const Particle &p2)
	{
		return p1.position == p2.position;
	}

    Eigen::Vector2d position;
    Eigen::Vector2d velocity;
    Eigen::Vector2d force;
	float density = 0.f;
    float pressure = 0.f;
};