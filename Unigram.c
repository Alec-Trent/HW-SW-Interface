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
unsigned int hashString(const char* str);
double calcUnigramLogProb(unsigned int count, unsigned long total);
void uppercaseAndStrip(char* dest, const char* src);

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

int main(int argc, char** argv)
{
   // If no command line input we print out a help message and also run test functions
   if (argc <= 1)
   {
      printf("Usage: Unigram <hash table size> [debug]\n\n");
      testHashString();
      printf("\n");
      testCalcUnigramLogProb();
      printf("\n");
      testUppercaseAndStrip();
      return 0;
   }

   // TODO: code!
   
   return 0;
}
