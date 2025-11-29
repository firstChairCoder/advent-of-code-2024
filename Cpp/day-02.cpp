#include <algorithm> // Required for std::abs (though <cmath> also provides abs for int)
#include <cctype>	 // Required for std::isspace
#include <charconv>	 // Required for std::from_chars (efficient string to integer conversion)
#include <cmath>	 // Required for std::abs (specifically for integer types)
#include <fstream>	 // Required for std::ifstream (file input)
#include <iostream>	 // Required for std::cout and std::endl (console I/O)
#include <string>	 // Required for std::string
#include <vector>	 // Required for std::vector

// --- Utility Functions Implementation ---

/**
 * @brief Parses a string containing space or comma-delimited integers into a
 * vector of ints. Uses std::from_chars for efficient parsing.
 * @param s The input string (e.g., "5 8, 4").
 * @return std::vector<int> A vector of the parsed integers.
 */
std::vector<int> DelimitedToInts(const std::string &s)
{
	std::vector<int> numbers;
	// Get raw C-style pointers for use with std::from_chars
	const char *str = s.c_str();
	const char *end = str + s.size();

	// Loop through the string, parsing integers one by one
	while (str < end)
	{
		// Skip leading whitespace or comma delimiters
		while (str < end && (std::isspace(*str) || *str == ','))
		{
			++str;
		}
		// If we reached the end of the string after skipping delimiters, break
		// the loop
		if (str >= end)
		{
			break;
		}

		int value;
		// std::from_chars attempts to parse an integer from [str, end)
		auto [ptr, ec] = std::from_chars(str, end, value);

		// Check if parsing was successful (ec == std::errc() means no error)
		if (ec == std::errc())
		{
			numbers.push_back(value);
			// Move the pointer 'str' to the position immediately after the
			// successfully parsed number
			str = ptr;
		}
		else
		{
			// If there's a parsing error (e.g., non-numeric characters found),
			// stop processing this line
			break;
		}
	}
	return numbers;
}

/**
 * @brief Reads a file line by line and converts each line into a vector of
 * integers using DelimitedToInts.
 * @param path The file path
 * @return std::vector<std::vector<int>> A vector where each inner vector is a
 * line from the file.
 */
std::vector<std::vector<int>> GetVectorIntsFromTxt(const std::string &path)
{
	std::string line;
	// Attempt to open the file specified by 'path'
	std::ifstream myfile(path);
	std::vector<std::vector<int>> vec;

	if (myfile.is_open())
	{
		// Read the file line by line until the end
		while (getline(myfile, line))
		{
			// Only process lines that are not empty
			if (!line.empty())
			{
				// Convert the line string into a vector of integers and add it
				// to the main vector
				vec.push_back(DelimitedToInts(line));
			}
		}
		// Close the file stream after reading is complete
		myfile.close();
	}
	else
	{
		// Output an error message if the file could not be opened
		std::cout << "ERROR: Unable to open file at path: " << path
				  << std::endl;
		// Return hardcoded mock data for demonstration if the file is missing
		return {{1, 2, 3}, {9, 6, 2}, {10, 7, 5, 2}};
	}
	return vec;
}

/**
 * @brief Abstract base class defining the required interface for a "Day"
 * solver. Ensures all derived classes implement PartOne() and PartTwo().
 */
class IDay
{
public:
	// Pure virtual functions: must be implemented by derived classes
	virtual void PartOne() = 0;
	virtual void PartTwo() = 0;
	// Virtual destructor: essential for proper cleanup of derived class objects
	virtual ~IDay() = default;
};

/**
 * @brief Implements the solution logic for Day 2, inheriting from IDay.
 */
class Day2 : public IDay
{
private:
	// Storage for any accumulated sums (currently unused)
	int part1Sum = 0;
	int part2Sum = 0;

	// Stores the input data: a vector of lines, where each line is a vector of
	// integers
	std::vector<std::vector<int>> data;

	// Helper methods for core logic
	bool processSafe(const std::vector<int> &nums);
	bool NumCheck(bool (*func)(int, int), int x, int y);
	std::vector<int> removeAtIndex(int index, const std::vector<int> &nums);

public:
	// Public interface required by IDay
	void PartOne() override;
	void PartTwo() override;
};

/**
 * @brief Solves Part One of the problem.
 * Finds the count of sequences in the input that are inherently "safe".
 */
void Day2::PartOne()
{
	// 1. Load the input data from the file
	data = GetVectorIntsFromTxt("../inputs/input-02.txt");

	int safeTotal = 0;
	// 2. Iterate over each sequence (inner vector) in the loaded data
	for (const std::vector<int> &nums : data)
	{
		// 3. Check if the sequence is safe according to the rules
		if (processSafe(nums))
		{
			safeTotal++;
		}
	}

	// 4. Output the final result
	std::cout << "Part 1: " << safeTotal << std::endl;
}

/**
 * @brief Solves Part Two of the problem.
 * Finds the count of sequences that are either safe, or can be made safe by
 * removing exactly one element.
 */
void Day2::PartTwo()
{
	int safeTotal = 0;
	// Iterate over the data (which was loaded in PartOne)
	for (std::vector<int> &nums : data)
	{
		// 1. Check if the sequence is already safe
		if (!processSafe(nums))
		{
			// 2. If unsafe, try removing each element one by one
			for (size_t i = 0; i != nums.size(); i++)
			{
				// Create a temporary sequence with the element at index 'i'
				// removed
				std::vector<int> temp = removeAtIndex(i, nums);

				// Check if the modified sequence is now safe
				if (processSafe(temp))
				{
					safeTotal++;
					// Found a valid removal, stop trying other removals for
					// this original sequence
					break;
				}
			}
		}
		else
		{
			// If it was already safe, count it
			safeTotal++;
		}
	}
	std::cout << "Part 2: " << safeTotal << std::endl;
}

/**
 * @brief Checks if a given sequence of numbers is "safe".
 * A sequence is safe if:
 * 1. The trend (increasing or decreasing) is set by the first two elements.
 * 2. Every subsequent adjacent pair follows that same trend.
 * 3. The absolute difference between any adjacent pair is <= 3.
 * @param nums The sequence of integers to check.
 * @return true if the sequence is safe, false otherwise.
 */
bool Day2::processSafe(const std::vector<int> &nums)
{
	// A sequence with 0 or 1 element is vacuously safe
	if (nums.size() < 2)
	{
		return true;
	}

	// funcPtr is a function pointer (or lambda) that checks the required trend
	// (x > y or x < y)
	bool (*funcPtr)(int, int) = [](int x, int y)
	{
		return x > y;
	}; // Default: Check for decreasing trend (x > y)

	// Determine the trend based on the first two elements
	if (nums[0] < nums[1])
	{
		// If the second element is larger, the required trend is increasing
		funcPtr = [](int x, int y)
		{ return x < y; };
	}
	// If nums[0] == nums[1], the default (decreasing, x>y) will cause the check
	// to fail in NumCheck unless we modify this logic for equality (based on
	// rules not provided). Assuming strict monotonic trend required (x < y OR x
	// > y).

	// Check all adjacent pairs against the determined trend
	for (size_t i = 0; i < nums.size() - 1; i++)
	{
		if (!NumCheck(funcPtr, nums[i], nums[i + 1]))
		{
			// If any pair fails the trend or difference check, the sequence is
			// unsafe
			return false;
		}
	}
	return true; // All checks passed
}

/**
 * @brief Performs the two safety checks on an adjacent pair of numbers.
 * @param func A function pointer/lambda representing the required trend check
 * (x > y or x < y).
 * @param x The current number.
 * @param y The next number.
 * @return true if both the trend and the difference checks pass.
 */
bool Day2::NumCheck(bool (*func)(int, int), int x, int y)
{
	// Check 1: Does the pair follow the required trend? (func(x, y))
	// Check 2: Is the absolute difference between them less than or equal to 3?
	return func(x, y) && (std::abs(x - y) <= 3);
}

/**
 * @brief Creates a new vector by removing the element at the specified index.
 * @param index The zero-based index of the element to remove.
 * @param nums The original vector.
 * @return std::vector<int> A new vector without the element at 'index'.
 */
std::vector<int> Day2::removeAtIndex(int index, const std::vector<int> &nums)
{
	std::vector<int> temp;
	for (size_t i = 0; i < nums.size(); i++)
	{
		// Copy all elements EXCEPT the one at the specified index
		if (i != index)
		{
			temp.push_back(nums[i]);
		}
	}
	return temp;
}

// --- main Function (Entry Point) ---

int main()
{
	// Instantiate the solver class for Day 2
	Day2 solver;

	// Run Part 1
	std::cout << "Running Part 1:\n";
	solver.PartOne();
	std::cout << "\n";

	// Run Part 2. It uses the 'data' vector already populated by PartOne.
	std::cout << "Running Part 2:\n";
	solver.PartTwo();
	std::cout << "\n";

	return 0;
}
