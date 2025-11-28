#include <algorithm> // For std::sort, std::count, and other algorithmss
#include <charconv>	 // For std::from_chars (fast string-to-int conversion)
#include <cstdint>	 // For int32_t
#include <cstdlib>	 // For std::abs (for integers)
#include <fstream>	 // For std::ifstream (file handling)
#include <iostream>	 // For std::cout, std::endl, std::cerr
#include <map>
#include <string> // For std::string, getline
#include <vector> // For std::vector (dynamic arrays)

// -- Placeholder for IDay.h ---
// Since we don't need the actual interface, we'll define a simple base class
// to satisfy the inheritance requirement without needing another file.
class IDay
{
};

// -- CLASS DEFINITION --
// Defines the main logic class, inheriting from the simple placeholder IDay.
class Day1 : public IDay
{
private:
	// Public members to store the data from the two columns. std::vector is the
	// C++ standard library's dynamic array
	std::vector<int32_t> list1;
	std::vector<int32_t> list2;

	// private function to handle all file I/O.
	bool ReadFileData();

public:
	// Declarations for the logic functions. The definitions follow below.
	void PartOne();
	void PartTwo();
};

bool Day1::ReadFileData()
{
	// Open the file. RAII: Destructor will automatically call myfile.close()
	// when scope ends.
	std::ifstream myfile("../inputs/input-01.txt.txt");

	if (!myfile.is_open())
	{
		std::cerr << "Error: Could not open file." << std::endl;
		return false;
	}

	int32_t left, right;

	// **IDIOMATIC C++ INPUT:** The `>>` operator reads formatted input.
	// It automatically skips all intermediate whitespace (spaces, tabs,
	// newlines) and attempts to convert the next non-whitespace sequence into
	// the target type (int32_t). The loop continues as long as two successful
	// reads occur.
	while (myfile >> left >> right)
	{
		list1.push_back(left);
		list2.push_back(right);
	}
	return true;
}

// --- METHOD IMPLEMENTATION: Day1::PartOne ---

void Day1::PartOne()
{
	// --- File Reading and Data Parsing ---

	if (!ReadFileData())
	{
		return; // Stop if file reading failed
	}

	// Sort the collected lists
	std::sort(list1.begin(), list1.end());
	std::sort(list2.begin(), list2.end());

	// Calculate Part One: total distance between lists
	long sum = 0;
	// Iterate and pair the sorted elements from both lists.
	for (size_t i = 0; i < list1.size(); i++)
		// Cast to 'long' before subtraction to avoid potential overflow during
		// calculation. std::abs from <cstdlib> is used for integer absolute
		// value.
		sum += std::abs((long)list1[i] - (long)list2[i]);

	std::cout << "Part 1: " << sum << std::endl;
}

void Day1::PartTwo()
{
	// OPTIMIZATION: Create a map to pre-count elements in list2 for faster
	// lookups. This makes the second loop much faster than repeatedly calling
	// std::count().
	std::map<int32_t, int> counts2;
	for (int32_t n : list2)
	{
		counts2[n]++;
	}

	long total = 0;
	// Calculate Part Two: similarity score
	for (int32_t i : list1)
	{
		// Multiply the element 'i' by its count found in the pre-calculated map
		// (counts2[i]). If i is not in the map, counts2[i] returns 0.
		total += (long)i * counts2[i];
	}

	std::cout << "Part 2: " << total << std::endl;
}

// --- MAIN ENTRY POINT ---

int main()
{
	// Create an instance of the solver class.
	Day1 solver;

	solver.PartOne();
	solver.PartTwo();

	return 0;
};
