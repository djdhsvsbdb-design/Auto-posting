# About AutoComment

**AutoComment** is a Geode mod for Geometry Dash Android 2.2081.

## Features

- Configurable target level ID and comment text.
- Automatic posting interval from 5 minutes to 24 hours.
- Enable/disable control.
- Strict level-ID validation and blank-comment rejection.
- Scheduler intended to remain active across the menu, editor, and gameplay scenes while Geometry Dash is running.

## Runtime boundary

The mod runs inside the Geometry Dash process. When the game is fully closed, the mod and its scheduler stop. It does not independently search for an account or publish comments outside the game process.

## Reliability notes

The mod delegates comment submission to the game's own upload method. A successful method call is not confirmation of server publication. Since upload completion is not yet observed, a request that remains pending for 90 seconds pauses automatic retries to reduce the risk of duplicate comments. The user can toggle posting off and on to retry.

## Compatibility and status

- Target game: Geometry Dash Android 2.2081
- Geode metadata: v5.11.2
- Android build and on-device behavior: not yet verified

Use the feature responsibly and follow Geometry Dash's community rules.
