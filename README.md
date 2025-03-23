# VIA's QMK Userspace Repository

![android-chrome-192x192](https://user-images.githubusercontent.com/1714072/222621960-ddfb8ee6-a486-4c66-8852-b204ba7c807b.png)

VIA is a powerful, open-source web-based interface for configuring your [QMK](https://qmk.fm)-powered mechanical keyboard. It allows you to customize your keymaps, create macros, and adjust RGB settings (if it has RGB) on the fly, without needing to recompile your keyboard's firmware. This makes keyboard customization easier and more accessible for everyone.

## Getting VIA to support your keyboard

Are you a keyboard maker or a developer interested in adding support for your keyboard? We welcome contributions to the VIA project!

1. The source code of the keyboard **MUST be merged** in [QMK Firmware Repositories](https://github.com/qmk/qmk_firmware) Master branch.
2. Your `keymaps/via` keymap **MUST be merged** in [VIA's QMK Userspace Repository](https://github.com/the-via/qmk_userspace_via) Main branch.
3. Create a definition in JSON format for your keyboard and submit it as a pull request to [VIA's Keyboards Repository](https://github.com/the-via/keyboards) Master branch.

Please follow our [Specification documentation](https://www.caniusevia.com/docs/specification) carefully to ensure your pull request is smoothly reviewed and merged.

## What to add in this repository

1. Add a `vendor/keyboardname/keymaps/via` directory, inside the `keyboards` directory (e.g. `xelus/dawn60/keymaps/via`)
2. Add a `rules.mk` file with `VIA_ENABLE = yes` in it--the `yes` needs to be in lowercase
3. Add a `keymap.c` with full layout and sane default keycodes to it

In case multiple boards make use of the same code snippets, for things like Custom UI or other functions, it is highly encouraged the creation of a `common` folder (e.g. `vendor/common`).

In this folder you will host all the common files and handler files that can then be imported by your keyboards.

This is to reduce code duplication.

## Facing Issues?

If you encounter any issues or bugs while using the [VIA Web Application](https://usevia.app), please report them by opening an issue in the [VIA Web Application Repository](https://github.com/the-via/app/issues). This will help us to track down and resolve problems, and improve the VIA experience for everyone.

Before reporting, please make sure to check if an issue has already been reported. Thank you!
