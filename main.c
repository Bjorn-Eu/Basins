#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "newton.h"


int main(int argc,char* argv[]){
    int opt;    
    int res = 5000; //default resolution
    int n_threads = 1; //default number of threads
    int degree;
    while((opt = getopt(argc,argv,"t:l:")) != -1){
        switch(opt){
            case 't':
                n_threads = atoi(optarg);
                break;
            case 'l':
                res = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-t threads] [-l resolution] degree\n",argv[0]);
                return EXIT_FAILURE;

        }
    }

    if(argc == (optind+1)){
        degree = atoi(argv[optind]);
        if(degree < 1 || degree > 9){
            fprintf(stderr,"The degree is required to be between 1 and 9\n");
            return EXIT_FAILURE;
        }
    } else{
        fprintf(stderr, "Usage: %s [-t threads] [-l resolution] degree\n",argv[0]);
        return EXIT_FAILURE;
    }

    printf("Running program with values are t:%d l:%d d:%d\n",n_threads,res,degree);

    calculate_basins(degree,res,n_threads);
    return EXIT_SUCCESS;
}