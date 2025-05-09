#include "splashkit.h"

// Access read_integer and read_string in utilities
#include "utilities.h"

using std::to_string;

/**
 * Genre data captures the list of genre's captured in the software.
 */
enum genre_data
{
    FANTASY = 0,
    SCIENCE_FICTION,
    MYSTERY,
    ROMANCE,
    COOKING,
    NONFICTION
};

// Get the number of genres. Cast the last
// one to an integer and add one as it start from 0
const int GENRE_COUNT = (int)NONFICTION + 1;

/**
 * Convert a genre to a string
 *
 * @param genre the data to convert
 */
string to_string(genre_data genre)
{
    switch (genre)
    {
    case FANTASY:
        return "fantasy";
    case SCIENCE_FICTION:
        return "science fiction";
    case MYSTERY:
        return "mystery";
    case ROMANCE:
        return "romance";
    case COOKING:
        return "cooking";
    case NONFICTION:
        return "nonfiction";
    default:
        // While we expect the above, it can be broken
        // when we cast from an integer...
        return "unknown";
    }
}

/**
 * Read in a genre value from the user.
 *
 * @param prompt a message to prompt the user with
 * @return a genre
 */
genre_data read_genre(string prompt)
{
    // Create a book variable, and initialise it
    genre_data result;
    int raw;

    // Loop through the integers that relate to the genre
    // Output as the list of options to choose from
    // Starting at 1
    for (raw = 0; raw < GENRE_COUNT; raw++)
    {
        // convert the raw integer value to a genre_data
        // and then conver that to a string
        write_line(to_string(raw + 1) + ": " + to_string((genre_data)raw));
    }

    // Read in the integer value of the genre
    // - user enters 1 to GENRE_COUNT. So -1 needed.
    // - provide a range if you can
    raw = read_integer(prompt) - 1;

    // Convert the raw integer data to a genre
    return (genre_data)raw;
}

int main()
{
    // Create a test_genre
    genre_data test_genre;

    // Assign a value
    test_genre = MYSTERY;

    // Output its name and integer value
    write_line(to_string(test_genre) + " has value " + to_string((int)test_genre));

    // Assign an invalid value - by casting an integer
    test_genre = (genre_data)157;

    // Output its name and integer value
    write_line(to_string(test_genre) + " has value " + to_string((int)test_genre));

    // Read in genre data from the user
    test_genre = read_genre("Enter a genre: ");

    // Output its name and integer value
    write_line(to_string(test_genre) + " has value " + to_string((int)test_genre));

    return 0;
}