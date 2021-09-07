/*
Alec Trent
ajtrent
This program creates a 2D array representing random solutions to a numeric optimization problem. It implements an algorithm that uses randomness (a genetic algorithm) to search for the best solution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char** argv) {

  //Error handling for not enough inputs provided
  if (argc < 5){ 
    printf("Usage: GA <constant> <individuals> <genes> <max steps> <seed> [verbose] \n");
    exit(1);
  }

  //Initialize all variables
  int fitness;
  int constant = atoi(argv[1]); //equation we are trying to find a solution for
  int individual = atoi(argv[2]); //number of individuals in the population
  int genes = atoi(argv[3]); //number of genes each individual has 
  int maxSteps = atoi(argv[4]); //Maximum number of evolutionary steps
  int seed = atoi(argv[5]); //Integer for seeding the random number generator
  //int verbose = atoi(argv[6]); //output that shows the population at each iteration
  int population[individual][genes]; //Initializing the array

  //Error handling individuals/genes less than 2
  if (individual <= 1 || genes <= 1 ){ 
    printf("ERROR: number of individuals and genes must be greater than 1! \n");
    exit(1);
  }
  
  //Random number generator
  srand(seed);

  //Creates the matrix
  for (int i = 0; i < individual; i++) {
    for (int j = 0; j < genes; j++) {
      population[i][j] = (rand() % 10);
    }
  }

  if(argc > 6){ //Checks for verbose
  int p = 0;
  int z = 0;
    for(int generationCounter = 0; generationCounter <= maxSteps; generationCounter++){
      int min = INT_MAX;
      int max = INT_MIN;
      int average = 0;
      int product;
      int maxIndex = 0;
      int minIndex = 0;
    
      printf("*** Generation %d ***\n", generationCounter);
    
      for (int i = 0; i < individual; i++) { //Print formatting
       printf("%d: ", i);
        for(int k=0; k < genes; k++ ){
          printf("%d", population[i][k]);
        }
    
        int sum = 0;
        for (int j = 0; j < genes; j++) { //Finds sum for fitness
         product = population[i][j] * (j+1);
         sum += product;
       }
        fitness = abs(sum - constant); //Calculates fitness
        printf(", fitness=%d \n", fitness);
    
        if(fitness < min){ //Finds min 
          min = fitness;
          minIndex = i;
        }

        if(fitness > max){ //Finds max
          max = fitness;
          maxIndex = i;
        }
       average += fitness; //Finds average
      }
      printf("avg=%.4f, min=%d, max=%d\n", (double)average/(double)individual, min, max );

      if(generationCounter % 2 == 0){ //if loop denotes even or odd for generation switch
      
        if(min == 0){ //Handling for fitness equals zero and early stops
          printf("BEST %d: ", minIndex);  
          for(int k=0; k < genes; k++ ){ //Print formatting
            printf("%d", population[minIndex][k]);
          }
          printf(", fitness=%d, steps=%d\n", min, generationCounter);
          exit(1);
        }
        //Crossover
        int person1 = rand() % individual; //random person
        int person2 = rand() % individual; //random person
        while(person1 == person2){
          person2 = rand() % individual; //random person
        }
        int splicePoint = rand() % (genes - 1); //where we split on copy

        for(int i = 0; i <= splicePoint; i++){ //copies first part of person1 to new person
          population[maxIndex][i] = population[person1][i];
        }

        for(int i = splicePoint + 1; i < genes; i++){ // copies second part of person2 to new person
          population[maxIndex][i] = population[person2][i];
        }

        if(p < maxSteps){ //Handling for not printing on last run
          printf("Crossover, %d + %d -> %d, splice at %d, new: ", person1, person2, maxIndex, splicePoint);

          for(int i = 0; i < genes; i++){ //Print formatting
            printf("%d", population[maxIndex][i]);
          }
          printf("\n");
          p++;
        }

      }else{
        if(min == 0){ //Handling for fitness equals zero and early stops
          printf("BEST %d: ", minIndex);  
          for(int k=0; k < genes; k++ ){ //Print formatting
            printf("%d", population[minIndex][k]);
          }
          printf(", fitness=%d, steps=%d\n", min, generationCounter);
          exit(1);
        }

        //Mutation
        int person1 = rand() % individual; //random person
        int splicePoint1 = rand() % genes; //random gene to replace
        int randomGene = rand() % 10; //random gene to implement
        
        for(int i = 0; i < genes; i++){ //
          population[maxIndex][i] = population[person1][i]; //Replaces worst with random person
        }
        population[maxIndex][splicePoint1] = randomGene; //replaces random gene on copy of person
        
        if(p < maxSteps){ //Handling for not printing on last run
          printf("Mutation, %d -> %d, changed gene %d to value %d, new: ", person1, maxIndex, splicePoint1, randomGene);

          for(int i = 0; i < genes; i++){ //Print formatting
            printf("%d", population[maxIndex][i]);
          }
          printf("\n");
          p++;
        }
      }

      //int z;
      if(z == maxSteps){ //Formatting for final line on last run
        printf("BEST %d: ", minIndex);  
        for(int k=0; k < genes; k++ ){ //Print formatting
          printf("%d", population[minIndex][k]);
        }
        printf(", fitness=%d, steps=%d\n", min, maxSteps);
      }else{
        z++;
      }
    }

  }else{
    //Non verbose output
    int w = 0;
    for(int generationCounter = 0; generationCounter <= maxSteps; generationCounter++){

    int min = INT_MAX;
    int max = INT_MIN;
    int product;
    int minIndex = 0;
    int maxIndex = 0;
      for (int i = 0; i < individual; i++) { //Finds sum for fitness
        int sum = 0;
        for (int j = 0; j < genes; j++) {
          product = population[i][j] * (j+1);
          sum += product;
        }
        fitness = abs(sum - constant); //Finds fitness
      
        if(fitness < min){ //Finds min
          min = fitness;
          minIndex = i;
        }

        if(fitness > max){ //Finds max
          max = fitness;
          maxIndex = i;
        }
      }

      if(generationCounter % 2 == 0){ //if loop denotes even or odd for generation switch
      
        if(min == 0){ //Handling for fitness equals zero and early stops
          printf("BEST %d: ", minIndex);  
          for(int k=0; k < genes; k++ ){ //Print formatting
            printf("%d", population[minIndex][k]);
          }
          printf(", fitness=%d, steps=%d\n", min, generationCounter);
          exit(1);
        }
        //Crossover
        int person1 = rand() % individual; //random person
        int person2 = rand() % individual; //random person
        while(person1 == person2){
          person2 = rand() % individual; //random person
        }
        int splicePoint = rand() % (genes - 1); //where we split on copy

        for(int i = 0; i <= splicePoint; i++){ //copies first part of person1 to new person
          population[maxIndex][i] = population[person1][i];
        }

        for(int i = splicePoint + 1; i < genes; i++){ // copies second part of person2 to new person
          population[maxIndex][i] = population[person2][i];
        }

      }else{
        if(min == 0){ //Handling for fitness equals zero and early stops
          printf("BEST %d: ", minIndex);  
          for(int k=0; k < genes; k++ ){ //Print formatting
            printf("%d", population[minIndex][k]);
          }
          printf(", fitness=%d, steps=%d\n", min, generationCounter);
          exit(1);
        }

        //Mutation
        int person1 = rand() % individual; //random person
        int splicePoint1 = rand() % genes; //random gene to replace
        int randomGene = rand() % 10; //random gene to implement
        
        for(int i = 0; i < genes; i++){ //
          population[maxIndex][i] = population[person1][i]; //Replaces worst with random person
        }
        population[maxIndex][splicePoint1] = randomGene; //replaces random gene on copy of person
      }
      if(w == maxSteps){ //Formatting for final line on last run
        printf("BEST %d: ", minIndex);  
        for(int k=0; k < genes; k++ ){ //Print formatting
          printf("%d", population[minIndex][k]);
        }
        printf(", fitness=%d, steps=%d\n", min, maxSteps);
      }else{
        w++;
      }
    }
  }
}
