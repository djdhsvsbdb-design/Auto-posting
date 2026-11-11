# AutoComment for Geometry Dash Android

A Geode mod targeting Geometry Dash Android 2.2081.

## Current functionality

- Adds an AutoComment shortcut to the Geometry Dash main menu.
- Reads the level ID and comment text from the mod settings.
- Supports a reminder interval from 5 to 1440 minutes.
- Shows an in-game reminder containing the level ID and prepared comment.
- The shortcut shows the current configuration and reminder status.
- Reminders are active while the main menu layer is open.

## Setup

1. Install the mod through Geode after a compatible Android build is available.
2. Open Geode's mod settings and select AutoComment.
3. Enter the numeric level ID and the comment text.
4. Choose an interval (minimum 5 minutes).
5. Enable reminders.
6. Keep the Geometry Dash main menu open; the reminder appears after the selected interval.
7. Open the level and submit the comment yourself if you still want to.

## Important behavior

AutoComment does **not** submit comments automatically. Each reminder is only a prompt; publishing remains a deliberate in-game action by the player. This avoids unattended repeated posting.

## Build target and status

- Geometry Dash Android: `2.2081`
- Geode metadata target: `v5.11.2`
- Android ABI/build output: not yet selected or verified.
- The C++ reminder flow and menu shortcut have been added to the repository.
- **No Android .geode package has been built or tested yet.** A successful compile and an on-device test are still required before calling this release ready.

## Build

With the Geode SDK and CLI installed and `GEODE_SDK` configured:

```sh
geode sdk install-binaries -p android64
geode build -p android64
```

For a 32-bit Android target, use `android32` instead. The correct ABI depends on the device and installed Geometry Dash build.
