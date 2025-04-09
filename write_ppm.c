#include "write_ppm.h"
#include <stdlib.h>
#include <stdio.h>

int write_ppm_to_file(int* roots_grid,int degree,int resolution){
    char filename[40]; //suffices
    sprintf(filename,"Basin_d%d_l%d.ppm",degree,resolution);
    FILE* file = fopen(filename,"w");
    fprintf(file,"P3\n");
    fprintf(file,"%d %d\n",resolution,resolution);
    fprintf(file,"255\n");
    for(int i = 0;i<resolution*resolution;i++){
        if(roots_grid[i] == -1){
            fprintf(file,"236 192 218\n");    
        }
        else if(roots_grid[i] == 0){
            fprintf(file,"233 142 192\n");
        }
        else if(roots_grid[i] == 1){
            fprintf(file,"164 160 230\n");
        }
        else if(roots_grid[i] == 2){
            fprintf(file,"142 231 231\n");
        }
        else if(roots_grid[i] == 3){
            fprintf(file,"255 228 97\n");
        }
        else if(roots_grid[i] == 4){
            fprintf(file,"255 148 148\n");
        }
        else if(roots_grid[i] == 5){
            fprintf(file,"111 85 129\n");
        }
        else if(roots_grid[i] == 6){
            fprintf(file,"166 207 79\n");
        }
        else if(roots_grid[i] == 7){
            fprintf(file,"238 177 116\n");
        }
        else{
            fprintf(file,"36 75 118\n");
        }
    }
    fclose(file);
    return 0;
}

int write_iterations_to_ppm(int* iterations_grid,int degree,int resolution){
    char filename[40]; //suffices
    sprintf(filename,"Iterations_d%d_l%d.ppm",degree,resolution);
    FILE* file = fopen(filename,"w");
    fprintf(file,"P3\n");
    fprintf(file,"%d %d\n",resolution,resolution);
    fprintf(file,"255\n");
    for(int i = 0;i<resolution*resolution;i++){
        fprintf(file,"%d %d %d\n",2*iterations_grid[i],2*iterations_grid[i],2*iterations_grid[i]);
    }
    fclose(file);
    return 0;
}