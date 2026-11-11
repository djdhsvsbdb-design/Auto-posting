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

The mod calls the game's own `GameLevelManager::uploadComment` method. The current implementation does not yet observe a definitive upload-completion callback. If a request remains pending for 90 seconds, automatic retries are paused rather than risking a duplicate comment. Toggle automatic posting off and on to explicitly retry.

The log message confirms that the game upload method was called; it does **not** prove that the server accepted or published the comment.

## Build and verification status

Target game version: **2.2081 Android**  
Geode metadata version: **v5.11.2**

The source has not yet been confirmed by a successful Android build or an on-device test. Treat this as an unverified development build, not a finished release.

## Build targets

GitHub Actions is configured for Android32 and Android64 builds. A green workflow and an on-device test are still required to verify compatibility.
