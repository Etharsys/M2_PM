#pragma once

#include <eigen3/Eigen/Dense>
#include <memory>
#include <iostream>

/*
 * Represent a fluid particle
 */
class Particle
{
public:
	Particle() = default;

	~Particle() = default;

	/*
	 * Initialize particle
	 * @param x x position
	 * @param y y position
	 */
	Particle(float x, float y);

	friend bool operator==(const Particle &p1, const Particle &p2)
	{
		return p1.position == p2.position;
	}

	friend bool operator!=(const Particle &p1, const Particle &p2)
	{
		return p1.position != p2.position;
	}

	/*particle position*/
	Eigen::Vector2d position{0., 0.};
	/*particle velocity*/
	Eigen::Vector2d velocity{0., 0.};
	/*particle force applied*/
	Eigen::Vector2d force{0., 0.};
	/*particle viscosity applied*/
	Eigen::Vector2d viscosity{0., 0.};
	/*particle density*/
	float density = 0.f;
	/*particle pressure*/
	float pressure = 0.f;
	/*is density already processed*/
	bool processed_density = false;
	/*is force already processed*/
	bool processed_force = false;
	/*previous particle in the doubly linked list*/
	Particle * previous;
	/*next particle in the doubly linked list*/
	Particle * next;
};