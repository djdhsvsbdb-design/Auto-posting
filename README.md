# AutoComment for Geometry Dash (Android)

An Android Geode mod project.

## Planned settings
- Level ID
- Comment text
- Repeat interval selector (minimum 5 minutes)
- Start / stop controls
- Persistent settings
- Visible countdown and last-action status

## Safe posting flow
The mod can prepare the comment and schedule reminders, but each actual publication requires an explicit in-game confirmation. It will not silently publish repeated comments in the background.

## Target
- Geometry Dash Android: `2.2081`
- Geode: `v5.11.2` (target metadata; SDK build still needs verification)
- Android architecture/build: to be selected for the user's device

## Development status
Initial Geode project scaffold committed (`mod.json`, `CMakeLists.txt`, `src/main.cpp`). This is **not a finished mod** and no `.geode` package has been built or tested yet. The source currently logs when loaded; the settings-driven reminder UI is the next implementation step.

## Posting behavior
The planned interval has a 5-minute minimum. The mod may prepare reminders, but each comment publication must be explicitly confirmed in-game; it will not silently post repeated comments.
