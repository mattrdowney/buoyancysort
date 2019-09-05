#pragma once

#include <cstddef>
#include <iostream>

namespace IntThatTracksComparisons
{
	class IntThatTracksComparisons
	{
	public:
		int data;

		IntThatTracksComparisons() { data = 0; }
		IntThatTracksComparisons(int const &wrapped_data) { data = wrapped_data; }
		IntThatTracksComparisons(int &&relocatable_data) { data = std::move(relocatable_data); }

		int& operator=(int const &wrapped_data) { data = wrapped_data; return data; }
		int& operator=(int &&relocatable_data) { data = std::move(relocatable_data); return data; }
		
		friend std::ostream& operator<< (std::ostream &stream, const IntThatTracksComparisons &data);

		friend bool operator== (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right);
		friend bool operator!= (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right);

		friend bool operator> (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right);
		friend bool operator<= (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right);

		friend bool operator< (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right);
		friend bool operator>= (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right);
	};

	static long long comparison_counter = 0; // std::size_t is unreliable (I had to change it because it overflowed on 100M-sized array (2^32 wasn't enough))

	long long get_comparisons()
	{
		return comparison_counter;
	}

	void reset_comparisons()
	{
		comparison_counter = 0;
	}

	bool operator== (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right)
	{
		comparison_counter += 1;
		return left.data == right.data;
	}

	bool operator< (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right)
	{
		comparison_counter += 1;
		return left.data < right.data;
	}

	bool operator<= (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right)
	{
		comparison_counter += 1;
		return left.data <= right.data;
	}

	bool operator!= (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right)
	{
		//comparison_counter += 1; // TODO: VERIFY // amusing bug
		return !(left == right);
	}

	bool operator> (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right)
	{
		//comparison_counter += 1; // TODO: VERIFY // amusing bug
		return !(left <= right);
	}

	bool operator>= (const IntThatTracksComparisons &left, const IntThatTracksComparisons &right)
	{
		//comparison_counter += 1; // TODO: VERIFY // amusing bug
		return !(left < right);
	}

	std::ostream& operator<< (std::ostream &stream, const IntThatTracksComparisons &data)
	{
		stream << data.data;
		return stream;
	}
}