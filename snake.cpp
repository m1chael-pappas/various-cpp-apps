#include "splashkit.h"
#include <vector>

// Constants for game configuration
const int CELL_SIZE = 20;
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 20;
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;
const int INITIAL_SNAKE_LENGTH = 3;
const int GAME_SPEED = 5; // Controls snake movement speed
// TODO: Add a high score system
// TODO: Add a pause feature
// TODO: Add Onyx or an other snake picture for better graphics.

// Direction enumeration
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Position structure for coordinates
struct Position
{
    double x;
    double y;

    // Check if two positions are equal
    bool operator==(const Position &other) const
    {
        return x == other.x && y == other.y;
    }
};

// Snake structure
struct Snake
{
    vector<Position> segments;
    Direction direction;

    // Check if snake collides with itself
    bool collideWithSelf() const
    {
        if (segments.size() <= 1)
            return false;

        Position head = segments[0];
        for (size_t i = 1; i < segments.size(); i++)
        {
            if (head == segments[i])
            {
                return true;
            }
        }
        return false;
    }

    // Check if snake collides with walls
    bool collideWithWall() const
    {
        Position head = segments[0];
        return head.x < 0 || head.x >= WINDOW_WIDTH ||
               head.y < 0 || head.y >= WINDOW_HEIGHT;
    }

    // Check if snake head collides with given position
    bool collideWith(const Position &pos) const
    {
        return !segments.empty() && segments[0] == pos;
    }
};

// Food structure
struct Food
{
    Position position;
};

// Game state structure
struct GameState
{
    Snake snake;
    Food food;
    int score;
    bool gameOver;
    int speed;
};

// Initialize the snake
void initializeSnake(Snake &snake)
{
    snake.segments.clear();

    // Start in the middle of the screen
    double startX = (GRID_WIDTH / 2) * CELL_SIZE;
    double startY = (GRID_HEIGHT / 2) * CELL_SIZE;

    // Add initial segments (all at the same position initially)
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
    {
        Position pos = {startX - (i * CELL_SIZE), startY};
        snake.segments.push_back(pos);
    }

    // Set initial direction
    snake.direction = RIGHT;
}

// Generate food at a random position not occupied by the snake
void spawnFood(GameState &gameState)
{
    Position newPos;
    bool validPosition;

    do
    {
        validPosition = true;

        // Generate random grid position
        int gridX = rnd(GRID_WIDTH);
        int gridY = rnd(GRID_HEIGHT);

        // Convert to pixel coordinates
        newPos.x = gridX * CELL_SIZE;
        newPos.y = gridY * CELL_SIZE;

        // Check if position conflicts with snake
        for (const Position &segment : gameState.snake.segments)
        {
            if (newPos == segment)
            {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);

    gameState.food.position = newPos;
}

// Initialize the game
void initializeGame(GameState &gameState)
{
    gameState.score = 0;
    gameState.gameOver = false;
    gameState.speed = GAME_SPEED;

    initializeSnake(gameState.snake);
    spawnFood(gameState);
}

// Handle player input
void handleInput(GameState &gameState)
{
    Direction currentDirection = gameState.snake.direction;

    if (key_down(UP_KEY) && currentDirection != DOWN)
    {
        gameState.snake.direction = UP;
    }
    else if (key_down(DOWN_KEY) && currentDirection != UP)
    {
        gameState.snake.direction = DOWN;
    }
    else if (key_down(LEFT_KEY) && currentDirection != RIGHT)
    {
        gameState.snake.direction = LEFT;
    }
    else if (key_down(RIGHT_KEY) && currentDirection != LEFT)
    {
        gameState.snake.direction = RIGHT;
    }
}

// Update the game state
void updateGame(GameState &gameState)
{
    if (gameState.gameOver)
        return;

    Snake &snake = gameState.snake;

    // Calculate new head position based on current direction
    Position newHead = snake.segments[0];

    // TODO: Implement movement based on direction
    switch (snake.direction)
    {
    case UP:
        newHead.y -= CELL_SIZE;
        break;
    case DOWN:
        newHead.y += CELL_SIZE;
        break;
    case LEFT:
        newHead.x -= CELL_SIZE;
        break;
    case RIGHT:
        newHead.x += CELL_SIZE;
        break;
    }
    // Check for collisions with wall
    if (newHead.x < 0 || newHead.x >= WINDOW_WIDTH ||
        newHead.y < 0 || newHead.y >= WINDOW_HEIGHT)
    {
        gameState.gameOver = true;
        return;
    }

    // Add new head to the snake
    snake.segments.insert(snake.segments.begin(), newHead);

    // Check for collisions with self
    if (snake.collideWithSelf())
    {
        gameState.gameOver = true;
        return;
    }

    // Check for food collision
    if (newHead == gameState.food.position)
    {
        // Eat food (snake grows)
        gameState.score++;
        if (gameState.score % 5 == 0)
        {                         // Every 5 points
            gameState.speed += 1; // Increase speed by 1
        }
        spawnFood(gameState);
    }
    else
    {
        // Remove the tail if no food was eaten
        snake.segments.pop_back();
    }
}

// Draw the snake
void drawSnake(const Snake &snake)
{
    if (snake.segments.empty())
        return;

    fill_circle(color_green(), snake.segments[0].x + CELL_SIZE / 2, snake.segments[0].y + CELL_SIZE / 2, CELL_SIZE / 2);
    for (size_t i = 1; i < snake.segments.size(); i++)
    {
        const Position &segment = snake.segments[i];
        fill_rectangle(color_blue(), segment.x, segment.y, CELL_SIZE, CELL_SIZE);
    }
}

// Draw the food
void drawFood(const Food &food)
{
    fill_circle(color_red(), food.position.x + CELL_SIZE / 2, food.position.y + CELL_SIZE / 2, CELL_SIZE / 2);
}

// Draw the score
void drawScore(int score)
{

    draw_text("Score: " + std::to_string(score), color_white(), 10, 10);
}

// Draw game over screen
void drawGameOver(int score)
{
    // TODO: Implement this function
    // Draw a game over message and the final score
    // Provide instructions to restart or quit
    draw_text("Game Over!", color_red(), WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 20);
    draw_text("Final Score: " + std::to_string(score), color_white(), WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2);
    draw_text("Press R to restart or Q to quit", color_white(), WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 20);
}

// Render the game
void renderGame(const GameState &gameState)
{
    clear_screen(COLOR_BLACK);

    drawSnake(gameState.snake);
    drawFood(gameState.food);
    drawScore(gameState.score);

    if (gameState.gameOver)
    {
        drawGameOver(gameState.score);
    }
}

// Main function
int main()
{
    // Create the game window
    open_window("Snake Game", WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize game state
    GameState gameState;
    initializeGame(gameState);

    // Main game loop
    int frameCount = 0;

    while (!quit_requested())
    {
        process_events();
        if (gameState.gameOver)
        {
            if (key_down(R_KEY))
            {
                initializeGame(gameState);
            }
            else if (key_down(Q_KEY))
            {
                break; // Exit the game
            }
        }
        else
        {

            handleInput(gameState);

            // Update game at a controlled rate
            frameCount++;
            if (frameCount >= 60 / gameState.speed)
            {
                updateGame(gameState);
                frameCount = 0;
            }
        }

        renderGame(gameState);
        refresh_screen(60);
    }

    return 0;
}