#pragma once

#include <eigen3/Eigen/Dense>
#include <memory>
#include <iostream>

class Particle
{
    public :

	Particle() = default;

	~Particle() = default;

	Particle(float x, float y);

	friend bool operator==(const Particle &p1, const Particle &p2)
	{
		return p1.position == p2.position;
	}

	class HashFunction
	{
		public:
		size_t operator()(const Particle* particle) const
		{
			/*
			size_t xHash = std::hash<float>()(particle->position.x());
			size_t yHash = std::hash<float>()(particle->position.y()) << 1;
			return xHash ^ yHash;
			*/
			return particle->position.x()+particle->position.y();
		}
	};

    Eigen::Vector2d position{0.,0.};
    Eigen::Vector2d velocity{0.,0.};
    Eigen::Vector2d force{0.,0.};
	float density = 0.f;
    float pressure = 0.f;
};