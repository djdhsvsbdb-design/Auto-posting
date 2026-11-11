# Changelog

All notable project changes are recorded here.

## Unreleased

- Reset the posting timer when the level, comment, or interval changes.
- Preserve the in-flight request lock when posting is disabled or settings become temporarily invalid.
- Observe the game's comment-upload completion callback and log its response.
- Ignore completion callbacks when AutoComment has no request pending.
- Retry after a 90-second callback timeout instead of permanently pausing.
- Clarified that a local upload-method call is not confirmation of server publication.
- Documented that the mod stops when Geometry Dash is fully closed.

## v0.1.0

- Initial development version for Geometry Dash Android 2.2081.
- Added level ID, comment text, posting interval, and automatic-posting toggle.
- Added basic input validation and a timed upload request.
- Added Android32 and Android64 GitHub Actions build configuration.
- Build and device compatibility remain unverified.
