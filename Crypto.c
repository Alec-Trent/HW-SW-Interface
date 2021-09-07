// Main program for encrypting and decrypting files.
// You only need to implement the main loop that reads and writes data.

#include <stdio.h>
#include "Key.h"
#include "Checksum.h"

int main(int argc, char** argv)
{
   // If we don't get enough command line options, print a helpful error message
   if (argc < 4)
   {
      printf("Usage: Crypto <input file> <key file> <output file> [debug]\n");
      return 1;
   }
   char* inputFilename = argv[1];
   char* keyFilename = argv[2];
   char* outputFilename = argv[3];

   // Check if we are suppose to print debug output
   int debug = 0;
   if (argc > 4)
   {
      debug = atoi(argv[4]);
   }

   // Open the input file
   FILE* fpIn = fopen(inputFilename, "rb");
   if (fpIn == NULL)
   {
      printf("Failed to open input file: %s\n", inputFilename);
      return 1;
   }

   // Load the key data from the file on disk
   Key key;   
   if (!initKey(&key, keyFilename))
   {
      printf("Failed to load key file: %s\n", keyFilename);
      fclose(fpIn);
      fpIn = NULL;
      return 1;
   }

   // Open the output file
   FILE* fpOut = fopen(outputFilename, "wb");
   if (fpOut == NULL)
   {
      printf("Failed to open output file: %s\n", outputFilename);
      fclose(fpIn);
      fpIn = NULL;
      return 1;
   }

   // Prepare a checksum to track the input and output bytes
   Checksum inChecksum;
   Checksum outChecksum;

   initChecksum(&inChecksum);
   initChecksum(&outChecksum);

   // TODO: Process all the bytes from the input file, writing to the output file   
   
   // Output the checksums of the input and output data
   char check[5];
   getChecksum(&inChecksum, check);
   printf("Input checksum  : %s\n", check);
   
   getChecksum(&outChecksum, check);
   printf("Output checksum : %s\n", check);
   
   // Close the open input and output file
   fclose(fpIn);
   fpIn = NULL;
   fclose(fpOut);
   fpOut = NULL;
   
   // Release any resources allocated by the Key struct   
   uninitKey(&key);
   
   return 0;
}
