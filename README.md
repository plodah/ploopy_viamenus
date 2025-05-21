# Plodah's Via Menus for Ploopy

This firmware for Ploopy trackballs & mice enables the use of menus in Via to control mouse settings. It also adds a variety of additional features.

This repo is essentially a [qmk_userspace](https://docs.qmk.fm/newbs_external_userspace) containing supplementary keymaps for [qmk_firmware](https://github.com/qmk/qmk_firmware).

### Please Note
**Testing has been limited!**
I use this firmware on my Adept or Thumb daily, but I'm just one guy and I don't have every model to test with.
There have been problems found that I didn't notice myself, and still could be more.

I'd recommend that you're comfortable returning your device to the original firmware in case you come across any such problems.
Please let me know or PR a fix if you find anything, but I can't commit to fixing any issue and provide no warranty etc. It's free after all!

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
- Dragscroll Straightening; slight left/right scrolling can be ignored while scrolling up/down. 
  Also Vertical scrolling ignored while scrolling Left/Right. 
  Sensitivity is adjustable; e.g. "75% sensitivity" will ignore lesser scroll where it's <75% of the greater scroll. 100% sensitivity is fully one-dimensional.

### Drag-volume
While this key is held, pointer movement is stopped and up/down movement adjusts volume.

### Mouse Jiggler 
The pointer will move an imperceptably small amount, in a pseudo-random direction at a pseudo-random interval.
This should be enough to keep your computer awake when you're not using it, but since it's only 1-2 px at a time, it's small enough that it doesn't hinder normal mouse usage.
_I am not liable for any disciplinary action taken by your employer!_
The jiggler will draw a circle to confirm it has been enabled, and a horizontal line to show when it's turned off.
Can be switched on/off by key press, via menu or gesture.

### Sniper mode
Change DPI while the appropriate button is held
Called this "sniper" mode since it's useful for fine movement, but could also be used with a high DPI instead.

### Gestures
Similar to _"Wiggleball"_ 
Wiggle the cursor to enable dragscroll or mouse jiggler (or not, if you prefer)

### Task Switcher
Keys allow you to "alt tab" between open Windows. 
Alt is held for ~1 second after this key released to allow switching between several windows.

### Custom Keycodes
| Key | Description |
| --- | --- |
| DPI Config | Stock Ploopy key to cycle between DPI presets |
| Ploopy Drag Scroll | Stock Ploopy key to turn on the original dragscroll.. it does **not** play nice with the "better" dragscroll keys |
| Better Drag Scroll Momentary | drag scroll mode on while held |
| Better Drag Scroll Toggle | toggles drag scroll mode off/on |
| Taskswitch Next | i.e. Alt + Tab |
| Taskswitch Prev | i.e. Alt + Shift + Tab |

## How do I use it?
1. Download Firmware and JSON from [**Releases**](https://github.com/plodah/ploopy_viamenus/releases)
   - _Dated releases include the json- `latest` release is built automatically and I haven't figured out github actions to include json yet._
3. Flash the firmware; e.g. with QMK toolbox. [Here is QMK's guide](https://docs.qmk.fm/newbs_flashing)
4. Import the json file to Via's 🖌️ design tab. If you don't have 🖌️ at the top of the Via, enable design tab it in settings ⚙️
5. When updating to a new release, please update both firmware and Via JSON

Alternatively, use my version of Via at [via.plodah.uk](https://via.plodah.uk), which would allow you to skip step 4; it already includes these JSON's.  

### Problems and workarounds
#### Known Issues
DPI settings seem to be volatile in many iterations of this firmware
This should be fixed as of 2025-04-28.

#### Tapdances
Via only supports the "keyboard" level custom keycodes. Those that are set up in a module, or at the user level can't be added to the custom menu
I've created "proxy" keycodes at the keyboard level for most, but haven't found a similar workaround for tapdance next.
These will appear as their hex keycode in Via and can only be assigned with the same;

| Key | Code | Notes |
| --- | --- | --- | 
| Dragscroll Tapdance | `0x5700` | Momentary dragscroll, toggles when tapped twice |

## What doesn't it do?
### Stuff that I think is possible, and may implement later
1. ~~"Sniper" mode  -- momentarily reduce DPI by some factor while "sniper" key held.~~  **DONE** 2 May 2025
2. ~~Dragscroll DPI or divisor control.~~ **DONE** 28 Mar 2025
3. Turbo keys/buttons. -- repeatedly send a click at a certain interval.
4. ~~Dragscroll ends on other key press. make this optional.~~ **DONE** 28 Mar 2025
5. Mouse jiggler- could expose different jiggle patterns, multipliers & speeds as via menu.
6. Maybe allow equivalent of ROTATIONAL_TRANSFORM_ANGLE to use trackball sideways
7. Check the dragscroll inversion on adept/madromys. Inversion state seems er.. inverted
8. ~~Dragscroll straightening~~ **DONE** 19 May 2025
9. ~~Drag-Volume~~ Hold a key and move the mouse up/down to adjust volume **DONE** 18 May 2025

### Stuff I would like to achieve, but have no earthly idea how to do so
1. Additional gesture to scroll by drawing a circle with the pointer.
2. Use a text input box for DPI presets rather than a dropdown list.
3. Use a 16-bit value for DPI.
   Currently uses an 8-bit value, multiplied by 10 giving a range of 0-2550 dpi.
   2-byte / 16-bit value would allow 0-65535 directly (no multiplier)
   Via only seems to support 16 bit values on sliders & keycode controls. Uses a dropdown currently.
4. Provide a better interface for tapdances ~~and community module keycodes~~ **fixed the community module kc's**
99. world peace

## I regret everything, take me back
If it all gets too much and you yearn for a simpler time, and wish to return your device to the original firmware or just something simpler.

1. Download the original firmware. Good sources are:-
    - [Via](https://www.caniusevia.com/docs/download_firmware)
    - [Ploopy's Github](https://github.com/ploopyco) - there's a repo per device and firmware location varies.
    - [keyboard.gay](https://keyboard.gay/)🏳️‍🌈 for Vial firmware
2. Flash the firmware; e.g. with QMK toolbox. [Here is QMK's guide](https://docs.qmk.fm/newbs_flashing)
   - _In the releases before 2025-03-29, be aware that the "bootmagic" key may be different to original firmware_
     - _e.g. Thumb: little button (originally scroll wheel) Adept: top left (originally bottom left)_
3. Remove the imported json from Via or clear site data from your browser.
4. If you were encountering a bug that made it impossible to use your trackball, let me know so I might be able to fix it.

Thanks for reading,
Plodah
