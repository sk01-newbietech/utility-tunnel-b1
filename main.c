#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

#include <psxapi.h>
#include <psxetc.h>
#include <psxgpu.h>
#include <psxpad.h>

#define SCREEN_W 320
#define SCREEN_H 240

static DISPENV disp;
static DRAWENV draw;
static uint8_t padbuff[2][34];

static int scene = 0;
static uint16_t old_buttons = 0xffff;

static int pressed(uint16_t buttons, uint16_t button) {
    return (!(buttons & button)) && (old_buttons & button);
}

static void init_game(void) {
    ResetGraph(0);

    SetDefDispEnv(&disp, 0, 0, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw, 0, 0, SCREEN_W, SCREEN_H);

    setRGB0(&draw, 8, 8, 10);
    draw.isbg = 1;

    PutDispEnv(&disp);
    PutDrawEnv(&draw);

    FntLoad(960, 0);
    FntOpen(8, 12, 304, 216, 2, 512);

    InitPAD(padbuff[0], 34, padbuff[1], 34);
    StartPAD();
    ChangeClearPAD(1);

    SetDispMask(1);
}

static void draw_scene(void) {
    PutDrawEnv(&draw);

    if (scene == 0) {
        FntPrint(-1,
            "UTILITY TUNNEL B1\n\n"
            "A FIRST PERSON PS1 HORROR DEMO\n\n"
            "YOU ARE OUTSIDE A RUSTED DOOR.\n"
            "THE SIGN READS:\n"
            "AUTHORIZED PERSONNEL ONLY.\n\n"
            "PRESS X TO ENTER\n"
            "PRESS START TO RESET\n"
        );
    }

    if (scene == 1) {
        FntPrint(-1,
            "ENTRANCE\n\n"
            "THE RED DOOR CREAKS OPEN.\n"
            "COLD AIR RUSHES FROM THE TUNNEL.\n\n"
            "A RED LIGHT BLINKS ON THE WALL.\n\n"
            "X: INSPECT RED LIGHT\n"
            "RIGHT: STEP INTO TUNNEL\n"
            "CIRCLE: GO BACK\n"
        );
    }

    if (scene == 2) {
        FntPrint(-1,
            "CONTROL PANEL\n\n"
            "THE PANEL IS OLD AND WET.\n"
            "ONE BUTTON STILL HAS POWER.\n\n"
            "TEXT SCRATCHED INTO THE METAL:\n"
            "'DO NOT OPEN B1 AFTER MIDNIGHT'\n\n"
            "X: PRESS BUTTON\n"
            "CIRCLE: BACK\n"
        );
    }

    if (scene == 3) {
        FntPrint(-1,
            "DARK TUNNEL\n\n"
            "YOU STEP FORWARD.\n"
            "THE DOOR SLAMS SHUT BEHIND YOU.\n\n"
            "SOMETHING MOVES IN THE DARK.\n\n"
            "X: CALL OUT\n"
            "LEFT: RETURN TO ENTRANCE\n"
        );
    }

    if (scene == 4) {
        FntPrint(-1,
            "DEMO END\n\n"
            "A VOICE WHISPERS FROM THE TUNNEL:\n\n"
            "'SEAN... YOU SHOULD NOT BE HERE.'\n\n"
            "PRESS START TO RESTART\n"
        );
    }

    FntFlush(-1);
    DrawSync(0);
    VSync(0);
}

int main(int argc, const char **argv) {
    init_game();

    while (1) {
        PADTYPE *pad = (PADTYPE *) padbuff[0];
        uint16_t buttons = pad->btn;

        if (pressed(buttons, PAD_START)) {
            scene = 0;
        }

        if (scene == 0 && pressed(buttons, PAD_CROSS)) {
            scene = 1;
        } else if (scene == 1) {
            if (pressed(buttons, PAD_CROSS)) scene = 2;
            if (pressed(buttons, PAD_RIGHT)) scene = 3;
            if (pressed(buttons, PAD_CIRCLE)) scene = 0;
        } else if (scene == 2) {
            if (pressed(buttons, PAD_CROSS)) scene = 4;
            if (pressed(buttons, PAD_CIRCLE)) scene = 1;
        } else if (scene == 3) {
            if (pressed(buttons, PAD_CROSS)) scene = 4;
            if (pressed(buttons, PAD_LEFT)) scene = 1;
        }

        old_buttons = buttons;
        draw_scene();
    }

    return 0;
}