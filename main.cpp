
#ifdef SIMPLE_KEYBOARD_EXAMPLE
#include "simple-keyboard-example/SimpleKeyboardExample.h"
#elif FLAPPY_BIRD
#include "flappy-bird/FlappyBird.h"
#else
#define NONE
#endif

int main(int argc, char *argv[]){
    {
#ifdef SIMPLE_KEYBOARD_EXAMPLE
        SimpleKeyboardExample game(16, 3);
#elif FLAPPY_BIRD
        FlappyBird game(16 * 10, 9 * 3);
#else
#endif
#ifndef NONE
        game.start();
#endif
    }
    return 0;
}
