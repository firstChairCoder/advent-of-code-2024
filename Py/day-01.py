# --- STEP 1: Reading and Flattening the Data ---
# Open the file 'input.txt', read its entire content as a single string.
#
# The .split() method then splits the string by ALL whitespace (spaces and newlines),
# resulting in a list of string numbers.
#
# map(int, ...) converts every string in that list to an integer.
#
# The [*...] syntax (list comprehension with the unpacking operator)
# converts the resulting map object into a single, flat list of integers.
data = [*map(int, open("../inputs/input-01.txt").read().split())]

# --- STEP 2: Splitting, Sorting, and Assigning ---

# data[0::2] is a list slice:
#   - Starts at index 0 (the first element).
#   - Steps by 2 (takes every other element: 0, 2, 4, ...).
# This effectively collects all the numbers from the **first column** of the input.
#
# data[1::2] is also a list slice:
#   - Starts at index 1 (the second element).
#   - Steps by 2 (takes every other element: 1, 3, 5, ...).
# This effectively collects all the numbers from the **second column** of the input.
#
# sorted(...) sorts each resulting list numerically.
# The tuple assignment (A, B = ...) assigns the sorted lists to variables A and B.
A, B = sorted(data[0::2]), sorted(data[1::2])

# --- STEP 3: Printing the Results (Two Calculations) ---

# The print function will output the results of two separate calculations,
# separated by a comma (the default print separator).

print(
    # --- Calculation 1: Sum of Absolute Differences (total distance between lists) ---
    #
    # map(lambda a, b: abs(a - b), A, B):
    #   - The lambda function takes one element from A (as 'a') and one from B (as 'b')
    #     at the same index (since 'map' iterates over both lists in parallel).
    #   - It calculates the absolute difference: |a - b|.
    #   - Example: |31594 - 24099|, |46608 - 70524|, |78052 - 93577|
    # sum(...) adds up all these absolute differences.
    sum(map(lambda a, b: abs(a - b), A, B)),

    # --- Calculation 2: Sum of A-values Weighted by their Count in B (similarity score) ---
    #
    # map(lambda a: a * B.count(a), A):
    #   - The lambda function takes one element from A (as 'a').
    #   - B.count(a) counts how many times 'a' (the value from A) appears in list B.
    #   - It calculates: a * (count of 'a' in B).
    # sum(...) adds up all these weighted values.
    sum(map(lambda a: a * B.count(a), A)),
)
