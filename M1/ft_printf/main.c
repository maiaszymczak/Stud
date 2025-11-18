#include "ft_printf.h"
#include <stdio.h>
#include <limits.h>

int main(void)
{
	int	ret1;
	int	ret2;

	printf("=== TEST CHARACTERS ===\n");
	ret1 = ft_printf("ft_printf: [%c]\n", 'A');
	ret2 = printf("   printf: [%c]\n", 'A');
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST STRINGS ===\n");
	ret1 = ft_printf("ft_printf: [%s]\n", "Hello, World!");
	ret2 = printf("   printf: [%s]\n", "Hello, World!");
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%s]\n", "");
	ret2 = printf("   printf: [%s]\n", "");
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	// Test with NULL string (commented out to avoid compilation warnings)
	// ret1 = ft_printf("ft_printf: [%s]\n", (char *)NULL);
	// ret2 = printf("   printf: [%s]\n", (char *)NULL);
	// printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST POINTERS ===\n");
	int x = 42;
	ret1 = ft_printf("ft_printf: [%p]\n", &x);
	ret2 = printf("   printf: [%p]\n", &x);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%p]\n", NULL);
	ret2 = printf("   printf: [%p]\n", NULL);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST INTEGERS ===\n");
	ret1 = ft_printf("ft_printf: [%d]\n", 42);
	ret2 = printf("   printf: [%d]\n", 42);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%i]\n", -42);
	ret2 = printf("   printf: [%i]\n", -42);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%d]\n", 0);
	ret2 = printf("   printf: [%d]\n", 0);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%d]\n", INT_MIN);
	ret2 = printf("   printf: [%d]\n", INT_MIN);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%d]\n", INT_MAX);
	ret2 = printf("   printf: [%d]\n", INT_MAX);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST UNSIGNED ===\n");
	ret1 = ft_printf("ft_printf: [%u]\n", 42);
	ret2 = printf("   printf: [%u]\n", 42);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%u]\n", 0);
	ret2 = printf("   printf: [%u]\n", 0);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%u]\n", UINT_MAX);
	ret2 = printf("   printf: [%u]\n", UINT_MAX);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST HEXADECIMAL (lowercase) ===\n");
	ret1 = ft_printf("ft_printf: [%x]\n", 255);
	ret2 = printf("   printf: [%x]\n", 255);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	ret1 = ft_printf("ft_printf: [%x]\n", 0);
	ret2 = printf("   printf: [%x]\n", 0);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST HEXADECIMAL (uppercase) ===\n");
	ret1 = ft_printf("ft_printf: [%X]\n", 255);
	ret2 = printf("   printf: [%X]\n", 255);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST PERCENT ===\n");
	ret1 = ft_printf("ft_printf: [%%]\n");
	ret2 = printf("   printf: [%%]\n");
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	printf("=== TEST MIXED ===\n");
	ret1 = ft_printf("ft_printf: %c %s %p %d %i %u %x %X %%\n", 'Z', "test", &x, 42, -42, 100, 255, 255);
	ret2 = printf("   printf: %c %s %p %d %i %u %x %X %%\n", 'Z', "test", &x, 42, -42, 100, 255, 255);
	printf("Return values: ft=%d, std=%d\n\n", ret1, ret2);

	return (0);
}
