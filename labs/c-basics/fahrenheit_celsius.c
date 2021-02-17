/* Ari Valenzuela A01635584*/

#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char** argv){
    int fahr;
    /*Case when only one argument*/
    if(argc==2){
        fahr= atoi(argv[1]);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
    /*Case when you give 3 arguments, (Lower bound, upper bound and the step)*/ 
    else if (argc==4){
        for (fahr = atoi(argv[1]); fahr <= atoi(argv[2]); fahr = fahr + atoi(argv[3]))
	    printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    } else{
        printf("ERROR, Give 1 argument with the fahrenheit temperature you want to convert or 3 arguments with the initial fahrenheit temperature, the upper bound and the step\n");
    }

    return 0;
}
