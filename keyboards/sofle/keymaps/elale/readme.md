# Sofle – ElAle's Keymap (ES Programmer Keymap with Game Mode)

A pragmatic, OS-aware Sofle layout optimized for programming on a Spanish (ES) layout and seamless switching between Windows, macOS, and Linux (GNOME). It also includes a dedicated **GAME** base layer with no tap-hold quirks and Space/Enter swapped for comfortable gaming.

## Highlights

- **Two base layers**: `QWERTY` (primary) and `GAME` (alt). Persistent switching via the Adjust layer.
- **Smart thumbs (typing)**: tap = `Enter`/`Space`, hold = `LOWER`/`RAISE`. In `GAME`, thumbs are plain: Space/Enter + MO(LOWER/RAISE).
- **Reused base thumb keys**: on `QWERTY`, the extra MO keys became **OS-aware Desktop Left / Desktop Right** for instant workspace switching.
- **Space-cadet pairs (ES-aware)**: Shifts tap to `(` and `)`. Alt/Ctrl pairs tap to `[ { } ]` (hold gives the real modifier).
- **OS toggle**: Windows ⇄ macOS ⇄ GNOME affects screenshots, desktop switching, emoji panel, etc. Stored in EEPROM.
- **LOWER symbol cluster**: `<`, `>`, `{}`, `[]`, `\`, plus a `<>` macro that leaves the caret inside.
- **RAISE navigation & utilities**: prev/next tab, prev/next word, lock, screenshot, emoji, arrows and paging.
- **ADJUST**: base switching (QWERTY/GAME), OS toggle, Ctrl⇄GUI swap, Caps Word, media & brightness.
- **OLEDs**: compact HUD (left) and contextual help (right). Auto-dim/off with split sync.

## Layers (Bird's-eye)

```
QWERTY (base)
º 1 2 3 4 5      6 7 8 9 0 '
Esc ...                            ... Bsp
Tab ...                            ... Ñ ´
Sft( ... Mute — ... - )Sft
LGui Alt[ Ctl{ Desk< Enter   Space Desk> }Ctl ]Alt RGui

GAME (base alt)
... same as QWERTY but: plain Shifts; Space/Enter swapped (left/right);
thumbs are MO(LOWER)/Space and Enter/MO(RAISE);
a QWERTY key is placed on the right home row to return.

LOWER
F-keys, 1–9, and symbol cluster: < > { } [ ] \  plus <> macro.

RAISE
Insert/Print/Menu, word nav, arrows, paging, screenshot/lock/emoji, tab nav.

ADJUST
QWERTY/GAME base toggle (persistent), OS toggle, Ctrl⇄GUI swap, Caps Word, media.
```

## OS-aware Shortcuts

Depending on the OS mode (`KC_OS_TOG` on Adjust), these keys adapt:

- **Desktop switching**: Ctrl+←/→ (macOS), Win+Ctrl+←/→ (Windows), Super+PgUp/PgDn (GNOME).
- **Screenshot**: Cmd+Shift+4 (macOS), Win+Shift+S (Windows), PrintScreen overlay (GNOME).
- **Emoji**: Ctrl+Cmd+Space (macOS), Win+. (Windows), Ctrl+. (GNOME default).

## Build / Flash

```sh
qmk compile -kb sofle -km <your_keymap_name>
qmk flash -kb sofle -km <your_keymap_name>
```

Requirements: `SPACE_CADET_ENABLE`, `ENCODER_MAP_ENABLE`, `OLED_ENABLE`, `CAPS_WORD_ENABLE`, `SPLIT_KEYBOARD`, and `SPLIT_*_ENABLE` are enabled in `rules.mk` / `config.h` as in this repo.

## Tuning

- **Tap-hold**: `TAPPING_TERM=180`, quick taps at `120ms`, permissive-hold for thumb LTs.
- **OLED**: 5s timeout, brightness 120. Split power is master-driven; the slave mirrors ON/OFF.

## Credits

Built on QMK + Sofle. Thanks to the QMK community. This layout is tailored for ES programmers who switch OSes and still want a frictionless gaming mode.