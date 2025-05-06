#include "splashkit.h"
#include <string>

/**
 * @struct bank_account
 * @brief Represents a bank account with name, interest rate and balance
 */
struct bank_account
{
    string name;
    double interest_rate;
    double balance;
};

/**
 * @brief Formats a double value as a currency string with 2 decimal places
 * @param amount The amount to format
 * @return A string representation of the amount with 2 decimal places
 */
string format_currency(double amount)
{
    int dollars = (int)amount;
    int cents = (int)((amount - dollars) * 100 + 0.5);
    return std::to_string(dollars) + "." + (cents < 10 ? "0" : "") + std::to_string(cents);
}

/**
 * @brief Displays the account details to the console
 * @param account The bank account to display
 */
void display_account(const bank_account &account)
{
    write_line("===== ACCOUNT DETAILS =====");
    write_line("Account Name: " + account.name);
    write_line("Interest Rate: " + std::to_string(account.interest_rate) + "%");
    write_line("Balance: $" + format_currency(account.balance));
    write_line("===========================");
}

/**
 * @brief Processes a deposit transaction for an account
 * @param account The bank account to deposit into
 */
void perform_deposit(bank_account &account)
{
    write_line("\n===== DEPOSIT =====");
    write_line("Current Balance: $" + format_currency(account.balance));

    double amount;
    bool valid_input = false;

    while (!valid_input)
    {
        write("Enter amount to deposit (or 0 to cancel): $");
        string input = read_line();

        amount = convert_to_double(input);

        if (amount < 0)
        {
            write_line("Error: Please enter a value greater than or equal to 0");
        }
        else
        {
            valid_input = true;
        }
    }

    if (amount > 0)
    {
        account.balance += amount;
        write_line("Deposit complete. New balance: $" + format_currency(account.balance));
    }
    else
    {
        write_line("Deposit cancelled.");
    }
}

/**
 * @brief Processes a withdrawal transaction for an account
 * @param account The bank account to withdraw from
 */
void perform_withdraw(bank_account &account)
{
    write_line("\n===== WITHDRAW =====");
    write_line("Current Balance: $" + format_currency(account.balance));

    double amount;
    bool valid_input = false;

    while (!valid_input)
    {
        write("Enter amount to withdraw (or 0 to cancel): $");
        string input = read_line();

        amount = convert_to_double(input);

        if (amount < 0)
        {
            write_line("Error: Please enter a value greater than or equal to 0");
        }
        else if (amount > account.balance)
        {
            write_line("Error: Insufficient funds");
        }
        else
        {
            valid_input = true;
        }
    }

    if (amount > 0)
    {
        account.balance -= amount;
        write_line("Withdrawal complete. New balance: $" + format_currency(account.balance));
    }
    else
    {
        write_line("Withdrawal cancelled.");
    }
}

/**
 * @brief Calculates and adds interest to the account based on number of days
 * @param account The bank account to add interest to
 */
void add_interest(bank_account &account)
{
    write_line("\n===== ADD INTEREST =====");
    write_line("Current Balance: $" + format_currency(account.balance));

    int days;
    bool valid_input = false;

    while (!valid_input)
    {
        write("Enter number of days (or 0 to cancel): ");
        string input = read_line();

        if (is_number(input))
        {
            days = convert_to_integer(input);
            if (days >= 0)
            {
                valid_input = true;
            }
            else
            {
                write_line("Error: Please enter a valid number of days");
            }
        }
        else
        {
            write_line("Error: Please enter a valid number");
        }
    }

    if (days > 0)
    {

        double daily_rate = account.interest_rate / 365.0;
        double period_rate = (daily_rate * days) / 100.0;
        double interest_amount = account.balance * period_rate;

        account.balance += interest_amount;

        write_line("Interest added:");
        write_line("Rate is " + std::to_string(account.interest_rate) + "% PA = " + std::to_string(daily_rate * days) + "% for the period");
        write_line("Interest Amount: $" + format_currency(interest_amount));
        write_line("New Balance: $" + format_currency(account.balance));
    }
    else
    {
        write_line("Interest calculation cancelled.");
    }
}

/**
 * @brief Creates a new bank account with user input for name, interest rate, and initial balance
 * @return A newly created bank account
 */
bank_account create_account()
{
    bank_account result;

    write_line("\n===== NEW ACCOUNT SETUP =====");

    write("Enter account name: ");
    result.name = read_line();

    bool valid_rate = false;
    while (!valid_rate)
    {
        write("Enter interest rate (%): ");
        string rate_input = read_line();

        if (is_number(rate_input))
        {
            result.interest_rate = convert_to_double(rate_input);
            valid_rate = true;
        }
        else
        {
            write_line("Error: Please enter a valid number");
        }
    }

    bool valid_balance = false;
    while (!valid_balance)
    {
        write("Enter initial balance ($): ");
        string balance_input = read_line();

        if (is_number(balance_input))
        {
            result.balance = convert_to_double(balance_input);
            if (result.balance < 0)
            {
                write_line("Error: Balance cannot be negative.");
            }
            else
            {
                valid_balance = true;
            }
        }
        else
        {
            write_line("Error: Please enter a valid number");
        }
    }

    write_line("Account created successfully!");
    return result;
}

/**
 * @brief Displays the main menu options to the user
 */
void display_main_menu()
{
    write_line("\n===== BANK ACCOUNT MANAGEMENT =====");
    write_line("1: View Account Details");
    write_line("2: Deposit");
    write_line("3: Withdraw");
    write_line("4: Add Interest");
    write_line("5: Quit");
    write("Select an option (1-5): ");
}

/**
 * @brief Main program function
 * @return Program exit code (0 for normal exit)
 */
int main()
{

    bank_account user_account = create_account();

    bool quit = false;

    while (!quit)
    {

        display_main_menu();

        string choice = read_line();

        if (choice == "1")
        {
            display_account(user_account);
        }
        else if (choice == "2")
        {
            perform_deposit(user_account);
        }
        else if (choice == "3")
        {
            perform_withdraw(user_account);
        }
        else if (choice == "4")
        {
            add_interest(user_account);
        }
        else if (choice == "5")
        {
            write_line("Thank you for using the Bank Account Management System. Goodbye!");
            quit = true;
        }
        else
        {
            write_line("Invalid option. Please try again.");
        }
    }

    return 0;
}