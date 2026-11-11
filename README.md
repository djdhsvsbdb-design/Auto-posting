# AutoComment for Geometry Dash Android

AutoComment is a Geode mod targeting Geometry Dash Android 2.2081.

## Intended behavior

- Enter one numeric level ID.
- Enter the comment text.
- Set the repeat interval (minimum 5 minutes, maximum 24 hours).
- Enable or disable automatic posting in Geode mod settings.
- While the Geometry Dash main menu is open and posting is enabled, the mod calls the game's native level-comment upload function after each interval.

## Current implementation status

The source has been changed from reminder-only behavior to a timed call to the game's native `GameLevelManager::uploadComment` function using `CommentType::Level`.

**This is not yet a verified release.** The current implementation has not been compiled against the Geode v5.11.2 Android SDK, and the upload path has not been tested on a real GD 2.2081 Android installation. The current timer is attached to the main menu layer, so automatic posting only runs while that layer is active. Build and on-device verification are still required.

## Build targets

- Geometry Dash Android: `2.2081`
- Geode metadata target: `v5.11.2`
- GitHub Actions workflow builds Android32 and Android64 variants.

