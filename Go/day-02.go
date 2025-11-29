// Ref: append(foo[:i], foo[i+1:]...) overwrites foo. https://stackoverflow.com/a/57213476/1200316

package main

import (
	"encoding/json" // Package for JSON encoding and decoding, used here for fast array parsing.
	"fmt"           // Package for formatted I/O (printing results).
	"os"            // Package for operating system interactions (reading files).
	"slices"        // Go 1.21+ package with optimized slice operations (cloning, deleting).
	"strings"       // Package for string manipulation (trimming, splitting, replacing).
)

// The main function is the entry point for the Go program execution.
func main() {
	// os.ReadFile reads the entire contents of "input.txt" into a byte slice.
	// The underscore (_) ignores the error returned by os.ReadFile for simplicity.
	input, _ := os.ReadFile("../inputs/input-02.txt")

	// Initialize counters for the two parts of the problem.
	part1, part2 := 0, 0

	// --- Input Processing Pipeline ---

	// string(input): Converts the byte slice into a string.
	// strings.TrimSpace(...): Removes leading/trailing whitespace (including newlines) from the file content.
	// strings.Split(..., "\n"): Splits the cleaned string into an array of individual lines (reports).
	// The loop iterates over each line (s) in the resulting slice of strings.
	for _, s := range strings.Split(strings.TrimSpace(string(input)), "\n") {

		// Variable to hold the parsed report numbers as a slice of integers.
		var report []int

		// --- Efficient String-to-Slice Parsing using JSON ---

		// strings.ReplaceAll(s, " ", ","): Replaces all spaces in the line with commas.
		//    (e.g., "1 2 3" becomes "1,2,3")
		// "[" + ... + "]": Wraps the comma-separated numbers in square brackets to form a valid JSON array string. (e.g., "[1,2,3]")
		// []byte(...): Converts the JSON string to a byte slice, required by Unmarshal.
		// json.Unmarshal(...): Parses the byte slice and efficiently populates the 'report' slice.
		json.Unmarshal([]byte("["+strings.ReplaceAll(s, " ", ",")+"]"), &report)

		// --- Part 1: Check original report safety ---
		if check(report) {
			part1++ // Increment if the original sequence is safe.
		}

		// --- Part 2: Check safety after removing one element ---
		// Iterate through every possible index 'i' to simulate deleting one element.
		for i := range report {
			// 1. slices.Clone(report): Creates a shallow copy of the report slice. This is essential
			//    because slices.Delete modifies the slice in place.
			// 2. slices.Delete(..., i, i+1): Creates a new slice with the element at index 'i' removed.
			//    The slice is passed to the 'check' function.
			if check(slices.Delete(slices.Clone(report), i, i+1)) {
				part2++ // Increment if a safe sub-sequence is found.
				break   // Stop checking this row and move to the next file line (since we only need ONE safe deletion).
			}
		}
	}

	// Print the final counts for Part 1 and Part 2.
	fmt.Println("Part 1 answer: ", part1)
	fmt.Println("Part 2 answer: ", part2)
}

// --- Safety Check Logic ---

// check verifies if the report sequence 'r' is safe according to the problem rules.
// A sequence is safe if the differences between consecutive numbers are:
// 1. Consistently positive OR consistently negative (no sign flips).
// 2. Have an absolute value between 1 and 3 (inclusive).
func check(r []int) bool {
	// A sequence with 0 or 1 element is trivially safe.
	if len(r) < 2 {
		return true
	}

	// Loop starts at the second element (i=1) to calculate the difference with the previous element (i-1).
	for i := 1; i < len(r); i++ {
		// Calculate the difference (d) between the current element and the previous one.
		d := r[i] - r[i-1]

		// Get the sign of the *first* difference (which dictates the required trend: increasing/decreasing).
		// This uses the first two elements of the *current* sequence being checked (r).
		firstDiffSign := r[1] - r[0]

		// --- Check 1: Sign Consistency ---
		// d * firstDiffSign <= 0: This is a fast way to check if the sign flipped or if d is zero.
		// If both signs are the same (d > 0 and firstDiffSign > 0, OR d < 0 and firstDiffSign < 0),
		// the product will be positive. If the trend flips, the product will be negative (<= 0).

		// --- Check 2: Magnitude Constraint ---
		// d < -3 || d > 3: Checks if the absolute difference is too large (> 3).

		if d*firstDiffSign <= 0 || d < -3 || d > 3 {
			return false // Immediately fail if any single difference violates the rules.
		}
	}

	// If the loop completes without finding any violations, the sequence is safe.
	return true
}
