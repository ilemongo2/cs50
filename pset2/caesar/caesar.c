#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cs50.h>

/*Constants*/
#define NUM_ALPH 26u
#define DECIMAL_BASE 10u
/*Variables*/
/*Function Prototypes*/
void encrypt( char* c_ptr, int key);


int main(int argc, char *argv[])
{
  int k_pos = 1;  // position of the key among the input arguments
  /* Check inputs to main */
  if (argc != 2)
  {
  printf("Not enough or too many input arguments. Please check usage.");
  return 1;
  }

  /* Set up key for encryption. Here, we convert the key to an integer using strtol */
  char *endptr;
  int k = strtol(argv[k_pos],&endptr,DECIMAL_BASE);

  /* Get message from user and encrypt it */
  char *ciphertext;
  ciphertext = get_string("plaintext: ");
  encrypt(ciphertext, k);
  printf("ciphertext: %s\n", ciphertext);

  return 0;
}



void encrypt( char* c_ptr, int key)
{
  int a_idx; // alphabet index: a is 1 and z is 26
  char c_letter; // encrypted letter (for cipher)
  int i;
  for(i = 0; c_ptr[i] != '\0'; i++  )
  {
    if(c_ptr[i] >= 'A' && c_ptr[i] <= 'Z')
    {
      a_idx = c_ptr[i] - 'A';
      c_letter = 'A' + (a_idx + key) % NUM_ALPH;
      c_ptr[i] = c_letter;

    }
    else if( c_ptr[i] >= 'a' && c_ptr[i] <= 'z')
    {
      a_idx = c_ptr[i] - 'a';
      c_letter = 'a' + (a_idx + key) % NUM_ALPH;
      c_ptr[i] = c_letter;
    }
    else
    {
      //do nothing
    }
    /*Debug Statements*/
    /*
    printf("a_idx %d\n", a_idx);
    printf("key %d\n", key);
    printf("c_ptr %c\n", c_ptr[i]);
    */
  }
}