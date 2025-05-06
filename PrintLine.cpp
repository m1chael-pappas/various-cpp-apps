#include "splashkit.h"

/**
 * Print text to the terminal a number of times, allowing repeated patterns.
 * @param text The text to print
 * @param count The number of times to print the text
 * @param new_line Should a new line be printed after the text?
 */
void print_repeated(string text, int count, bool new_line)
{
    for (int i = 0; i < count; i++)
    {
        write(text);
    }

    if (new_line)
    {
        write_line("\n");
    }
}

/**
 * Print a line of a specified length
 * @param length The length of the line to print
 */
void print_line(int length)
{
    print_repeated("-", length, true);
}

int main()
{
    string input;
    int test_length;

    print_line(20);
    write_line("| Line print test  |");
    print_line(20);

    print_repeated("--+--+", 5, true);
    print_repeated("Hello World\n", 5, false);
    print_repeated("--+--+", 5, true);

    write("Enter a length for a test line: ");
    input = read_line();
    test_length = stoi(input);

    print_line(test_length);
}