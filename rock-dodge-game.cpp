#include "splashkit.h"
#include <iostream>
#include <random>
#include <ctime>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SIZE = 30;
const int MAX_ROCKS = 100;
const int PLAYER_SPEED = 5;
const int INITIAL_LIVES = 3;
const string BACKGROUND_MUSIC = "boss_music";
const string ROCK_BITMAP = "rock_image";
const string PLAYER_BITMAP = "player_image";

/**
 * Structure to represent a rock in the game
 *
 * @field position    The position of the rock on screen
 * @field size        The size (diameter) of the rock
 * @field speed       How fast the rock falls down the screen
 */
struct rock_data
{
    point_2d position;
    float size;
    float speed;
};

/**
 * Structure to hold all game data
 *
 * @field rocks           Array of all rocks in the game (maximum MAX_ROCKS)
 * @field num_rocks       Current number of rocks in the array
 * @field player_position Position of the player on screen
 * @field next_rock_time  Time (in ms) when the next rock should appear
 * @field score           Current player score
 * @field lives           Number of lives remaining
 * @field quit            Flag to indicate if the game should exit
 */
struct game_data
{
    rock_data rocks[MAX_ROCKS];
    int num_rocks;
    point_2d player_position;
    unsigned int next_rock_time;
    int score;
    int lives;
    bool quit;
};

/**
 * Checks if two circles are colliding
 *
 * @param a The first circle
 * @param b The second circle
 * @return  True if the circles are colliding, false otherwise
 */
bool check_collision(circle a, circle b)
{
    float dx = center_point(a).x - center_point(b).x;
    float dy = center_point(a).y - center_point(b).y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (circle_radius(a) + circle_radius(b));
}

/**
 * Creates a new rock with random properties
 *
 * @return A new rock with random size, position and speed
 */
rock_data create_rock()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> size_dist(20.0f, 200.0f);
    std::uniform_real_distribution<float> pos_dist(0.0f, static_cast<float>(SCREEN_WIDTH));
    std::uniform_real_distribution<float> speed_dist(15.0f, 25.0f); // Bookmark: set speed here <----

    rock_data rock;
    rock.size = size_dist(gen);
    rock.position = point_at(pos_dist(gen), -rock.size);
    rock.speed = speed_dist(gen);

    return rock;
}

/**
 * Adds a new rock to the game
 *
 * @param game The game data to add the rock to
 */
void add_rock(game_data &game)
{
    if (game.num_rocks < MAX_ROCKS)
    {
        game.rocks[game.num_rocks] = create_rock();
        game.num_rocks++;
        game.score++;
    }
}

/**
 * Removes a rock from the game by its index
 *
 * @param game  The game data to remove the rock from
 * @param index The index of the rock to remove
 */
void remove_rock(game_data &game, int index)
{
    if (index >= 0 && index < game.num_rocks)
    {

        for (int i = index; i < game.num_rocks - 1; i++)
        {
            game.rocks[i] = game.rocks[i + 1];
        }
        game.num_rocks--;
    }
}

/**
 * Initializes the game data to start a new game
 *
 * @param game The game data to initialize
 */
void init_game(game_data &game)
{
    game.player_position = point_at(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - PLAYER_SIZE * 2);
    game.next_rock_time = current_ticks() + 1000;
    game.score = 0;
    game.lives = INITIAL_LIVES;
    game.quit = false;
    game.num_rocks = 0; // Start with no rocks
}

/**
 * Updates the game state for one frame
 *
 * @param game The game data to update
 */
void update_game(game_data &game)
{
    unsigned int current_time = current_ticks();
    if (current_time >= game.next_rock_time && game.num_rocks < MAX_ROCKS)
    {
        add_rock(game);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> time_dist(1000, 2000); // <-- made the game a bit harder
        game.next_rock_time = current_time + time_dist(gen);
    }

    // Update all rocks
    for (int i = 0; i < game.num_rocks; i++)
    {
        // Update rock position
        game.rocks[i].position.y += game.rocks[i].speed;

        // Check if rock is off the screen
        if (game.rocks[i].position.y - game.rocks[i].size > SCREEN_HEIGHT)
        {
            game.score += static_cast<int>(game.rocks[i].size); // Add rock size to score
            remove_rock(game, i);                               // Remove rock
            i--;                                                // Adjust index after removal
            continue;
        }

        // Check collision with player
        circle rock_circle = circle_at(game.rocks[i].position.x, game.rocks[i].position.y, game.rocks[i].size / 2.0f);
        circle player_circle = circle_at(game.player_position.x, game.player_position.y, PLAYER_SIZE / 2.0f);

        if (check_collision(rock_circle, player_circle))
        {
            game.lives--;         // Lose a life
            remove_rock(game, i); // Remove the rock
            i--;                  // Adjust index after removal
        }
    }
}

/**
 * Handles user input for one frame
 *
 * @param game The game data to update based on input
 */
void handle_input(game_data &game)
{
    process_events();

    if (quit_requested())
    {
        game.quit = true;
    }

    // Handle keyboard input for player movement
    if (key_down(LEFT_KEY) || key_down(A_KEY))
    {
        game.player_position.x -= PLAYER_SPEED;
        if (game.player_position.x < PLAYER_SIZE / 2.0f)
        {
            game.player_position.x = PLAYER_SIZE / 2.0f;
        }
    }
    if (key_down(RIGHT_KEY) || key_down(D_KEY))
    {
        game.player_position.x += PLAYER_SPEED;
        if (game.player_position.x > SCREEN_WIDTH - PLAYER_SIZE / 2.0f)
        {
            game.player_position.x = SCREEN_WIDTH - PLAYER_SIZE / 2.0f;
        }
    }
}

/**
 * Draws the current game state to the screen
 *
 * @param game The game data to draw
 */
void draw_game(const game_data &game)
{
    clear_screen(COLOR_BLACK);

    bitmap player_bmp = bitmap_named(PLAYER_BITMAP);

    // Player bitmap
    double player_scale = 0.3;
    int vertical_offset = 90;
    draw_bitmap(PLAYER_BITMAP,
                game.player_position.x - (bitmap_width(player_bmp) * player_scale) / 2,
                game.player_position.y - (bitmap_height(player_bmp) * player_scale) / 2 - vertical_offset,
                option_scale_bmp(player_scale, player_scale));

    // Rocks
    bitmap rock_bmp = bitmap_named(ROCK_BITMAP);
    for (int i = 0; i < game.num_rocks; i++)
    {
        // Scale the rock bitmap to match the rock's size
        double scale = game.rocks[i].size / bitmap_width(rock_bmp);

        draw_bitmap(ROCK_BITMAP,
                    game.rocks[i].position.x - (bitmap_width(rock_bmp) * scale) / 2,
                    game.rocks[i].position.y - (bitmap_height(rock_bmp) * scale) / 2,
                    option_scale_bmp(scale, scale));
    }

    // Draw score and lives text
    std::string score_text = "Score: " + std::to_string(game.score);
    std::string lives_text = "Lives: " + std::to_string(game.lives);

    draw_text(score_text, COLOR_WHITE, 10, 10);
    draw_text(lives_text, COLOR_WHITE, 10, 40);

    refresh_screen(60);
}

/**
 * Main function - entry point of the program
 *
 * @return Program exit status (0 = success)
 */
int main()
{
    open_window("Rock Dodge Game", SCREEN_WIDTH, SCREEN_HEIGHT);

    music boss_music = load_music("boss_music", "boss_battle.mp3");
    play_music(boss_music, 0.7, true);

    load_bitmap("rock_image", "fire_img.png");
    load_bitmap("player_image", "main_char.png");
    game_data game;
    init_game(game);

    const int FPS = 60;
    const int frame_delay = 1000 / FPS;

    unsigned int frame_start;
    int frame_time;

    while (!game.quit && game.lives > 0)
    {
        frame_start = current_ticks();

        update_game(game);

        handle_input(game);

        draw_game(game);

        frame_time = current_ticks() - frame_start;
        if (frame_delay > frame_time)
        {
            delay(frame_delay - frame_time);
        }
    }

    if (game.lives <= 0)
    {
        clear_screen(COLOR_BLACK);

        std::string game_over_text = "Game Over! Final Score: " + std::to_string(game.score);
        draw_text(game_over_text,
                  COLOR_WHITE,
                  SCREEN_WIDTH / 2 - 100,
                  SCREEN_HEIGHT / 2);
        refresh_screen(60);

        delay(3000);
    }

    stop_music();
    free_music(boss_music);
    close_window("Rock Dodge Game");

    return 0;
}