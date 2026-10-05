#include <bn_core.h>
#include <bn_display.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_random.h>
#include <bn_rect.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_size.h>
#include <bn_string.h>
#include <bn_bg_palettes.h>
#include <bn_color.h>
#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"
#include "common_fixed_8x16_font.h"

// Pixels / Frame player moves at
// Change 1 - Changed the speed from 1 to 2
static constexpr bn::fixed SPEED = 2.0;

// Width and height of the the player and treasure bounding boxes
static constexpr bn::size PLAYER_SIZE = {8, 8};
static constexpr bn::size TREASURE_SIZE = {8, 8};

// Full bounds of the screen
static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y = bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X = bn::display::width() / 2;

// Number of characters required to show the longest numer possible in an int (-2147483647)
static constexpr int MAX_SCORE_CHARS = 11;

// Score location
static constexpr int SCORE_X = 70;
static constexpr int SCORE_Y = -70;

// Change 3 - Changing the starting position of the player and the dot
static constexpr int PLAYER_START_X = -25;
static constexpr int PLAYER_START_Y = 25;
static constexpr int TREASURE_START_X = 0;
static constexpr int TREASURE_START_Y = 0;

int main()
{
    bn::core::init();

    // Change 2 - Changed the backdrop color to a light blue color
    bn::bg_palettes::set_transparent_color(bn::color(20, 26, 31));

    bn::random rng = bn::random();

    // Will hold the sprites for the score
    bn::vector<bn::sprite_ptr, MAX_SCORE_CHARS> score_sprites = {};
    bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

    // timer used for speed boost functionality (measured in frames)
    int boostTimer = 0;
    // charges of boost ability
    int boostCount = 3;

    int score = 0;

    bn::sprite_ptr player = bn::sprite_items::square.create_sprite(PLAYER_START_X, PLAYER_START_Y);
    bn::sprite_ptr treasure = bn::sprite_items::dot.create_sprite(TREASURE_START_X, TREASURE_START_Y);

    while (true)
    {

        // Change 4 - When the player hits start, the game restarts
        if (bn::keypad::start_pressed())
        {
            score = 0;
            player.set_position(PLAYER_START_X, PLAYER_START_Y);
            treasure.set_position(TREASURE_START_X, TREASURE_START_Y);
            boostTimer = 0;
            boostCount = 3;
        }

        // Change 5 - The player loops around the screen when they go off the edge
        if (player.x() < MIN_X){
            player.set_x(MAX_X);
        }
        if (player.x() > MAX_X){
            player.set_x(MIN_X);
        }
        if (player.y() < MIN_Y){
            player.set_y(MAX_Y);
        }
        if (player.y() > MAX_Y){
            player.set_y(MIN_Y);
        }

        // Move player with d-pad
        if (boostTimer > 0) { 
            // if there is time remaining on a boost, double movement speed
            if (bn::keypad::left_held())
            {
                player.set_x(player.x() - (SPEED * 2));
            }
            if (bn::keypad::right_held())
            {
                player.set_x(player.x() + (SPEED * 2));
            }
            if (bn::keypad::up_held())
            {
                player.set_y(player.y() - (SPEED * 2));
            }
            if (bn::keypad::down_held())
            {
                player.set_y(player.y() + (SPEED * 2));
            }
            boostTimer--; // subtract time from boost each frame
        } else { // if no boost time remaining: move at normal speed
            if (bn::keypad::left_held())
            {
                player.set_x(player.x() - SPEED);
            }
            if (bn::keypad::right_held())
            {
                player.set_x(player.x() + SPEED);
            }
            if (bn::keypad::up_held())
            {
                player.set_y(player.y() - SPEED);
            }
            if (bn::keypad::down_held())
            {
                player.set_y(player.y() + SPEED);
            }
        }
        // when player presses A and boost is not active, add ~5 seconds to boost timer
        if (bn::keypad::a_pressed() && boostCount > 0 && boostTimer == 0) {      
            boostTimer = 300;
            boostCount--;
        }

        // The bounding boxes of the player and treasure, snapped to integer pixels
        bn::rect player_rect = bn::rect(player.x().round_integer(),
                                        player.y().round_integer(),
                                        PLAYER_SIZE.width(),
                                        PLAYER_SIZE.height());
        bn::rect treasure_rect = bn::rect(treasure.x().round_integer(),
                                          treasure.y().round_integer(),
                                          TREASURE_SIZE.width(),
                                          TREASURE_SIZE.height());

        // If the bounding boxes overlap, set the treasure to a new location an increase score
        if (player_rect.intersects(treasure_rect))
        {
            // Jump to any random point in the screen
            int new_x = rng.get_int(MIN_X, MAX_X);
            int new_y = rng.get_int(MIN_Y, MAX_Y);
            treasure.set_position(new_x, new_y);

            score++;
        }

        // Update score display
        bn::string<MAX_SCORE_CHARS> score_string = bn::to_string<MAX_SCORE_CHARS>(score);
        score_sprites.clear();
        text_generator.generate(SCORE_X, SCORE_Y,
                                score_string,
                                score_sprites);

        // Update RNG seed every frame so we don't get the same sequence of positions every time
        rng.update();

        bn::core::update();
    }
}