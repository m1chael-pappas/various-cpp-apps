#include "splashkit.h"
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include <stdexcept>

int main()
{
    int count = 0;
    double total = 0.0;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
    double average;
    bool continue_input = true;
    std::string answer;
    std::string user_input;
    bool valid_input;

    write_line("Welcome to the simple stats calculator:");

    while (continue_input)
    {
        write("Enter value: ");
        user_input = read_line();

        try
        {
            double value = std::stod(user_input);
            valid_input = true;

            count++;
            total += value;

            if (value < min)
            {
                min = value;
            }

            if (value > max)
            {
                max = value;
            }

            average = total / count;

            write_line("Count: " + std::to_string(count));
            write_line("Total: " + std::to_string(total));
            write_line("Min: " + std::to_string(min));
            write_line("Max: " + std::to_string(max));
            write_line("Average: " + std::to_string(average));

            write("Add another value: [y/n] ");
            answer = read_line();

            std::transform(answer.begin(), answer.end(), answer.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            if (answer == "y")
            {
                continue_input = true;
            }
            else if (answer == "n")
            {
                continue_input = false;
            }
            else
            {
                write_line("Invalid choice. Assuming 'n'.");
                continue_input = false;
            }
        }
        catch (const std::invalid_argument &)
        {
            write_line("Invalid input. Please enter a valid number.");
        }
    }

    write_line("I hope you got the information you are after!");

    return 0;
}