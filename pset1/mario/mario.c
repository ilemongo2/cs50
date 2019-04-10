#include <stdio.h>
#include <cs50.h>
/* This code prints out a pyramid made up of two half pyramids on each side. The
# sign will be used for the building blocks of the pyramid by default */

int getPyramidHeight();                        // Prompts user to enter pyramid height
void buildPyramid(int h);                      // Builds a pyramid of height h
char base_Block = '#';                          // Character used for base blocks
int gap_Size = 2;

int main(void)
{
    int h = getPyramidHeight(); /* Prompts user to enter a number between 0 and 23 for the pyramid
                                   height. If the number entered is incorrect. The user will get
                                   prompted again */
    buildPyramid(h);
}

int getPyramidHeight()
{
    // prompt user for pyramid height
    int h;
    do
    {
        h = get_int("Please enter an integer between 0 and 23: ");
        //printf(" Height: %d \n", h);
    }
    while (h < 0 ||  h > 23);
    return h;
}


void print_Blocks(int b)      // Used for building the blocks , takes argument b for the number of blocks
{
    for (int i = 0; i < b; i++)
    {
        printf("%c", base_Block);
    }
}

void print_Spaces(int s)    // Used for creating spaces, take s argument s for the number of spaces
{
    for (int i = 0; i < s; i++)
    {
        printf(" ");
    }
}
le
void buildPyramid(int h)
{
    for (int i = 1; i <= h; i++)
    {
        print_Spaces(h - i);
        print_Blocks(i);
        print_Spaces(gap_Size);
        print_Blocks(i);
        printf("\n");

    }
}

