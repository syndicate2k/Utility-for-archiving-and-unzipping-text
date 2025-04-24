# About project
**Text archiving Utility** is a C++ program designed to encode and decode strings using a custom compression algorithm as part of the coursework 1st year of the bachelor's degree in the specialty "Applied Mathematics and Computer Science". 

This utility can read from a text file, compress the content, and then decode it back to its original form. The program also supports manual decoding by providing a predefined dictionary.
<br><br>
# Features

- **String Compression:** Compresses a given string based on a custom dictionary.
- **String Decompression:** Decompresses a string back to its original form.
- **Manual Decoding:** Allows manual decoding using a predefined dictionary and encoded string.
- **File Handling:** Reads input from a text file and writes output to a file.
- **Interactive Menu:** Provides an interactive menu for user operations.
<br><br>
# Functions

- **dictionary_create(string s):** Creates a dictionary from the input string.

- **table_create():** Creates encoding and decoding tables based on the dictionary.

- **compression(string s):** Compresses the input string using the encoding table.

- **decode(string s):** Decodes the input string using the decoding table.

- **code_string_operation():** Performs the string encoding operation.

- **decode_string_operation():** Performs the string decoding operation.

- **decode_selected():** Allows manual decoding using a predefined dictionary.

- **merge():** Merges consecutive identical encoded strings.

- **menu_init():** Initializes the interactive menu.

- **get_medium():** Calculates the median frequency of dictionary entries.

- **get_new_index():** Generates a new index for the encoding table.

- **panic():** Exits the program.
