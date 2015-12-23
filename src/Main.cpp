#include <stdlib.h>
#include <stdio.h>

#include "gfx/Renderer.h"

void usage(char* program)
{
    fprintf(stderr, "USAGE\n");
    fprintf(stderr, "%s: sample_filename\n", program);
}

int main(int argc, char** argv)
{
    if(argc != 2){
        //usage(argv[0]);
        //exit(EXIT_FAILURE);
    }
    GFXInit(argc, argv);

    return EXIT_SUCCESS;
}
