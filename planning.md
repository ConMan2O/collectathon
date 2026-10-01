A place to write your findings and plans

## Understanding

This code section seems to make the "collectable" jump to a random x and y position on the screen when the player touches it: 
if (player_rect.intersects(treasure_rect)){
            // Jump to any random point in the screen
            int new_x = rng.get_int(MIN_X, MAX_X);
            int new_y = rng.get_int(MIN_Y, MAX_Y);
            treasure.set_position(new_x, new_y);
            score++;
        }

This code sectoin seems to update the score of the player when he interacts with the collectable updating it by 1 point.
        bn::string<MAX_SCORE_CHARS> score_string = bn::to_string<MAX_SCORE_CHARS>(score);
        score_sprites.clear();
        text_generator.generate(SCORE_X, SCORE_Y, score_string, score_sprites);

Kevin - 
bn::string<MAX_SCORE_CHARS> score_string = bn::to_string<MAX_SCORE_CHARS>(score);
        score_sprites.clear();
        text_generator.generate(SCORE_X, SCORE_Y,
                                score_string,
                                score_sprites);   
This piece of code updates the score. I see there is a clear method which I think erases the current score before its updated with the newer score. 

## Planning required changes

## Brainstorming game ideas

## Plan for implementing game

