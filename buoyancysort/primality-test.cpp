#include "stdafx.h"

#include "primality-test.h"

// Yes, I realize how lazy I am for not making a new project.

namespace PrimalityTest
{
	// Data structures:
	// Static prime cache (perfect hash) of primes e.g. up to 2^(32-1) -- optimization for common cases
	// Dynamic cache (array, using GallopSearch) of primes (updated at the end of the algorithm)
	// Memoization (binary tree) of primes (stores any primes that do not exist in the static or dynamic cache for the entire duration of the algorithm)
	// Static factorial cache (array, using GallopSearch)

	bool is_a140475(long long n);

	long long factorial(long long n)
	{
		if (n <= 1)
		{
			return 1;
		}
		return n * factorial(n - 1);
	}

	bool is_prime(long long n)
	{
		if (n <= 3)
		{
			return n != 1;
		}
		long long low_factorial = 1;
		long long high_factorial = 1;
		while (low_factorial < n) // Ideally, we iterate in the opposite direction (I think), since we can do an early "break;" when the index is prime (and avoid tons of modulo operators).
		{

		}
	}

	bool is_a140475(long long n) // http://oeis.org/A140475
	{
		if (n <= 3)
		{
			return n == 1;
		}
		return is_prime(n);
	}
}