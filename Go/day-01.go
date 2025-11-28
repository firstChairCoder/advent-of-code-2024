package main

// Import necessary packages for file handling, string manipulation, formatting, and slices.
import (
	"fmt"     // Package for formatted I/O (like reading and printing)
	"os"      // Package for operating system functions (like reading files)
	"slices"  // Package for working with slices (used for sorting)
	"strings" // Package for string manipulation
)

// The main function is the entry point of the program.
func main() {
	// --- STEP 1: Reading the File and Error Handling ---

	// os.ReadFile("input.txt") reads the entire contents of the file into a byte slice.
	// The function returns two values: the data (byte slice) and an error.
	// The underscore (_) is used to ignore the potential error value for brevity.
	input, _ := os.ReadFile("../inputs/input-01.txt")

	// --- STEP 2: Initialization and Data Parsing ---

	// Declare two dynamic arrays (slices) to hold the numbers from the two columns.
	var list1, list2 []int

	// Declare a map (hash table) to store the count of each number in the second column (list2).
	// The keys are the numbers (int), and the values are their counts (int).
	counts2 := map[int]int{}

	// Convert the input byte slice to a string.
	inputStr := string(input)

	// strings.TrimSpace(inputStr) removes leading/trailing whitespace.
	// strings.Split(..., "\n") splits the string into a slice of strings based on newline characters,
	// giving us a slice where each element is a line from the file.
	for _, s := range strings.Split(strings.TrimSpace(inputStr), "\n") {
		// Declare two integer variables to hold the numbers read from the current line (s).
		var n1, n2 int

		// fmt.Sscanf reads formatted data from the string s and stores the results
		// into the memory addresses of n1 and n2.
		// The format "%d   %d" tells Sscanf to expect two integers separated by one or more spaces.
		// This is the Go equivalent of Python's mapping and splitting lines.
		fmt.Sscanf(s, "%d   %d", &n1, &n2)

		// append(list, element) dynamically adds the element to the slice.
		list1 = append(list1, n1)
		list2 = append(list2, n2)

		// Increment the count for the number n2 in the counts2 map.
		counts2[n2]++
	}

	// --- STEP 3: Sorting the Lists ---

	// slices.Sort performs an in-place sort on the elements of the slices.
	slices.Sort(list1)
	slices.Sort(list2)

	// --- STEP 4: Calculations ---

	// Initialize variables to store the results of the two calculations.
	part1, part2 := 0, 0

	// Iterate through the indices of list1 (since list1 and list2 are the same length).
	for i := range list1 {
		// --- Calculation for Part 1 (total distance between lists) ---
		// Note: Go's math.Abs only works on float64, so explicit casting is required.
		// The custom 'abs' function below is often preferred for integer math.
		part1 += abs(list2[i] - list1[i])

		// --- Calculation for Part 2 (similarity score) ---
		// It multiplies the current element of list1 by its count found in the second column data (counts2 map).
		part2 += list1[i] * counts2[list1[i]]
	}

	// --- STEP 5: Output Results ---

	fmt.Println(part1)
	fmt.Println(part2)
}

// Custom integer absolute value function.
// This is necessary because Go's built-in math.Abs requires float64 types.
func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}
