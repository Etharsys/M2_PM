#define IX(i, j) ((i)+(N+2)*(j))
#define SWAP(x0, x) {float * tmp=x0;x0=x;x=tmp;}
#define FOR_EACH_CELL for ( i=1 ; i<=N ; i++ ) { for ( j=1 ; j<=N ; j++ ) {
#define END_FOR }}

void add_source(int N, float *x, const float *s, float dt)
{
    int i, size = (N + 2) * (N + 2);
    for (i = 0; i < size; i++)
    {
        x[i] += dt * s[i];
    }
}

void set_bnd(int N, int b, float *x)
{
    int i;

    for (i = 1; i <= N; i++)
    {
        /* periodic bc */
        x[IX(0  ,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
        x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
        x[IX(i,0  )] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
        x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
        /* lid driven cavity bc */
        // left
//        x[IX(0, i)] = 0.f;
//        // right
//        x[IX(N + 1, i)] = 0.f;
//        // bottom
//        x[IX(i, 0)] = 0.f;
//        // top / lid
//        x[IX(i, N + 1)] = 0;
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
    x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
    x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void lin_solve(int N, int b, float *x, const float *x0, float a, float c)
{
    int i, j, k;

    for (k = 0; k < 20; k++)
    {
        FOR_EACH_CELL
                x[IX(i, j)] =
                        (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] + x[IX(i, j - 1)] + x[IX(i, j + 1)]))
                        / c;
        END_FOR
        set_bnd(N, b, x);
    }
}

void diffuse(int N, int b, float *x, float *x0, float diff, float dt)
{
    auto Nf = static_cast<float>(N);
    float a = dt * diff * Nf * Nf;
    lin_solve(N, b, x, x0, a, 1 + 4 * a);
}

/*
*
*	->Advect step:
*
*		*d : pointer to advected scalar
*		*u : pointer to advective velocity field (x-component)
*		*v : pointer to advective velocity field (y-component)
*		 b = 0 : density advection
*		   = 1 : u-component advection
*		   = 2 : v-component advection
*
*/
void advect(int N, int b, float *d, const float *d0, const float *u, const float *v, float dt)
{
    auto Nf = static_cast<float>(N);
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;

    dt0 = dt * Nf;
    FOR_EACH_CELL
            // fluid particle back tracing
            x = static_cast<float>(i) - dt0 * u[IX(i, j)];
            y = static_cast<float>(j) - dt0 * v[IX(i, j)];
            // find nodes adjacent to former position
            if (x < 0.5f)
            {
                x = 0.5f;
            }
            if (x > Nf + 0.5f)
            {
                x = Nf + 0.5f;
            }
            i0 = (int) x;
            i1 = i0 + 1;
            if (y < 0.5f)
            {
                y = 0.5f;
            }
            if (y > Nf + 0.5f)
            {
                y = Nf + 0.5f;
            }
            j0 = (int) y;
            j1 = j0 + 1;
            // bi-linear interpolation from adjacent nodes to former position x
            s1 = x - static_cast<float>(i0);
            s0 = 1 - s1;
            t1 = y - static_cast<float>(j0);
            t0 = 1 - t1;
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                          s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
    END_FOR
    set_bnd(N, b, d);
}

void project(int N, float *u, float *v, float *p, float *div)
{
    auto Nf = static_cast<float>(N);
    int i, j;

    FOR_EACH_CELL
            div[IX(i, j)] = -0.5f * (u[IX(i + 1, j)] - u[IX(i - 1, j)] + v[IX(i, j + 1)] - v[IX(i, j - 1)]) / Nf;
            p[IX(i, j)] = 0;
    END_FOR
    set_bnd(N, 0, div);
    set_bnd(N, 0, p);

    lin_solve(N, 0, p, div, 1, 4);

    FOR_EACH_CELL
            u[IX(i, j)] -= 0.5f * Nf * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
            v[IX(i, j)] -= 0.5f * Nf * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
    END_FOR
    set_bnd(N, 1, u);
    set_bnd(N, 2, v);
}

void dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt)
{
    add_source(N, x, x0, dt);
    SWAP (x0, x)
    diffuse(N, 0, x, x0, diff, dt);
    SWAP (x0, x)
    advect(N, 0, x, x0, u, v, dt);
}

void vel_step(int N, float *u, float *v, float *u0, float *v0, float viscosity, float dt)
{
    add_source(N, u, u0, dt);
    add_source(N, v, v0, dt);
    SWAP (u0, u)
    diffuse(N, 1, u, u0, viscosity, dt);
    SWAP (v0, v)
    diffuse(N, 2, v, v0, viscosity, dt);
    project(N, u, v, u0, v0);
    SWAP (u0, u)
    SWAP (v0, v)
    advect(N, 1, u, u0, u0, v0, dt);
    advect(N, 2, v, v0, u0, v0, dt);
    project(N, u, v, u0, v0);
}

