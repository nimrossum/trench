#include "sound.h"

#include <stdlib.h>

#include "array_list.h"
#include "game_state.h"
#include "log.h"

// Default relative path for audio assets; can be overridden with the TRENCH_SOUND_DIR env var.
static const char* default_sound_dir = "./sounds";
static const char* sound_dir_env = "TRENCH_SOUND_DIR";

// Internal representation of a queued sound request.
typedef struct sound_event {
    sound_effect effect;
    int x;
    int y;
} sound_event;

const char* sound_base_directory(void) {
    const char* from_env = getenv(sound_dir_env);
    return from_env ? from_env : default_sound_dir;
}

const char* sound_effect_name(sound_effect effect) {
    switch (effect) {
        case SOUND_TREE_CHOP: return "tree_chop";
        case SOUND_PLAYER_DEATH: return "player_death";
        case SOUND_SHOOT: return "shoot";
        case SOUND_HIT: return "hit";
        case SOUND_POWER_UP: return "power_up";
        case SOUND_WIN: return "win";
        default: return "unknown";
    }
}

const char* sound_effect_filename(sound_effect effect) {
    switch (effect) {
        case SOUND_TREE_CHOP: return "tree_chop.wav";
        case SOUND_PLAYER_DEATH: return "player_death.wav";
        case SOUND_SHOOT: return "shoot.wav";
        case SOUND_HIT: return "hit.wav";
        case SOUND_POWER_UP: return "power-up.wav";
        case SOUND_WIN: return "win.wav";
        default: return "unknown.wav";
    }
}

static void enqueue(sound_effect effect, int x, int y) {
    if (!_gs || !_gs->sound_events) return;
    sound_event* evt = malloc(sizeof(sound_event));
    if (!evt) return;
    evt->effect = effect;
    evt->x = x;
    evt->y = y;
    array_list.add(_gs->sound_events, evt);
}

void sound_emit(sound_effect effect) {
    enqueue(effect, SOUND_POS_UNKNOWN, SOUND_POS_UNKNOWN);
}

void sound_emit_at(sound_effect effect, int x, int y) {
    enqueue(effect, x, y);
}

void sound_flush(void) {
    if (!_gs || !_gs->sound_events || _gs->sound_events->count == 0) return;

    while (_gs->sound_events->count) {
        sound_event* evt = array_list.remove(_gs->sound_events, 0, 0);
        if (!evt) continue;

        const char* name = sound_effect_name(evt->effect);
        const char* file = sound_effect_filename(evt->effect);
        const char* base = sound_base_directory();

        if (evt->x != SOUND_POS_UNKNOWN && evt->y != SOUND_POS_UNKNOWN)
            _log(INFO, "SOUND %s (%i,%i) -> %s/%s", name, evt->x, evt->y, base, file);
        else
            _log(INFO, "SOUND %s -> %s/%s", name, base, file);

        free(evt);
    }
}
