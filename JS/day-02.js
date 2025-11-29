/**
 * @function isSafe
 * @description This function checks if a sequence of numbers (levels) is 'safe'
 * based on a specific set of rules (strict monotonicity and difference limit)
 * @param {number[]} levels - An array of integers representing a sequence/report.
 * @returns {boolean} - True if the sequence meets the criteria, false otherwise.
 */
function isSafe(levels) {
  // 1. Calculate the difference between every adjacent pair of numbers.
  const differences = [];
  for (let i = 1; i < levels.length; i++) {
    // Push (Current value - Previous value)
    differences.push(levels[i] - levels[i - 1]);
  }

  // 2. Check for a STRICTLY INCREASING trend where every difference 'd' is:
  //    - Greater than or equal to 1 (strictly increasing, no equality allowed)
  //    - Less than or equal to 3 (difference limit)
  const increasing = differences.every((d) => d >= 1 && d <= 3);

  // 3. Check for a STRICTLY DECREASING trend where every difference 'd' is:
  //    - Less than or equal to -1 (strictly decreasing, no equality allowed)
  //    - Greater than or equal to -3 (difference limit)
  const decreasing = differences.every((d) => d <= -1 && d >= -3);

  // 4. The sequence is 'safe' if it is either perfectly increasing OR perfectly decreasing.
  return increasing || decreasing;
}

/**
 * @function sortReports
 * @description Main solver function that processes all input reports to calculate
 * the count of 'naturally safe' reports (Part 1) and 'actual safe' reports (Part 2).
 * It relies on the global 'input' variable for data.
 * @returns {number[]} - An array containing [safeCount, madeSafeCount].
 */
function sortReports() {
  // Convert the raw text input into a 2D array of numbers.
  const reports = input
    .trim() // Remove leading/trailing whitespace (crucial for removing trailing newline)
    .split("\n") // Split the cleaned string into an array of lines/reports
    .map(
      (line) =>
        line
          .split(" ") // Split the line string into an array of number strings (assuming single space delimiter)
          .map(Number) // Convert the array of strings to an array of numbers
    );

  // Initialize counters for the two parts of the problem
  let safe,
    madeSafe = 0;

  // Iterate through each report/sequence
  for (const report of reports) {
    let tolerable = false; // Flag to track if the report can be made safe by removal

    // (Part 2 Logic): Try removing every single element one-by-one
    for (let i = 0; i < report.length; i++) {
      // Create a new array 'removed' by splicing out the element at index 'i'.
      // This is achieved by combining the slice before 'i' and the slice after 'i'.
      const removed = [...report.slice(0, i), ...report.slice(i + 1)];

      // Check if the sequence becomes safe after removing this element
      if (isSafe(removed)) {
        tolerable = true; // Success! The report can be made safe
        break; // Once one successful removal is found, stop checking others
      }
    }

    // Check and count reports that were safe initially
    const isReportSafe = isSafe(report); // Calculate safety only once
    if (isReportSafe) {
      safe++;
    }

    // Check and count reports that are safe OR made safe
    if (isReportSafe || tolerable) {
      madeSafe++;
    }
  }

  // 6. Return the final counts
  return [safe, madeSafe];
}

// --- FILE READING AND EXECUTION ---

// Node.js module required for file system operations
const fs = require("fs");

// Read the entire content of the input file synchronously using UTF-8 encoding
// The content is stored globally in the 'input' variable.
const input = fs.readFileSync("input-02.txt", "utf-8");

// Execute the main solver function and log the results to the console.
const results = sortReports();
console.log(
  "Naturally safe reports: ",
  results[0], // Part 1 result
  " and ",
  "Actual safe reports: ",
  results[1] // Part 2 result
);
