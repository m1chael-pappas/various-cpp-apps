#include "splashkit.h"

struct player_data
{
    double x;
    double y;
};

const double PLAYER_SPEED = 3;
const double PLAYER_RADIUS = 10;

void draw_player(const player_data &player)
{
    fill_circle(color_blue(), player.x, player.y, PLAYER_RADIUS);
}

void update_player(player_data &player)
{
    if (key_down(LEFT_KEY))
    {
        player.x -= PLAYER_SPEED;
    }
    if (key_down(RIGHT_KEY))
    {
        player.x += PLAYER_SPEED;
    }
}

int main()
{
    player_data player1 = {400, 300};

    open_window("Pass by reference test", 800, 600);

    while (!quit_requested())
    {
        process_events();
        update_player(player1);

        clear_screen(color_white());
        draw_player(player1);
        refresh_screen(60);
    }
    return 0;
}