#include <stdlib.h>
#include <stdio.h>

#include "lib/test-lib.h"

int main(int argc, char *argv[]) {

    printf("Starting...\n");

    printf("Got MyVar from test-lib.h: %d\n", myVar);

    printf("Got %d arguments:\n", argc);

    for(int i = 0; i < argc; i++){
        printf("%d: %s",i, argv[i]);
        printf("\n");
    }
}