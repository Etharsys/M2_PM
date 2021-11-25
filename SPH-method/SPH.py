import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

import random,math
class Particle :
    def __init__(self,position) :
        self.position = position
        self.velocity = np.array([0,0],dtype=np.float64)
        self.force = np.array([0,0],dtype=np.float64)
        self.density = 0
        self.pressure = 0

class Fluid :

    def __init__(self,width,height) :
        self.particles = []
        self.G = np.array([0,-10],dtype=np.float64)
        self.H = 16
        self.HSQ = self.H  * self.H 
        self.MASS = 2.5
        self.REST_DENS = 300
        self.GAS_CONST = 2000
        self.VISC = 200
        self.POLY6 =  4 / (math.pi * pow(self.H , 8))
        self.SPIKY_GRAD = -10 / (math.pi * pow(self.H , 5))
        self.VISC_LAP =  40 / (math.pi * pow(self.H , 5))
        self.DT = 0.0007
        self.WIDTH = width
        self.HEIGHT = height
        self.BOUND_DAMPING = -0.5
        self.fig = plt.figure()
        self.ax = plt.axes(xlim=(0, width), ylim=(0, height))
        
        for x in range(width//2,width//2+100,self.H ) :
            for y in range(height//2,height//2+100,self.H ) :
                self.particles.append(Particle(np.array([x + random.random(),y],dtype=np.float64)))

    def computeDensityPressure(self) :
        for p1 in self.particles :
            p1.density = 0.0
            for p2 in self.particles :
                v12 = p2.position - p1.position
                r2 = np.inner(v12,v12)
                if(r2 < self.HSQ) :
                    p1.density += self.MASS * self.POLY6 * pow(self.HSQ - r2,3)
            p1.pressure = self.GAS_CONST * (p1.density - self.REST_DENS)

    def __normalize(self,v):
        norm = np.linalg.norm(v)
        if norm == 0: 
            return v
        return v / norm

    def computeForces(self) :
        for p1 in self.particles :
            fpress = np.zeros(2)
            fvisc = np.zeros(2)
            for p2 in self.particles :
                if(p1 == p2) :
                    continue
                v12 = p2.position - p1.position
                r = math.sqrt(np.inner(v12,v12))
                v12 = self.__normalize(v12)
                if(r < self.H) :
                    fpress += -v12 * self.MASS * (p1.pressure + p2.pressure) / (2 * p2.density) * self.SPIKY_GRAD * pow(self.H-r,3)
                    fvisc += self.VISC * self.MASS * (p2.velocity - p1.velocity) / p2.density * self.VISC_LAP * (self.H-r)
            fgrav = self.G *self.MASS / p1.density
            p1.force = fpress + fvisc + fgrav
    
    def integrate(self):
        for particule in self.particles :
            particule.velocity += self.DT * particule.force / particule.density
            particule.position += self.DT * particule.velocity
            if(particule.position[0] - self.H < 0) :
                particule.velocity[0] *= self.BOUND_DAMPING
                particule.position[0] = self.H
            if(particule.position[0] + self.H > self.WIDTH - 1) :
                particule.velocity[0] *= self.BOUND_DAMPING
                particule.position[0] = self.WIDTH - self.H
            if(particule.position[1]- self.H < 0) :
                particule.velocity[1] *= self.BOUND_DAMPING
                particule.position[1] = self.H
            if(particule.position[1] + self.H > self.HEIGHT - 1) :
                particule.velocity[1] *= self.BOUND_DAMPING 
                particule.position[1] = self.HEIGHT - self.H
        

    def update(self,i) :
        self.computeDensityPressure()
        self.computeForces()
        self.integrate()
        self.ax.clear()
        self.ax.set_xlim(0,self.WIDTH)
        self.ax.set_ylim(0,self.HEIGHT)
        npParticles = np.array(list(map(lambda x : x.position,self.particles)))
        self.ax.scatter(npParticles[:,0],npParticles[:,1],s=self.H//2,c="darkred")
        
    def show(self) :
        anim = animation.FuncAnimation(self.fig, self.update,
                               frames=100, interval=1)
        plt.show()

fluid = Fluid(1000,1000)
fluid.show()