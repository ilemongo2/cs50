#include <stdio.h>
#include <math.h>
#include <cs50.h>


long long number;
int length;
// Function prototypes;
int get_CardType(int array[], int lngth);
int numLength(long long num);

// Body
int main(void){

    number = get_long_long("Please enter credit card number: ");
    length = numLength(number);
    //printf("number of #: %i\n", length);
    int digt;
    int digts_Store[length];
    // Let's extract the digits of "number" one by one
    for (int i = 0; i < length; i++)
    {
        digt = number % 10;
        digts_Store[i] = digt;
        number /= 10;

    }
    int cardType = get_CardType(digts_Store, length);
    digt = 0; // reinitializing digt for future use;
    int digt1; // used to store 1st (from right) digit of digt
    int digt2; // used to store 2nd (from right) digit of digt
    //int digts_Store2[2]; // Using this variable to store the digits of the result of multilying digts_Store elements by 2.
    int sum = 0; // Used to store the sum of the digits. This is the sum used for the credit card validity check.
    // Now let's multiply by 2 every other digit starting with the second from right digit
    for (int i = 1; i < length ; i += 2 )
    {
        digt = digts_Store[i];
        digt *= 2;
        digt1 = digt % 10;
        digt2 = digt/10;
        sum += digt1 + digt2;
        //printf("sum parameters: %i %i %i %i %i\n" , i, digt, digt1, digt2, sum);
    }
    // add sum to the sum of the digits that were not multiplied by 2
    for (int k = 0 ; k < length ; k += 2)
    {
        sum += digts_Store[k];
        //printf("sum: %i\n", sum);
    }
    //printf("sum: %i\n", sum);
    // Check card validity, if valid, return the type of the card to the user
    if(sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        //printf("type: %i\n\n\n\n", cardType);
        switch(cardType)
        {

            case 1 :
            printf("VISA\n");
            break;

            case 2:
            printf("MASTERCARD\n");
            break;

            case 3:
            printf("AMEX\n");
            break;

            default:
            printf("INVALID\n");
        }
    }

    return 0;
}

int get_CardType(int array[], int lngth){
/* type = 1 is VISA , type = 2 is MASTERCARD, type = 3 is AMEX*/
    int type = 0;
    {
        if (lngth == 13){
            type = 1;

        }
        else if (lngth == 15 && (array[lngth-1] == 3) && (array[lngth-2] == 4 || array[lngth-2] == 7))
        {
            type = 3;
        }
        else if(lngth == 16)
        {

            if (array[lngth-1] == 4)
            {
                type = 1;
            }
            else if ((array[lngth-1] == 5) && (array[lngth-2] <= 5) && (array[lngth-2] >= 1))
            {
              type = 2;
            }
        }
        return type;
    }
}

 int numLength(long long num){
     int len;
     len = log10(number) + 1;
     return len;
 }
