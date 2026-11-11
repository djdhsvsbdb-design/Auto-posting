# AutoComment for Geometry Dash Android

AutoComment is a Geode mod targeting Geometry Dash Android 2.2081.

## Settings

- **Level ID** — positive numeric ID of the target level.
- **Comment text** — the text submitted to the level.
- **Posting interval** — 5 minutes to 24 hours.
- **Enable automatic posting** — turns the scheduler on or off.

## Runtime behavior

- The scheduler is attached to the long-lived `GameLevelManager`, not to `MenuLayer`. It is intended to keep ticking in the main menu, editor, and levels while the game process is running.
- Closing Geometry Dash stops the mod and its timer. It does not run as an independent Android background service.
- Level IDs are parsed strictly; trailing characters, non-numeric values, zero, and negative values are rejected.
- Blank comment text is rejected after trimming whitespace.
- The interval is clamped to 5–1440 minutes. Changing the level, comment, or interval restarts the timer.
- Disabling posting resets the elapsed timer but does not discard an in-flight request.

## Request handling and known limitations

The mod calls the game's own `GameLevelManager::uploadComment` method. The mod observes `GameLevelManager::onUploadCommentCompleted`. The callback releases the pending-request lock and logs the response and tag. A response other than `-1` is treated as a successful server response; `-1` is logged as failure. If no callback arrives within 90 seconds, the mod clears the lock and retries after the configured interval. The callback log is the best available signal here, but an on-device check is still needed to confirm that the comment appears on the target level.

The completion callback logs the server response and tag. A response other than `-1` is treated as accepted; `-1` is treated as failure. A successful callback is stronger evidence than a local method call, but check the level in-game to confirm the comment is visible.

## Build and verification status

Target game version: **2.2081 Android**  
Geode metadata version: **v5.11.2**

The source has not yet been confirmed by a successful Android build or an on-device test. Treat this as an unverified development build, not a finished release.

## Build targets

GitHub Actions is configured for Android32 and Android64 builds. A green workflow and an on-device test are still required to verify compatibility.
