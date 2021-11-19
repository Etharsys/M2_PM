
# Fluid dynamics simulation
## Table of contents
1. [Fluid definition](#fluid-definition)
2. [Navier-Stokes equations](#navier-stokes-equations)
3. [Methods mainly used](#methods-mainly-used)
   
	3.1  [Grid based method](#grid-based-method)

	3.2  [Particle based method](#particle-based-method)

	3.3 [Hybrid methods](#hybrid-methods)

4. [References](#references)

## Fluid definition
In [physics](https://en.wikipedia.org/wiki/Physics "Physics"), a **fluid** is a [liquid](https://en.wikipedia.org/wiki/Liquid "Liquid"), [gas](https://en.wikipedia.org/wiki/Gas "Gas"), or other material that continuously [deforms](https://en.wikipedia.org/wiki/Deformation_(physics) "Deformation (physics)") (_flows_) under an applied [shear stress](https://en.wikipedia.org/wiki/Shear_stress "Shear stress"), or external force.[[1]](https://en.wikipedia.org/wiki/Fluid#cite_note-1) They have zero [shear modulus](https://en.wikipedia.org/wiki/Shear_modulus "Shear modulus"), or, in simpler terms, are [substances](https://en.wikipedia.org/wiki/Matter "Matter") which cannot resist any [shear force](https://en.wikipedia.org/wiki/Shear_force "Shear force") applied to them.
Fluids display properties such as:
*   lack of resistance to permanent deformation, resisting only [relative rates of deformation](https://en.wikipedia.org/wiki/Viscosity "Viscosity") in a dissipative, frictional manner, and
*   the ability to flow (also described as the ability to take on the shape of the container).
## Navier-Stokes equations
Navier-Stokes describes the motion of [viscous fluid](https://en.wikipedia.org/wiki/Viscous_fluid  "Viscous fluid") substances :

---

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20\nabla\cdot%20u=0"/>

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20\nabla\cdot"/> &#8594; divergence

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20u"/> &#8594; velocity vector field (having a vector for each point in the space)

**mass continuity no fluid is lose, no compression.**

  

---

  
<img src="https://latex.codecogs.com/png.latex?\large\bg_white&space;\rho\frac{du}{dt}=-\nabla&space;p&space;&plus;\mu\nabla^2u&plus;F" />

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20\rho\frac{du}{dt}"/> &#8594; it's the motion similar to mass * acceleration with <img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20\rho"/> the density, being the mass and <img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20\frac{du}{dt}"/> (derivative of the velocity vector field) being the acceleration

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20-\nabla&space;p"/> &#8594; pressure

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20\mu\nabla^2u"/> &#8594; friction/viscosity

<img  src="https://latex.codecogs.com/png.latex?\large%5Cbg_white%20F"/> &#8594; external force (such as gravity)

---
Video

[![Navier-stokes](https://img.youtube.com/vi/Ra7aQlenTb8/0.jpg)](https://www.youtube.com/watch?v=Ra7aQlenTb8)

## Methods mainly used

### Grid based method 

In the **Eulerian viewpoint**, the simulation takes place in a specific finite volume of space, which is represented by a 3D or 2D grid or field. The grid space is divided into a number of rectangular cells and the fluid flows through it. The  quantities  of the fluid are defined at every point of the grid. Instead of keeping record of every single particle  and its attributes, we keep record of the cells of a grid  where we can measure  how  their values change over time. (Bridson, 2008)

Implementation explained :

* [first paper](http://graphics.cs.cmu.edu/nsp/course/15-464/Fall09/papers/StamFluidforGames.pdf)
* [second paper](https://mikeash.com/pyblog/fluid-simulation-for-dummies.html) 

### Particle based method

According to the Lagrangian viewpoint, we  define the fluid flow with particles where each particle carries its own properties, like mass, velocity, density etc. Conservation of mass and  Newton's laws apply directly to each fluid particle.  The **Lagrangian method** is easy to implement  as all the calculations are performed on the particles, but that makes them to rely heavily on the density which means in areas with low density, they could develop high percentage of inaccuracy  (Strantzi, 2016).

[Smoothed-particle hydrodynamics](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics)

[Simple implementation example](https://pmocz.github.io/manuscripts/pmocz_sph.pdf)

### Hybrid methods 

Also known as **Material point methods**

#### PIC method

The **particle-in-cell** (**PIC**) method refers to a technique used to solve a certain class of [partial differential equations](https://en.wikipedia.org/wiki/Partial_differential_equations "Partial differential equations"). In this method, individual particles (or fluid elements) in a [Lagrangian](https://en.wikipedia.org/wiki/Lagrangian_and_Eulerian_coordinates "Lagrangian and Eulerian coordinates") frame are tracked in continuous [phase space](https://en.wikipedia.org/wiki/Phase_space "Phase space"), whereas moments of the distribution such as densities and currents are computed simultaneously on Eulerian (stationary) [mesh](https://en.wikipedia.org/wiki/Mesh_(mathematics) "Mesh (mathematics)") points.

#### FLIP method

The **FLIP (Fluid  Implicit Particle  Method)** method is a simple modification of the PIC method. It was an attempt to solve the limitations of the PIC method, by Brackbill and Ruppel in 1986  (Brackbill and Ruppel,  1986).  To  avoid the interpolation during the particle-to-grid transfer, the FLIP method interpolates the change in the quantity (e.g. velocity) and  uses it to increment the particle value, instead of replacing it. Every increment undergoes one smoothing and the result is no numerical dissipation (Bridson,  
2008).

## References
* https://en.wikipedia.org/wiki/Fluid
* https://www.youtube.com/watch?v=Ra7aQlenTb8
* https://en.wikipedia.org/wiki/Navier%E2%80%93Stokes_equations
* https://nccastaff.bournemouth.ac.uk/jmacey/MastersProject/MSc17/03/thesis.pdf
* http://graphics.cs.cmu.edu/nsp/course/15-464/Fall09/papers/StamFluidforGames.pdf
* https://en.wikipedia.org/wiki/Particle-in-cell
* https://en.wikipedia.org/wiki/Material_point_method
