#ifndef SOUND_H
#define SOUND_H

// Identifiers for sound effects triggered by gameplay events.
typedef enum {
    SOUND_NONE = 0,
    SOUND_TREE_CHOP,
    SOUND_PLAYER_DEATH,
    SOUND_SHOOT,
    SOUND_HIT,
    SOUND_POWER_UP,
    SOUND_WIN,
} sound_effect;

// Value used when no board position is associated with the sound event.
#define SOUND_POS_UNKNOWN (-1)

// Queue a sound effect without any positional context.
void sound_emit(sound_effect effect);

// Queue a sound effect tagged with board coordinates for future spatial use.
void sound_emit_at(sound_effect effect, int x, int y);

// Drain the queued sound events and publish them to the log for external playback.
void sound_flush(void);

// Helpers for consumers that want to locate audio assets.
const char* sound_effect_name(sound_effect effect);
const char* sound_effect_filename(sound_effect effect);
const char* sound_base_directory(void);

#endif
