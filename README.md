# AutoComment for Geometry Dash Android

AutoComment is a Geode mod targeting Geometry Dash Android 2.2081.

## Settings

- **Level ID** — positive numeric ID of the target level.
- **Comment text** — the text submitted to the level.
- **Posting interval** — 5 minutes to 24 hours.
- **Enable automatic posting** — turns the scheduler on or off.

## Implementation

- The timer is scheduled on the long-lived `GameLevelManager` singleton rather than `MenuLayer`, so it is not tied to the main-menu screen.
- Level IDs are parsed strictly; trailing characters, non-numeric values, zero, and negative values are rejected.
- Blank comment text is rejected after trimming whitespace.
- The interval is clamped to the supported range (5–1440 minutes) even if a stored setting is invalid.
- The timer resets when posting is disabled or the configured level/comment changes.
- A request lock prevents immediate duplicate calls while a request is pending. If no completion is observed within 90 seconds, the lock expires and the next interval can retry.

## Build and verification status

Target game version: **2.2081 Android**  
Geode metadata version: **v5.11.2**

The source and workflow have not yet been confirmed by a successful Android build or an on-device test. The request lock uses a timeout because the current implementation does not yet consume the game's upload-completion callback; a timed-out request may have succeeded remotely, so a later retry could create a duplicate comment. Treat this as an unverified development build, not a finished release.

## Build targets

GitHub Actions is configured to build Android32 and Android64 variants and combine their packages into one artifact.
