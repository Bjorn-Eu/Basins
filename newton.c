#include "newton.h"
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <threads.h>
#include "write_ppm.h"

#define _XY_COORD_RANGE 2
#define _MAX_ITERATIONS 128
#define _CONVERGENCE_LIMIT 0.001
#define _CONVERGENCE_UB 10000000000 

typedef struct{
    int* roots_grid;
    int* iterations_grid;
    int sid;
    int eid;
    double complex* roots;
}output_grid_t;


const double convergence_limit_2 = 0.00001; //square of the norm
const double convergence_ub_2 = 100000000000000000000.0; //square of the norm 10**20
int degree;
int resolution;
int n_threads;

//integers powers for n=1,...,9 , faster in practice than e.g. a loop.
double complex pow_z(double complex z,int n){
    if(n==0){
        return 1.0;
    }
    if(n==1){
        return z;
    }
    else if(n==2){
        return z*z;
    }
    else if(n==3){
        return z*z*z;
    }
    else if(n==4){
        return z*z*z*z;
    }
    else if(n==5){
        return z*z*z*z*z;
    }
    else if(n==6){
        return z*z*z*z*z*z;
    }
    else if(n==7){
        return z*z*z*z*z*z*z;
    }
    else if(n==8){
        return z*z*z*z*z*z*z*z;
    }
    else if(n==9){
        return z*z*z*z*z*z*z*z*z;
    }
    else{
        exit(EXIT_FAILURE);
    }
}

double complex func(double complex z){
    double complex fz;
    fz = pow_z(z,degree)-1;
    return fz;
}

double complex next_iteration(double complex z){
    double complex newz;
    double complex divz = degree*pow_z(z,degree-1);
    newz = (degree-1)*z/degree+1/divz;
    return newz;
}

double complex find_root(double complex z0,int* iterations_used){
    double complex z = z0;
    for(int i=0;i<_MAX_ITERATIONS;i++){
        z = next_iteration(z);
        //calculate cabs^2
        double fz_2 = creal(func(z))*creal(func(z))+cimag(func(z))*cimag(func(z));
        double absz_2 = creal(z)*creal(z)+cimag(z)*cimag(z);
        if(fz_2<convergence_limit_2){
            break;
        }
        if(absz_2<convergence_limit_2|| absz_2>convergence_ub_2)
            break;
        (*iterations_used) = i;
    }
    
    return z;
}


int is_root(double complex z,double complex* roots){
    for(int i = 0;i<degree;i++){
        double complex diff = z-roots[i];
        double diff_2 = creal(diff)*creal(diff)+cimag(diff)*cimag(diff);
        if(diff_2<convergence_limit_2){
            return i;
        }
    }
    return -1;
}

int process_grid(void* args){
    output_grid_t* output_grid = (output_grid_t*)args;
    int* roots_grid = output_grid->roots_grid;
    int* iterations_grid = output_grid->iterations_grid;
    int sid = output_grid->sid;
    int eid = output_grid->eid;
    double complex* roots = output_grid->roots;
    double step_size = (double)(2*_XY_COORD_RANGE)/resolution;
    for(int i = sid;i<eid;i++){
        for(int j = 0;j<resolution;j++){
            double complex z0 = (step_size/2-2+i*step_size)+I*(step_size/2-2+j*step_size);
            int iterations_used;
            double complex z = find_root(z0,&iterations_used);
            roots_grid[resolution*i+j] = is_root(z,roots);
            iterations_grid[resolution*i+j] = iterations_used;
        }
    }
    return 0;
}

    

//degree, resolution, n_threads
void calculate_basins(int d, int l,int t){
    degree = d;
    resolution = l;
    n_threads = t;
    //initiate roots
    double complex* roots = (double complex*)malloc(degree*sizeof(double complex));
    printf("The roots are:\n");
    for(int i = 0;i<degree;i++){
        roots[i] = cos(2*i*M_PI/degree)+I*sin(2*i*M_PI/degree);
        printf("%.03f %+.03fi\n",creal(roots[i]),cimag(roots[i]));
    }




    int* roots_grid = (int*)malloc(resolution*resolution*sizeof(int));
    int* iterations_grid = (int*) malloc(resolution*resolution*sizeof(int));

    thrd_t* threads = (thrd_t*)malloc(n_threads*sizeof(thrd_t));
    output_grid_t* output_grid = (output_grid_t*)malloc(n_threads*sizeof(output_grid_t));
    int step_size = resolution/n_threads;
    for(int i = 0;i<n_threads;i++){
        output_grid[i].roots_grid = roots_grid;
        output_grid[i].iterations_grid = iterations_grid;
        output_grid[i].sid = step_size*i;
        output_grid[i].eid = step_size*(i+1);
        output_grid[i].roots = roots;

        int r = thrd_create(threads+i, process_grid, (void*)(output_grid+i));
        if ( r != thrd_success ) {
          fprintf(stderr, "failed to create thread\n");
          exit(EXIT_FAILURE);
        }
    }

    




    //join the threads
    int r;
    for(int i = 0;i<n_threads;i++){
        thrd_join(threads[i],&r);
    }


    //write to file
    write_ppm_to_file(roots_grid,degree,resolution);
    write_iterations_to_ppm(iterations_grid,degree,resolution);

    free (threads);
    free(output_grid);
    free(roots_grid);
    free(iterations_grid);
    free(roots);
}