#include <stdio.h>
#include <cs50.h>
#include <math.h>

// Constant definitions
#define QUARTER 25.0
#define DIME 10.0
#define NICKEL 5.0
#define PENNY 1.0


// Function prototypes

// Body
int main(void)
{
	double amt_Owed;
	int num_Cents;
	int num_Quarters;
	int num_Dimes;
	int num_Nickels;
	int num_Pennies;
	int min_Num_Coins;

	do {
		amt_Owed = get_float("Please enter the amount owed: ");  // Take amount owed from user
	}
	while(amt_Owed < 0);

	num_Cents = round(amt_Owed*100.0); // Multiplying by a 100.0 and then rounding to get an integer between 0 and 100.
	num_Quarters = num_Cents / QUARTER;
	num_Dimes = (num_Cents - num_Quarters*QUARTER) / DIME;
	num_Nickels = (num_Cents - num_Quarters*QUARTER - num_Dimes*DIME) / NICKEL;
	num_Pennies = (num_Cents - num_Quarters*QUARTER - num_Dimes*DIME - num_Nickels*NICKEL) / PENNY;
	min_Num_Coins = num_Quarters + num_Dimes + num_Nickels + num_Pennies;

    printf("Change owed: %i\n", min_Num_Coins);
}