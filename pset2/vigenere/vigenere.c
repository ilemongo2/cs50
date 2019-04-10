#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cs50.h>

/*Constants*/
#define NUM_ALPH 26u
#define DECIMAL_BASE 10u
#define CASE_CONV_FACTOR 32u // lower case to upper case conv factor
/*Variables*/
/*Function Prototypes*/
void encrypt( char* c_ptr, char *key_ptr);
int is_alphabetical(char* s_ptr); // returns 1 if string pointed to by s_ptr is alphabetical and 0 otherwise

int main(int argc, char *argv[])
{
  int k_pos = 1;  // position of the key among the input arguments
  /* Check inputs to main */
  if (argc != 2 || 0 == is_alphabetical(argv[k_pos]))
  {
  printf("Incorrect usage. Please verify number or arguments and make sure the key is alphabetical.\n");
  return 1;
  }


  /* Set up key for encryption */
  char *k_ptr = argv[k_pos];

  /* Get message from user and encrypt it */
  char *ciphertext;
  ciphertext = get_string("plaintext: ");
  encrypt(ciphertext, k_ptr);
  printf("ciphertext: %s\n", ciphertext);

  return 0;
}



char toupper(char lower_c)
{
    char ret;
    if (lower_c >= 'a' && lower_c <= 'z')
    {
        ret = lower_c - CASE_CONV_FACTOR;
    }
    else
    {
        ret = lower_c;
    }
    return ret;
}

void encrypt( char *c_ptr, char *charkey)
{
  int a_idx; // alphabet index: a is 1 and z is 26
  char c_letter; // encrypted letter (for cipher)
  int i;
  int key_idx = 0; //stores the index of the current key as we loop through the keyword
  int key;         // stores the current key used to encrypt a character
  for(i = 0; c_ptr[i] != '\0'; i++  )
  {
    key = toupper(charkey[key_idx]) - 'A';
    if(c_ptr[i] >= 'A' && c_ptr[i] <= 'Z')
    {
      a_idx = c_ptr[i] - 'A';
      c_letter = 'A' + (a_idx + key) % NUM_ALPH;
      c_ptr[i] = c_letter;
      key_idx++;

    }
    else if( c_ptr[i] >= 'a' && c_ptr[i] <= 'z')
    {
      a_idx = c_ptr[i] - 'a';
      c_letter = 'a' + (a_idx + key) % NUM_ALPH;
      c_ptr[i] = c_letter;
      key_idx++;
    }
    else
    {
      //do nothing
    }
    /* Make sure we don't index outside of the key array*/
    if(key_idx >= strlen(charkey)){
        key_idx = 0;
    }
    /*Debug Statements*/
    /*
    printf("a_idx %d\n", a_idx);
    printf("key %d\n", key);
    printf("c_ptr %c\n", c_ptr[i]);
    */
  }
}

int is_alphabetical(char* s_ptr)
{
    int i = 0;
    int ret = 0;
    char c;
    while(s_ptr[i] != '\0')
    {
        c = toupper(s_ptr[i]);
        i++;
        if(!(c >= 'A' && c <= 'Z'))
        {
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }
    return ret;
}
