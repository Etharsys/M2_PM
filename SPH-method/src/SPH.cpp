#include <GL/glut.h>
#include <Fluid.hpp>

/*
References : 
https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
https://lucasschuermann.com/writing/implementing-sph-in-2d
https://matthias-research.github.io/pages/publications/sca03.pdf
*/

Fluid fluid{100,500,100,500};

void Update(void)
{
	fluid.update();
	glutPostRedisplay();
}

void InitGL(void)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(fluid.H / 2.f);
	glMatrixMode(GL_PROJECTION);
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0,fluid.WIDTH, 0, fluid.HEIGHT, 0, 1);

	glColor4f(0.2f, 0.6f, 1.f, 1);
	
	glBegin(GL_POINTS);
	for (auto &p : fluid.particles.get_all_elements())
	{
		
		glVertex2f(p.position(0), p.position(1));
		
	}
	glEnd();
	glutSwapBuffers();
}

void Keyboard(unsigned char c, __attribute__((unused)) int x, __attribute__((unused)) int y)
{
	/*
	switch (c)
	{
	case ' ':
		if (particles.size() >= MAX_PARTICLES)
		{
			std::cout << "maximum number of particles reached" << std::endl;
		}
		else
		{
			unsigned int placed = 0;
			for (float y = VIEW_HEIGHT / 1.5f - VIEW_HEIGHT / 5.f; y < VIEW_HEIGHT / 1.5f + VIEW_HEIGHT / 5.f; y += H * 0.95f)
			{
				for (float x = VIEW_WIDTH / 2.f - VIEW_HEIGHT / 5.f; x <= VIEW_WIDTH / 2.f + VIEW_HEIGHT / 5.f; x += H * 0.95f)
				{
					if (placed++ < BLOCK_PARTICLES && particles.size() < MAX_PARTICLES)
					{
						particles.push_back(Particle(x, y));
					}
				}
			}
		}
		break;
	case 'r':
	case 'R':
		particles.clear();
		InitSPH();
		break;
	}
	*/
}

int main(int argc, char **argv)
{
	glutInitWindowSize(fluid.WIDTH, fluid.HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInit(&argc, argv);
	glutCreateWindow("Müller SPH");
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutKeyboardFunc(Keyboard);

	InitGL();

	glutMainLoop();
	return 0;
}