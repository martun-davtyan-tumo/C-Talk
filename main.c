#include <stdio.h>

int main()
{
	int a = 5;
	
	printf("a = %d, address - %p\n", a, &a);

	int b = 3;
	int* ptr_b = &b;

	printf("%d\n", *ptr_b);

	*ptr_b = 10;

	printf("%d\n", *ptr_b);

	char letter = 'Z';
	char* ptr_letter = &letter;
	printf("%c %c %p %p\n", letter, *ptr_letter, &letter, ptr_letter);

	int nums[5] = {1, 2, 3, 4, 5};
	int* ptr_nums = nums;

	for (int i = 0; i < 5; i++)
	{
		printf("%d ", *ptr_nums + i);
	}
	printf("\n");

	int sum = 0;
	for (int i = 0; i < 5; i++)
	{
		sum += *ptr_nums + i;;
	}

	printf("Sum: %d\n", sum);

	ptr_nums += 2;
	printf("Third element: %d\n", *ptr_nums);

	return 0;
}
