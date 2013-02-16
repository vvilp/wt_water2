#ifndef _WT_STABLE_FLUID_PRE_H_
#define _WT_STABLE_FLUID_PRE_H_

#include <iostream>
#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float * tmp=x0;x0=x;x=tmp;}

class Wt_StableFuild
{

public:
    int N;
    float dt, diff, visc;
    float force, source;
    int dvel;

    float *u, * v, * u_prev, * v_prev;
    float *dens, * dens_prev;

    float limit_vel;

public:

    void init()
    {
        N = 100;
        dt = 0.1f;
        diff = 0.0f;
        visc = 0.0f;
        force = 30.0f;
        source = 500.0f;
        limit_vel = 500.0f;
        int size = (N + 2) * (N + 2);

        u           = (float *) malloc ( size * sizeof(float) );
        v           = (float *) malloc ( size * sizeof(float) );
        u_prev      = (float *) malloc ( size * sizeof(float) );
        v_prev      = (float *) malloc ( size * sizeof(float) );
        dens        = (float *) malloc ( size * sizeof(float) );
        dens_prev   = (float *) malloc ( size * sizeof(float) );

        for ( int i = 0 ; i < size ; i++ )
        {
            u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = 0.0f;

            //v[i] = 1.0f;
            //u[i] = 0.001f;
        }
    }


    void add_source ( int N, float *s_prev, float *s, float dt )
    {
        for (int i = 0 ; i < (N + 2) * (N + 2) ; i++ )
        {
            s_prev[i] += dt * s[i];
        }
    }

    void set_bnd ( int N, int b, float *x )
    {
        int i;

        for ( i = 1 ; i <= N ; i++ )
        {
            x[IX(0    , i    )] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
            x[IX(N + 1, i    )] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
            x[IX(i    , 0    )] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
            x[IX(i    , N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
        }
        x[IX(0     , 0    )] = 0.5f * (x[IX(1, 0    )] + x[IX(0    , 1)]);
        x[IX(0     , N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0    , N)]);
        x[IX(N + 1 , 0    )] = 0.5f * (x[IX(N, 0    )] + x[IX(N + 1, 1)]);
        x[IX(N + 1 , N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
    }

    void lin_solve ( int N, int b, float *x, float *x0, float a, float c )
    {
        int i, j, k;

        //for ( k = 0 ; k < 20 ; k++ )
        {
            for (int i = 1; i < N; ++i)
            {
                for (int j = 1; j < N; ++j)
                {
                    x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] + x[IX(i, j - 1)] + x[IX(i, j + 1)])) / c;
                }
            }

            set_bnd ( N, b, x );
        }
    }

    void diffuse ( int N, int b, float *x, float *x_prev, float diff, float dt )
    {
        float a = dt * diff * N * N;
        //lin_solve ( N, b, x, x0, a, 1 + 4 * a );
        for (int k = 0 ; k < 20 ; k++ )
        {
            for (int i = 1; i < N; ++i)
            {
                for (int j = 1; j < N; ++j)
                {
                    x[IX(i, j)] = (x_prev[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] + x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
                }
            }

            set_bnd ( N, b, x );
        }
    }

    void advect ( int N, int b, float *d, float *d0, float *u, float *v, float dt )
    {
        int i, j, i0, j0, i1, j1;
        double x, y, s0, t0, s1, t1, dt0;
        dt0 = dt * N;
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                x = i - dt0 * u[IX(i, j)];
                y = j - dt0 * v[IX(i, j)];
                if (x < 0.5)
                    x = 0.5;
                if (x > N + 0.5)
                    x = N + 0.5;
                i0 = (int) x;
                i1 = i0 + 1;
                if (y < 0.5)
                    y = 0.5;
                if (y > N + 0.5)
                    y = N + 0.5;
                j0 = (int) y;
                j1 = j0 + 1;
                s1 = x - i0;
                s0 = 1 - s1;
                t1 = y - j0;
                t0 = 1 - t1;
                d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)])
                        + s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
            }
        }
        set_bnd(N, b, d);
    }

    void project ( int N, float *u, float *v, float *p, float *div )
    {
        int i, j;

        for (int i = 1; i < N; ++i)
        {
            for (int j = 1; j < N; ++j)
            {
                div[IX(i, j)] = -0.5f * (u[IX(i + 1, j)] - u[IX(i - 1, j)] + v[IX(i, j + 1)] - v[IX(i, j - 1)]) / N;
                p[IX(i, j)] = 0;
            }
        }
        set_bnd ( N, 0, div ); 
        set_bnd ( N, 0, p );

        //lin_solve ( N, 0, p, div, 1, 4 );
        for (int k = 0 ; k < 20 ; k++ )
        {
            for (int i = 1; i < N; ++i)
            {
                for (int j = 1; j < N; ++j)
                {
                    p[IX(i, j)] = (div[IX(i, j)] + (p[IX(i - 1, j)] + p[IX(i + 1, j)] + p[IX(i, j - 1)] + p[IX(i, j + 1)])) / 4;
                }
            }

            set_bnd ( N, 0, p );
        }

        for (int i = 1; i < N; ++i)
        {
            for (int j = 1; j < N; ++j)
            {
                u[IX(i, j)] -= 0.5f * N * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
                v[IX(i, j)] -= 0.5f * N * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
            }
        }
        set_bnd ( N, 1, u ); set_bnd ( N, 2, v );
    }

    void dens_step ( int N, float *x, float *x0, float *u, float *v, float diff, float dt )
    {
        float * tmp;
        add_source ( N, x, x0, dt );
        //tmp = x0 ; x0 = x ; x = tmp;
        SWAP(x0,x);
        diffuse ( N, 0, x, x0, diff, dt );
        //tmp = x0 ; x0 = x ; x = tmp;
        SWAP(x0,x);
        advect ( N, 0, x, x0, u, v, dt );
    }

    void vel_step ( int N, float *u, float *v, float *u0, float *v0, float visc, float dt )
    {
        add_source ( N, u, u0, dt ); 
        add_source ( N, v, v0, dt );

        SWAP ( u0, u );
        diffuse ( N, 1, u, u0, visc, dt );
        SWAP ( v0, v ); 
        diffuse ( N, 2, v, v0, visc, dt );

        project ( N, u, v, u0, v0 );

        SWAP ( u0, u ); 
        SWAP ( v0, v );
        advect ( N, 1, u, u0, u0, v0, dt ); 
        advect ( N, 2, v, v0, u0, v0, dt );

        project ( N, u, v, u0, v0 );
    }

    void fluid_step()
    {
        vel_step ( N, u, v, u_prev, v_prev, visc, dt );
        dens_step ( N, dens, dens_prev, u, v, diff, dt );

        int i, j, size = (N + 2) * (N + 2);

        for ( i = 0 ; i < size ; i++ )
        {
            u_prev[i] = v_prev[i] = dens_prev[i] = 0.0f;
        }
    }

    void set_u(int i, int j, float vel)
    {
        //printf("vel : %f\n",vel);
        u_prev[IX(i, j)] = vel < limit_vel ? vel : limit_vel;
        
    }

    float get_u(int i,int j)
    {
        //std::cout<<"vel : " << u[IX(i, j)] <<std::endl;
        return u[IX(i, j)];

    }

    void set_v(int i, int j, float vel)
    {
        v_prev[IX(i, j)] = vel < limit_vel ? vel : limit_vel;
    }

    float get_v(int i,int j)
    {
        return v[IX(i, j)];
    }

    void set_den(int i, int j, float den)
    {
        dens_prev[IX(i, j)] = den;
    }

};


#endif