/**
 * Processes the raw file input string into two separate arrays of numbers.
 * This function performs the necessary string manipulation and type conversion.
 * * @param {string} input - The raw string content read from the file.
 * @returns {Array<Array<number>>} An array containing two lists: [leftCol, rightCol].
 */
const formatLists = (input) => {
  // .trim(): Removes leading/trailing whitespace from the entire file content
  // .split("\n"): Splits the cleaned string into an array of individual lines (rows).
  const rows = input.trim().split("\n");

  // Initialize two arrays (lists) to hold the numbers from the respective columns.
  const leftCol = [];
  const rightCol = [];

  // Iterate over each line (row) read from the input file.
  for (let i = 0; i < rows.length; i++) {
    // .split(/\s+/): Splits the current row string by one or more whitespace characters.
    // This correctly handles multiple spaces or tabs between the two numbers.
    const [left, right] = rows[i].split(/\s+/);

    // .push(+value): Adds the value to the list. The unary plus operator (+)
    // is a fast, explicit way to convert a string ('left' or 'right') into a number.
    leftCol.push(+left);
    rightCol.push(+right);
  }
  // Return the two populated lists.
  return [leftCol, rightCol];
};

/**
 * Calculates the 'Total Distance' between the two columns. 
 * This requires sorting both lists and pairing them by index.
 * * @param {string} ids - The raw file input string.
 * @returns {number} The sum of absolute differences.
 */
const totalDistance = (ids) => {
  // Deconstruct the array returned by formatLists into the two column arrays.
  const [leftCol, rightCol] = formatLists(ids);

  // .sort((a, b) => a - b): Sorts the array numerically in ascending order (in-place).
  const leftSorted = leftCol.sort((a, b) => a - b);
  const rightSorted = rightCol.sort((a, b) => a - b);

  let distance = 0;
  // Iterate over the sorted lists. The length should be the same.
  leftSorted.forEach((id, i) => {
    // Get the corresponding value from the second list.
    const partner = rightSorted[i];
    
    // The absolute difference is calculated as (largest - smallest) to avoid using Math.abs
    const smallest = Math.min(id, partner);
    
    // Add the absolute difference to the running total.
    distance += largest - smallest;
  });
  return distance;
};

/**
 * Calculates the 'Similarity Score' or 'Weighted Sum' (Part 2). 
 * The logic is: Sum(element_A * count_of_element_A_in_B)
 * * @param {string} ids - The raw file input string.
 * @returns {number} The calculated similarity score.
 */
const idPow = (ids) => {
  // Read and format the data again. (Performance could be improved by passing the lists
  // directly from totalDistance, but this ensures function isolation).
  const [leftCol, rightCol] = formatLists(ids);
  
  // Initialize an object (hash map) to store the frequency of numbers in the second column.
  const numberCounts = {};
  
  // FIRST PASS: Populate the frequency map using the second column (rightCol).
  rightCol.forEach((num) => {
    // Check if the number already exists as a key in the map.
    if (num in numberCounts) {
      numberCounts[num]++;
    } else {
      numberCounts[num] = 1; // Initialize count to 1.
    }
  });

  let popularity = 0;
  // SECOND PASS: Calculate the weighted sum using the first column (leftCol).
  leftCol.forEach((num) => {
    // Get the count ('pow') of the current number 'num' from the map.
    // The || 0 ensures that if 'num' is not found in the map, 'pow' defaults to 0.
    const pow = numberCounts[num] || 0;
    
    // Add the element multiplied by its count in the other list.
    popularity += num * pow;
  });
  return popularity;
};

// --- MAIN EXECUTION BLOCK ---

const fs = require("fs");
// fs.readFileSync: Reads the file 'input.txt' synchronously into a string.
// "utf-8": Specifies the text encoding.
const input = fs.readFileSync("../inputs/input-01.txt", "utf-8");

// Calls both functions with the raw input string and logs the results to the console.
console.log(totalDistance(input), " and ", idPow(input));
