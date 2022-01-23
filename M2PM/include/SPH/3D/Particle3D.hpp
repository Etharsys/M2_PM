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
	 * @param z z position
	 */
	Particle(float x, float y, float z);

	friend bool operator==(const Particle &p1, const Particle &p2)
	{
		return p1.position == p2.position;
	}

	friend bool operator!=(const Particle &p1, const Particle &p2)
	{
		return p1.position != p2.position;
	}

	/*particle position*/
	Eigen::Vector3d position{0., 0., 0.};
	/*particle velocity*/
	Eigen::Vector3d velocity{0., 0., 0.};
	/*particle force applied*/
	Eigen::Vector3d force{0., 0., 0.};
	/*particle density*/
	float density = 0.f;
	/*particle pressure*/
	float pressure = 0.f;
	/*previous particle in the doubly linked list*/
	Particle *previous;
	/*next particle in the doubly linked list*/
	Particle *next;
};