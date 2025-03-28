# Plodah's Via Menus for Ploopy

This firmware for Ploopy trackballs & mice enables the use of menus in Via to control mouse settings. It also adds a variety of additional features.

This repo is essentially a [qmk_userspace](https://docs.qmk.fm/newbs_external_userspace) containing supplementary keymaps for [qmk_firmware](https://github.com/qmk/qmk_firmware).

### Please Note
**Testing has been limited!**
I use this firmware on my Adept or Thumb daily, but I'm just one guy and I don't have every trackball to test this on, so there could be issues that I've never imagined.

I'd recommend that you're comfortable returning your device to the original firmware if needed.
If you do encounter problems, please let me know or PR a fix but I can't commit to fixing any issue and provide no warranty etc. It's free after all!

![alt text](readme-examplemenu.png)


## What does it do?
### DPI Config
Configure 5 **DPI** presets and switch between them.
Can additionally apply global multiplier to DPI
### Dragscroll
Kinda improved version of dragscroll. 
- Momentary and Toggle keys available.
- Tapdance, which will act as a momentary dragscroll, but when tapped twice will toggle
- Mofify the dragscroll divisor to adjust scroll speed relative to DPI
- Enable/disable dragscroll with caps/num/scroll lock
- If toggled on, dragscroll can optionally be deactivated by another key press
- invert dragscroll direction for x/y independently

### Mouse Jiggler 
The pointer will move an imperceptably small amount, in a pseudo-random direction at a pseudo-random interval.
This should be enough to keep your computer awake when you're not using it, but since it's only 1-2 px at a time, it's small enough that it doesn't hinder normal mouse usage.
_I am not liable for any disciplinary action taken by your employer!_
The jiggler will draw a circle to confirm it has been enabled, and a horizontal line to show when it's turned off.
Can be switched on/off by key press, via menu or gesture.

### Gestures
Similar to _"Wiggleball"_ 
Wiggle the cursor to enable dragscroll or mouse jiggler (or not, if you prefer)
### Task Switcher
Keys allow you to "alt tab" between open Windows. 
Alt is held for ~1 second after this key released to allow switching between several windows.

## How do I use it?
1. Download Firmware and JSON from [**Releases**](https://github.com/plodah/ploopy_viamenus/releases)
   - _Dated releases include the json- `latest` release is built automatically and I haven't figured out github actions to include json yet._
3. Flash the firmware; e.g. with QMK toolbox. [Here is QMK's guide](https://docs.qmk.fm/newbs_flashing)
4. Import the json file to Via's 🖌️ design tab. If you don't have 🖌️ at the top of the Via, enable design tab it in settings ⚙️
5. When updating to a new release, please update both firmware and Via JSON

### Problems and workarounds
#### Tapdance and Community Module keycodes
Several keys that are implemented in this firmware are not in "ranges" that are supported properly by Via.
These will show as their hexidecimal keycode in Via and can only be assigned with the same;
| Key | Code | Notes |
| --- | --- | --- | 
| Dragscroll Tapdance | `0x5700` | Momentary dragscroll, toggles when tapped twice |
| Mouse Jiggler | `0x77c0` | |
| Taskswitch Next | `0x77c1` | i.e. Alt + Tab |
| Taskswitch Prev | `0x77c2` | i.e. Alt + Shift + Tab |

## What doesn't it do?
### Stuff that I think is possible, and may implement later
1. "Sniper" mode  -- momentarily reduce DPI by some factor while "sniper" key held.
2. ~~Dragscroll DPI or divisor control.~~ **DONE**
3. Turbo keys/buttons. -- repeatedly send a click at a certain interval.
4. ~~Dragscroll ends on other key press. make this optional.~~
5. The way I've set things up, Via uses only 1 byte for DPI. 
   The value (0-255) is multiplied by 10 when applying the DPI, meaning the limit is 2550. 
   Should make this a 2 byte value, and directly apply without this calculation.
6. Mouse jiggler- could expose different jiggle patterns, multipliers & speeds as via menu.
7. maybe allow equivalent of ROTATIONAL_TRANSFORM_ANGLE to use trackball sideways
8. check the dragscroll inversion on adept/madromys. Inversion state seems er.. inverted
9. Dragscroll straightening

### Stuff I would like to do, but have no earthly idea how to achieve
1. Scroll by moving the pointer in a circle.
2. Use a text input box for DPI presets rather than a dropdown list.
3. Provide a better interface for tapdances and community module keycodes.
99. world peace

## I regret everything, take me back
If it all gets too much and you yearn for a simpler time, and want to return your device to the original firmware
1. Download the original firmware. Good sources are:-
    - [Via](https://www.caniusevia.com/docs/download_firmware)
    - [Ploopy's Github](https://github.com/ploopyco) - there's a repo per device and firmware location varies.
    - [keyboard.gay](https://keyboard.gay/)🏳️‍🌈 for Vial firmware
2. Flash the firmware; e.g. with QMK toolbox. [Here is QMK's guide](https://docs.qmk.fm/newbs_flashing)
3. Remove the imported json from Via or clear site data from your browser.
