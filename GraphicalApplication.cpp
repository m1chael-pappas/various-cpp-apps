/**
 * Interactive Circles Application
 *
 * This program creates a window where users can draw colored circles
 * using mouse clicks and keyboard controls.
 */
#include "splashkit.h"

int main()
{
    // Create and open a window with specified title and dimensions
    window window = open_window("Interactive Circles", 800, 600);

    // Initialize variables
    int current_radius = 50;
    color background_color;
    bool quit_program = false;

    // Set initial background to white
    clear_screen(COLOR_WHITE);

    // Main program loop
    // Continues until window is closed or quit is requested
    while (!window_close_requested(window) && !quit_program)
    {
        // Handle any pending events (keyboard, mouse, etc.)
        process_events();

        // Handle keyboard inputs
        if (key_typed(C_KEY))
        {
            // Change background color to a random color
            background_color = random_rgb_color(255);
            clear_screen(background_color);
        }
        else if (key_typed(S_KEY))
        {
            // Set circle size to small
            current_radius = 10;
        }
        else if (key_typed(M_KEY))
        {
            // Set circle size to medium
            current_radius = 50;
        }
        else if (key_typed(L_KEY))
        {
            // Set circle size to large
            current_radius = 100;
        }
        else if (key_typed(NUM_5_KEY))
        {
            // Generate 100 random circles across the window
            for (int i = 0; i < 100; i++)
            {
                // Generate random position within window bounds
                double x = rnd(window_width(window));
                double y = rnd(window_height(window));

                // Create random color and size
                color random_color = random_rgb_color(255);
                int random_radius = rnd(10, 100);

                // Draw the circle
                fill_circle(random_color, x, y, random_radius);
            }
        }
        else if (key_typed(Q_KEY))
        {
            // Set flag to quit the program
            quit_program = true;
        }

        // Handle mouse input
        if (mouse_clicked(LEFT_BUTTON))
        {
            // Get current mouse position
            double x_pos = mouse_x();
            double y_pos = mouse_y();

            // Generate a random color
            color random_color = random_rgb_color(255);

            // Draw a filled circle at mouse position with current radius setting
            fill_circle(random_color, x_pos, y_pos, current_radius);
        }

        // Update the screen at 60 frames per second
        refresh_screen(60);
    }

    // Clean up resources by closing the window
    close_window(window);
    return 0;
}