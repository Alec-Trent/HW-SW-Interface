//Alec Trent
//ajtrent
// Reads in the file output by GenerateMap using file I/O. Prints to standard output a visualization of the map.
// Lets the player enter coordinates of cities via standard input and displays the results of each battle.
// The game continues until only one city remains or the player decides to quit.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Represents a city somewhere on the map, DO NOT MODIFY
typedef struct
{
    char* name;     // Null-terminated string name of the city
    int strength;   // Current strength of the city
} City;

// Represents the location of something on the map, DO NOT MODIFY
typedef struct
{
    int row;        // Row location on the map
    int col;        // Column location on the map
} Location;

//This function is responsible for reading information about cities from the passed in filename.
// The file is in the format output by GenerateMap.
// Since the person running CityBattle may specify a different number of rows and columns compared to the one
// that generated a data file, this function is responsible for checking the row and column of a city is valid
// prior to adding it to the two-dimensional array.
// TODO: readMap function
int readMap(const char*filename, int rows, int cols, City* cities[rows][cols]){
    FILE* fpIn = fopen(filename, "r");
    if (fpIn == NULL){
        printf("Failed to open input file: %s\n", filename);
        return 1;
    }

    char cityName[255];
    int readRow;
    int readCol;
    int strength;
    int cityCounter = 0;

    //Read in file/ ADD city to map if valid / IF NOT print skip message
    while(fscanf(fpIn, "%s %d %d %d" , cityName, &readRow, &readCol, &strength) != EOF){
        //The program skips adding any subsequent cities that map to the same coordinate.
        if(readRow > rows || readCol > cols){
            printf("Skipped '%s', row %d column %d invalid\n", cityName, readRow, readCol);
        }else{
            City *city = malloc(sizeof(City));
            city->name = malloc(sizeof(char)*strlen(cityName)+1);
            cities[readRow][readCol] = city;
            strcpy(city->name, cityName);
            city->strength = strength;
            cityCounter++;
        }
    }
    fclose(fpIn);
    fpIn = NULL;

    //RETURNS an int of # of cities that were added to the game
    return cityCounter;
}

//This function prints an ASCII visualization of the game map.
// Each grid coordinate is represented by a single character.
// The map shows the first letter of a city's name at its position on the map.
// The map is surrounded by a border made up of pluses, dashes, and vertical bars.
// To the right of each map row, the full name of each city appears (in order from left to right).
// The city names are separated by a space.
// TODO: printMap function
void printMap(int rows, int columns, City* cities[rows][columns]){
    //Prints top border
    printf("+");
    for(int i = 0; i < columns; i++){
        printf("-");
    }
    printf("+\n");

    //Prints side borders and array
    char* cityLetter;
    for(int j = 0; j < rows; j++) {
        printf("|");
        for (int i = 0; i < columns; i++) {
            City* city = cities[j][i];
            if(city == NULL){
                printf(" ");
            }else{
                cityLetter = city->name;
                printf("%c", cityLetter[0]);
            }
        }
        printf("| ");

        //Print names after this
        char* cityName;
        for(int i = 0; i<columns; i++) {
            City *city = cities[j][i];
            if(city != NULL){
                cityName = city->name;
                printf("%s ", cityName);
            }
        }
        printf("\n");
    }

    //Prints bottom border
    printf("+");
    for(int i = 0; i<columns; i++){
        printf("%s", "-");
    }
    printf("+\n");
}

//This function takes the location of two cities read in from the player.
// The function should first check if the coordinate for the first city is valid (i.e. it has a row and column that are in the range of the map and there is in fact a city at that coordinate).
// If the first coordinate is invalid, the function prints an error message and returns.
// The function similarly should check the second coordinate.
//If both coordinates are valid, the function further checks that they are not the identical location (cities can't fight themselves).
// The function should return true if there was actually a battle between two cities, or false if something was wrong with the locations.
// TODO: battleCities function
bool battleCities(int rows, int cols, City* cities[rows][cols], Location location1, Location location2){
    City* city1 = cities[location1.row][location1.col];
    City* city2 = cities[location2.row][location2.col];

    if(location1.row > rows || location1.col > cols || city1 == NULL){
        printf("ERROR: first location invalid!\n");
        return false;
    }

    if(location2.row > rows || location2.col > cols || city2 == NULL){
        printf("ERROR: second location invalid!\n");
        return false;
    }

    if(location1.row == location2.row && location1.col == location2.col){
        printf("ERROR: a city can't fight itself!\n");
        return false;
    }

    //City1 less than
    if(city1->strength < city2->strength){
        printf("%s (%d) versus %s (%d), %s wins\n", city1->name, city1->strength, city2->name, city2->strength, city2->name);
        city2->strength = city2->strength + city1->strength;
        free(cities[location1.row][location1.col]);
        cities[location1.row][location1.col] = NULL;
        return true;
    }

    //City1 greater than
    if(city1->strength > city2->strength){
        printf("%s (%d) versus %s (%d), %s wins\n", city1->name, city1->strength, city2->name, city2->strength, city1->name);
        city1->strength = city1->strength + city2->strength;
        free(cities[location2.row][location2.col]);
        cities[location2.row][location2.col] = NULL;
        return true;
    }

    //City1 & City2 Equal
    if(city1->strength == city2->strength){
        printf("%s (%d) versus %s (%d), %s wins\n", city1->name, city1->strength, city2->name, city2->strength, city1->name);
        city1->strength = city1->strength + city2->strength;
        free(cities[location2.row][location2.col]);
        cities[location2.row][location2.col] = NULL;
        return true;
    }

    return false;
}

//The job of this function is to make sure any memory allocated by readMap is deallocated at the end of the game.
// Note that the player of the game may stop the game early by hitting ctrl-d (Mac/Unix) or ctrl-z (Windows) at one of the location prompts.
// In such cases, there could be multiple cities remaining when the main game loop exits.
// TODO: uninitMap function
void uninitMap(int rows, int cols, City* cities[rows][cols]){
    //City* city = cities[rows][cols];
    //free(city->name);
    //city->name = NULL;

    for(int i = 0; i < rows; i++){ //for loops iterate over array
        for(int j = 0; j < cols; j++){
            if(cities[i][j] != NULL) {
                City* city = cities[i][j];
                free(city->name);
                city->name = NULL;
                free(city);
                city = NULL;
            }
        }
    }
}


// NOTE: you should only add code to the TODO section of the main function
// DO NOT MODIFY other parts of the main function
int main(int argc, char** argv)
{
    if (argc < 4)
    {
        printf("Usage: CityBattle <rows> <columns> <map filename>\n");
        return 0;
    }

    // TODO: Parse and validate the comand line arguments for the number of rows and columns
    //save arguments to vars
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    //validate
    if (rows < 1 || rows > 100 || cols < 1 || cols > 100) {
        printf("ERROR: rows and columns must be between 1-100!\n");
        return 1;
    }

    // Create and initialize a 2D array that contains pointers to City structs
    City* cities[rows][cols];
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            cities[row][col] = NULL;
        }
    }

    // Read in the cities from standard input into elements of the 2D array
    int numCities = readMap(argv[3], rows, cols, cities);
    if (numCities <= 0)
    {
        printf("ERROR: failed to read in any cities!\n");
        return 0;
    }
    printf("Read in %d cities\n", numCities);

    // Read coordinates for cities until user hits ctrl-d (mac/unix) or ctrl-z (windows), or only one city left
    bool loop = true;
    while ((loop) && (numCities > 1))
    {
        loop = false;
        printMap(rows, cols, cities);

        Location location1 = {0, 0};
        printf("City 1? ");
        if (scanf("%d %d", &location1.row, &location1.col) == 2)
        {
            Location location2 = {0, 0};
            printf("City 2? ");
            if (scanf("%d %d", &location2.row, &location2.col) == 2)
            {
                if (battleCities(rows, cols, cities, location1, location2))
                {
                    // Only decrement if battle was valid
                    numCities--;
                }
                loop = true;
            }
        }
    }
    if (numCities == 1)
    {
        printf("We have a winner!\n");
    }
    else
    {
        printf("Player quit\n");
    }

    // Deallocate memory allocated in readMap
    uninitMap(rows, cols, cities);

    return 0;
}
