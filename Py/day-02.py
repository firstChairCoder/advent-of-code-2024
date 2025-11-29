def is_safe(row):
    """
    Checks if a sequence of numbers is 'safe'. A sequence is safe if the difference
    between every consecutive pair of numbers (the 'increments') falls entirely
    within the set {1, 2, 3} or entirely within the set {-1, -2, -3}.

    The function uses set operations for a concise check of all unique differences.

    :param row: A list of integers (the sequence).
    :return: True if the sequence is safe, False otherwise.
    """
    # Calculate the unique differences between each pair of consecutive elements in the list `row`.
    # Prodices a set so duplicates are removed.
    # It computes the difference between each pair of consecutive elements in `row`.
    inc = {row[i + 1] - row[i] for i in range(len(row) - 1)}

    # Check 1: Does the set of unique increments (inc) contain *only* values from {1, 2, 3}?
    # The '<=' operator checks if 'inc' is a subset of the target set.
    is_positive_safe = inc <= {1, 2, 3}

    # Check 2: Does the set of unique increments (inc) contain *only* values from {-1, -2, -3}?
    is_negative_safe = inc <= {-1, -2, -3}

    # Return True if either condition is met.
    return is_positive_safe or is_negative_safe


# --- Data Input and Preprocessing ---

# This is a single, concise list comprehension to read and format the input file.
data = [
    # Converts strings to integers for one row.
    # 1. x.strip().split(" "): Splits the row string (x) by spaces.
    # 2. if y: Filters out empty strings, which prevents 'ValueError: invalid literal for int()'
    #    when handling multiple spaces between numbers or spaces at the ends of the line.
    # 3. int(y): Converts the remaining strings to integers.
    [int(y) for y in x.strip().split(" ") if y]
    # Reads the file and splits into rows.
    # 1. open(...).read(): Reads the entire file content into a single string.
    # 2. .strip(): Removes leading/trailing whitespace and crucial blank lines from the file content.
    # 3. .split("\n"): Splits the content into a list of individual row strings (x).
    for x in open("../inputs/input-02.txt").read().strip().split("\n")
]

# --- Part 1: Number of Safe Reports - Count of Naturally Safe Rows ---

# sum(...): Treats True as 1 and False as 0, counting the number of rows where the condition is True.
safe_count = sum(
    # A Generator Expression that runs the is_safe() function through every row in the processed 'data'.
    is_safe(row)
    for row in data
)
print(f"Naturaly Safe Rows: {safe_count}")


# --- Part 2: Number of Actual Safe Reports - Count of Rows Made Safe by Removing One Element ---

# The goal is to count how many rows can become 'safe' if *any* single element is removed.
safe_count = sum(
    # An Outer Generator Expression that that runs the is_safe() function through every row in the processed 'data'.
    # The result of this inner part is either True (if a safe sub-sequence was found) or False.
    # An Inner List Comprehension: Generates a sequence of boolean results (True/False) for each removal attempt.
    # row[:i] + row[i + 1 :]: This is list slicing that effectively removes the element at index 'i'.
    #                         It creates a new list from the start up to 'i', and then appends the
    #                         rest of the list from 'i + 1' to the end.
    # any(...): This checks if *at least one* of the generated sequences (the ones with one element removed)
    #           is 'safe'. If any is True, the 'any' function returns True.
    any([is_safe(row[:i] + row[i + 1 :]) for i in range(len(row))])
    for row in data
)
print(f"Actual Safe Rows: {safe_count}")
