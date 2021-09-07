//Alec Trent
//ajtrent
//Reads in details about a list of cities from standard input.
// Prints to standard output the name of each city including its state abbreviation and the calculated strength of the city.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv) {
    /*printf("hi\n");
    //If info is entered, stored here
    char* cityName = argv[1];
    printf("hi1\n");
    char* stateAbbreviation = argv[2];
    printf("hi2\n");
    int population = argv[5];
    printf("pops = %d\n", population);
    int incorporated = atoi(argv[6]);*/

    //Initialize the variables
    char cityName[255];
    char stateAbbreviation[255];
    double t;
    double r;
    int population;
    int incorporated;

    while(scanf("%s %s %lf %lf %d %d", cityName, stateAbbreviation, &t, &r, &population, &incorporated) !=EOF) {
        double cityStrength = log(population); //calculates city strength
        if (incorporated == 0) { //penalty applied for not being incorporated
            cityStrength = cityStrength * 0.75;
        }
        printf("%s_%s %.0f\n", cityName, stateAbbreviation, round(cityStrength));
    }
}
