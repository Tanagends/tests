#include <stdio.h>
int zimbo_add(int a, int b)
{
	return a + b;
}
int main()
{
	int example;

	example = zimbo_add(5, 1);
	printf("%i\n", example);
	return 0;
}
