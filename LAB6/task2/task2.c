#include <stdio.h>
#include <stdlib.h>

/* IMPLEMENT ME: Declare your functions here */
int add (int a, int b);
int subtract (int a, int b);
int multiply (int a, int b);
int divide (int a, int b);


int main (void)
{
	/* IMPLEMENT ME: Insert your algorithm here */
	int a = 6;
	int b = 3;
	char op;

	int (*operations[])(int,int) = {add, subtract, multiply, divide};
	while (1){
		printf("Operand 'a' : %d | Operand 'b' : %d", a, b);
		printf("Enter the operation you want to perform | 0 : + , 1 : - , 2 : * , 3 : / , 4 : exit");
		scanf("%c", &op);

		int index = op - 0;
		if (index == 4) break;

		int valid = (index >= 0 && index <= 3);

		int result = valid ? operations[index] (a,b) : 0;
		printf("X = %d\n", result);	

	}
	return 0;
}

/* IMPLEMENT ME: Define your functions here */
int add (int a, int b) { printf ("Adding 'a' and 'b'\n"); return a + b; }
int subtract (int a, int b) { printf ("Subtracting 'a' and 'b'\n"); return a - b; }
int multiply (int a, int b) { printf ("Multiplying 'a' and 'b'\n"); return a * b; }
int divide (int a, int b) { printf ("Dividing 'a' and 'b'\n"); return a / b; }
