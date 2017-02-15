#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 2) {
        int i, num, j=1;
        //sscanf (char, "%format", variable_to_store_converted_format)
        //sscanf (argv[1],"%d", &num);
        num = atoi(argv[1]); //note that atoi() requires stdlib.h

        printf ("Number entered: %d\n", num);
        for (i=1; i<=num; i++)
            j=j*i;

        printf("The factorial of %d is %d\n",num,j);
    }
    else if (argc>2) {
        printf("Too many arguments supplied.\nUsage: %s num\n", argv[0]);
    }
    else {
        printf("One argument expected.\nUsage: %s num\n", argv[0]);
    }
    return 0;
}
