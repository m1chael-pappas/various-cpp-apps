#include "splashkit.h"
#include <string>
#include <stdexcept>
#include <cctype>
#include <algorithm>

/**
 * @brief Checks if a string contains only digits
 *
 * @param s The string to check
 * @return bool True if the string contains only digits, false otherwise
 */
bool is_whole_number(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

/**
 * @brief Reads a string input from the user
 *
 * @param prompt The message to display before reading input
 * @return std::string The string entered by the user
 */
std::string read_string(const std::string &prompt)
{
    write(prompt);
    return read_line();
}

/**
 * @brief Reads and validates an integer input from the user
 *
 * @param prompt The message to display before reading input
 * @return int The validated integer entered by the user
 */
int read_integer(const std::string &prompt)
{
    std::string input;
    bool valid_input = false;
    int number = 0;

    while (!valid_input)
    {
        write(prompt);
        input = read_line();

        if (is_whole_number(input))
        {
            try
            {
                number = std::stoi(input);
                valid_input = true;
            }
            catch (const std::out_of_range &)
            {
                write_line("Number out of range. Please enter a smaller number");
            }
        }
        else
        {
            write_line("Please enter a whole number");
        }
    }

    return number;
}

/**
 * @brief Reads and validates a yes/no input from the user
 *
 * @param prompt The message to display before reading input
 * @return bool True if user entered 'y' or 'Y', false if 'n' or 'N'
 */
bool read_yes_no(const std::string &prompt)
{
    std::string input;

    while (true)
    {
        write(prompt);
        input = read_line();

        if (input == "y" || input == "Y")
        {
            return true;
        }
        else if (input == "n" || input == "N")
        {
            return false;
        }
        else
        {
            write_line("Please enter y or n");
        }
    }
}

/**
 * @brief Calculates the total score from goals and behinds
 *
 * @param goals The number of goals scored (worth 6 points each)
 * @param behinds The number of behinds scored (worth 1 point each)
 * @return int The total score calculated as (goals * 6) + behinds
 */
int calculate_score(int goals, int behinds)
{
    return (goals * 6) + behinds;
}

/**
 * @brief Determines which team is winning or if it's a draw
 *
 * @param team1_name Name of the first team
 * @param team1_score Score of the first team
 * @param team2_name Name of the second team
 * @param team2_score Score of the second team
 * @return std::string A message indicating which team is winning or if it's a draw
 */
std::string determine_winner(const std::string &team1_name, int team1_score,
                             const std::string &team2_name, int team2_score)
{
    if (team1_score > team2_score)
    {
        return "The " + team1_name + " are winning";
    }
    else if (team2_score > team1_score)
    {
        return "The " + team2_name + " are winning";
    }
    else
    {
        return "It is currently a draw!";
    }
}

/**
 * @brief Outputs the details of the current game state to the console
 *
 * @param team1_name Name of the first team
 * @param team1_goals Number of goals scored by the first team
 * @param team1_behinds Number of behinds scored by the first team
 * @param team2_name Name of the second team
 * @param team2_goals Number of goals scored by the second team
 * @param team2_behinds Number of behinds scored by the second team
 */
void output_details(const std::string &team1_name, int team1_goals, int team1_behinds,
                    const std::string &team2_name, int team2_goals, int team2_behinds)
{
    int team1_score = calculate_score(team1_goals, team1_behinds);
    int team2_score = calculate_score(team2_goals, team2_behinds);

    write_line("Calculating details...");
    write_line(determine_winner(team1_name, team1_score, team2_name, team2_score));
    write_line(team1_name + ": " + std::to_string(team1_goals) + ", " + std::to_string(team1_behinds) +
               ", " + std::to_string(team1_score));
    write_line(team2_name + ": " + std::to_string(team2_goals) + ", " + std::to_string(team2_behinds) +
               ", " + std::to_string(team2_score));
}

/**
 * @brief Draws the current game state to the SplashKit window
 *
 * @param team1_name Name of the first team
 * @param team1_goals Number of goals scored by the first team
 * @param team1_behinds Number of behinds scored by the first team
 * @param team2_name Name of the second team
 * @param team2_goals Number of goals scored by the second team
 * @param team2_behinds Number of behinds scored by the second team
 */
void draw_scoreboard(const std::string &team1_name, int team1_goals, int team1_behinds,
                     const std::string &team2_name, int team2_goals, int team2_behinds)
{
    int team1_score = calculate_score(team1_goals, team1_behinds);
    int team2_score = calculate_score(team2_goals, team2_behinds);

    // Clear the window
    clear_screen(COLOR_WHITE);

    // Draw scoreboard title
    draw_text("AFL SCOREBOARD", COLOR_BLACK, "Arial", 24, 320, 30, option_to_screen());

    // Draw team names
    draw_text(team1_name, COLOR_BLACK, "Arial", 20, 200, 100, option_to_screen());
    draw_text(team2_name, COLOR_BLACK, "Arial", 20, 520, 100, option_to_screen());

    // Draw VS
    draw_text("VS", COLOR_BLACK, "Arial", 20, 360, 100, option_to_screen());

    // Draw goals and behinds
    std::string team1_stats = std::to_string(team1_goals) + "." + std::to_string(team1_behinds);
    std::string team2_stats = std::to_string(team2_goals) + "." + std::to_string(team2_behinds);

    draw_text(team1_stats, COLOR_BLACK, "Arial", 40, 200, 150, option_to_screen());
    draw_text(team2_stats, COLOR_BLACK, "Arial", 40, 520, 150, option_to_screen());

    // Draw total scores
    draw_text(std::to_string(team1_score), COLOR_BLACK, "Arial", 60, 200, 220, option_to_screen());
    draw_text(std::to_string(team2_score), COLOR_BLACK, "Arial", 60, 520, 220, option_to_screen());

    // Draw winner message
    draw_text(determine_winner(team1_name, team1_score, team2_name, team2_score),
              COLOR_BLUE, "Arial", 24, 320, 320, option_to_screen());

    // Draw menu instructions
    draw_text("Use the console window to interact with the program",
              COLOR_BLACK, "Arial", 16, 320, 380, option_to_screen());

    // Refresh the window
    refresh_screen(60);
}

/**
 * @brief Prints the menu options for the user
 *
 * @param team1_name Name of the first team
 * @param team2_name Name of the second team
 */
void print_menu(const std::string &team1_name, const std::string &team2_name)
{
    write_line("Menu:");
    write_line("1: Update " + team1_name + " goals");
    write_line("2: Update " + team1_name + " behinds");
    write_line("3: Update " + team2_name + " goals");
    write_line("4: Update " + team2_name + " behinds");
    write_line("5: Print details");
    write_line("6: Quit");
}

/**
 * @brief Main program entry point
 *
 * @return int Program exit code
 */
int main()
{
    std::string team1_name, team2_name;
    int team1_goals, team1_behinds, team2_goals, team2_behinds;
    int option;
    bool running = true;

    open_window("AFL Score Calculator", 640, 480);

    write_line("Welcome to the AFL score calculator!");

    // Get team 1 details
    write_line("Enter team 1 details:");
    team1_name = read_string("name: ");
    team1_goals = read_integer("goals: ");
    team1_behinds = read_integer("behinds: ");

    // Get team 2 details
    write_line("Enter team 2 details:");
    team2_name = read_string("name: ");
    team2_goals = read_integer("goals: ");
    team2_behinds = read_integer("behinds: ");

    // Initial output of details
    output_details(team1_name, team1_goals, team1_behinds, team2_name, team2_goals, team2_behinds);

    // Draw initial scoreboard
    draw_scoreboard(team1_name, team1_goals, team1_behinds, team2_name, team2_goals, team2_behinds);

    while (running)
    {
        print_menu(team1_name, team2_name);

        process_events();

        bool valid_option = false;
        while (!valid_option)
        {
            option = read_integer("Option: ");

            if (option >= 1 && option <= 6)
            {
                valid_option = true;
            }
            else
            {
                write_line("Please enter a number between 1 and 6");
            }
        }

        switch (option)
        {
        case 1:
            team1_goals = read_integer("goals: ");
            break;
        case 2:
            team1_behinds = read_integer("behinds: ");
            break;
        case 3:
            team2_goals = read_integer("goals: ");
            break;
        case 4:
            team2_behinds = read_integer("behinds: ");
            break;
        case 5:
            output_details(team1_name, team1_goals, team1_behinds, team2_name, team2_goals, team2_behinds);
            break;
        case 6:
            if (read_yes_no("Are you sure you want to quit? [Y/n]: "))
            {
                running = false;
                write_line("Bye!");
                delay(1000);
                close_window("AFL Score Calculator");
            }
            break;
        }

        if (running)
        {
            draw_scoreboard(team1_name, team1_goals, team1_behinds, team2_name, team2_goals, team2_behinds);
        }
    }

    return 0;
}