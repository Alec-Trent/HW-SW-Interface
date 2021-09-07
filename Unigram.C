//Alec Trent
//ajtrent
//Reads its language model training data from standard input. All words in the training file are converted to uppercase and stripped of letters aside from A-Z and apostrophe. It is then run through a hash function and added to linked lists stored in nodes. Outputs two results, the non verbose includes the probability a word appears in the language. The verbose shows the string, its hash code, the bucket it lives in, and the number of times it shows up.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

// Maximum length of words that we support
#define MAX_WORD_LEN 255

// Represents a single entry in a bucket's null-terminated linked list
// You should NOT modify this data type
typedef struct node
{
    char* word;  	 // Null-terminated string for this word
    unsigned int count;  // How often we saw this word in the training file
    struct node* next;   // Pointer to next Node in this bucket
} Node;

// Function prototypes, this allows the function implementations to appear in any order in the file
// You will implement these functions:
//convert the null-terminated string str into an unsigned int hash value
unsigned int hashString(const char* str){
  int hash = 0;
  for (int i = 0; i < strlen(str); i++){
    hash = 31*hash + str[i];
  }
  return hash;
}

//calculate the log base 10 probability of count divided by total
double calcUnigramLogProb(unsigned int count, unsigned long total){
  return log10((double)count/total);
}

//convert a string to uppercase and strip any character that are not A-Z or apostrophe
void uppercaseAndStrip(char* dest, const char* src){
  memset(dest, 0, strlen(dest)); //Clears dest of left over entries
  strncpy(dest, src, strlen(src));
  for (int i = 0; i < strlen(dest); i++) {
    if(dest[i] >= 'a' && dest[i] <= 'z') {
      dest[i] = dest[i] - 32;
    }
  }

  //Strips non alphabet/apostrophe 
  for (int i = 0; i < strlen(dest); i++) {
    while(!(dest[i] >= 'A' && dest[i] <= 'Z') && !(dest[i] == '\'') && !(dest[i] == '\0')){
      for (int j = i; dest[j] != '\0'; j++){
        dest[j] = dest[j+1];
      }
    }
  }
  //dest[strlen(src) + 1] = '\0';
}

// Test function prototypes, you should NOT modify these
void testHashString();
void testCalcUnigramLogProb();
void testUppercaseAndStrip();
void testFillAllPrintable(char* str);

// Fill an array entry with all printable ASCII characters
void testFillAllPrintable(char* str)
{
    // Create a maximum size string with all printable ASCII characters
    int index = 0;
    char ch = ' ';
    while (index < MAX_WORD_LEN)
    {
        str[index] = ch;
        ch++;
        if (ch > 126)
        {
            ch = ' ';
        }
        index++;
    }
}

// Test function for the hashString function
void testHashString()
{
    char tests[][MAX_WORD_LEN + 1] = {"BAD", "DAB", "GOODFELLOWS", "WRITERSHIP", "a", "A", "abcdefghijklmnopqrstuvwxyz", "1234567890!@#$%^&*()", "", ""};
    testFillAllPrintable(tests[8]);
    
    int i = 0;
    while (strlen(tests[i]) > 0)
    {
        printf("\"%s\" -> %u\n", tests[i], hashString(tests[i]));
        i++;
    }
}

// Test function for the calcUnigramLogProb function
void testCalcUnigramLogProb()
{
    const int SIZE = 10;
    unsigned int  counts[] = { 5 ,  50 ,   1,    1,   1234,   0 , 100, 1, 1        , UINT_MAX - 10000 };
    unsigned long totals[] = {10 , 100 , 100, 2000, 567890, 123 , 100, 1, ULONG_MAX, UINT_MAX         };
    
    for (int i = 0; i < SIZE; i++)
    {
        printf("%5u %7lu -> %9.6f\n", counts[i], totals[i], calcUnigramLogProb(counts[i], totals[i]));
    }
}

// Test function for the stripLower function
void testUppercaseAndStrip()
{
    char tests[][MAX_WORD_LEN + 1] = {"b", "B", "bad", "BAD", "BaD", "CAN'T", "well-done", "!L00K-", "1234567890", "abcdefghijklmnopqrstuvwxyz", "", ""};
    char dest[MAX_WORD_LEN + 1];
    testFillAllPrintable(tests[10]);
    
    int i = 0;
    while (strlen(tests[i]) > 0)
    {
        printf("\"%s\" (len %zu) -> ", tests[i], strlen(tests[i]));
        uppercaseAndStrip(dest, tests[i]);
        printf("\"%s\" (len %zu)\n", dest, strlen(dest));
        i++;
    }
}


int main(int argc, char** argv){
  // If no run line input we print out a help message and also run test functions
  if (argc <= 1) {
    printf("Usage: Unigram <hash table size> [debug]\n\n");
    testHashString();
    printf("\n");
    testCalcUnigramLogProb();
    printf("\n");
    testUppercaseAndStrip();
    return 0;
  }

  if (atoi(argv[1]) <= 0) { //error handling
    printf("ERROR: Table size must be positive!\n");
    return 0;
  }

  int hashTableSize = atoi(argv[1]); 
  struct node* arrayForLinkedList[hashTableSize]; //initialized the linked list
  for(int i = 0; i < hashTableSize; i++){ //set everything to null/0/empty
      struct node* empty = malloc(sizeof(Node));
      empty->word = "null";
      empty->count = 0;
      empty->next = NULL;
    arrayForLinkedList[i] = empty;
  }
  
  int iterationCounter = 0;
  bool run = 1;
  while(run == 1){ 
    int returnCount = 0;
    int bucket = 0;

    struct node* structNode = malloc(sizeof(Node)); //mallocing the size of the Node
    structNode->word = malloc(sizeof(char)*MAX_WORD_LEN+1); //mallocs the word size

    char buffer[256] = {}; //buffer used to read in data from file
    char arrayOfStrippedChars[256] = {}; //keeps chars after sending to uppercaseAndStrip

    if(scanf("%s", buffer) >= 0){   //reads into the buffer
      uppercaseAndStrip(arrayOfStrippedChars, buffer);  //function call returns the correct chars
      if(strcmp(arrayOfStrippedChars, "") == 0){
        continue;
      }
      strcpy(structNode->word, arrayOfStrippedChars); // copies strings into structNode->word
      bucket = hashString(structNode->word) % hashTableSize; //returns hashString mods it with hashtable size and returns and int from that
      
      if(arrayForLinkedList[bucket]->count == 0){ //if bucket is 0 create first node of Linked List
        structNode->next = NULL;
        structNode->count = 1;
        returnCount = structNode->count;
        free(arrayForLinkedList[bucket]);
        arrayForLinkedList[bucket] = structNode;  
      }else{ 
        struct node* current = arrayForLinkedList[bucket];
        while(current->count != 0){  
          char* currentWord = current->word;
          if(strcmp(currentWord, structNode->word) == 0){ //if the same, increase count
            current->count += 1;
            returnCount = current->count;
            break;
          }
        
          if(current->next != NULL){ //iterates over list to find matching
            current = current->next;
          }else{ //adds to the front of the list 
            structNode->next = arrayForLinkedList[bucket];
            arrayForLinkedList[bucket] = structNode;
              structNode->count = 1;
              returnCount = structNode->count;
            break;
          }
        }
      }
    }else{ //updates while loop for termination
      run = 0;
    }
    iterationCounter++;
    
    if (run == 1 && (argc == 3 || !(atoi(argv[3]) == 0))){ //prints verbose output
      printf("%s -> hash %u, bucket %d, count %d\n", structNode->word, hashString(structNode->word), bucket, returnCount);
    }

    if(run == 0){
      for(int i = 0; i < hashTableSize; i++){ //prints NON verbose output 
        struct node *current = arrayForLinkedList[i];
        while(current->count != 0){
          printf("%.6f %s\n", calcUnigramLogProb(current->count, iterationCounter-1), current->word);
          if(current->next != NULL){
            struct node* toBeFree = current;
            current = current->next;
            free(toBeFree->word);
            free(toBeFree);
          }
          else{
            break;
          }
        }
      }
    }
  } 
  return 0;
}