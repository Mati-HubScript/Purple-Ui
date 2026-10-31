# PurpleUI

A dark-purple fullscreen interface prototype for Geometry Dash, built with Geode.

## Targets

- Windows x64
- Android ARM64 (`android64`), suitable for the Samsung Galaxy A36 5G

## Build automatically on GitHub

1. Create a repository named `PurpleUI`.
2. Extract this ZIP on your computer.
3. Upload the **contents** of the extracted `PurpleUI` folder to the root of the repository. Keep the `.github/workflows/build.yml` path intact.
4. Open the repository's **Actions** tab.
5. Select **Build PurpleUI** and click **Run workflow**. A push to `main` or `master` also starts a build.
6. When both jobs finish, open each job's Artifacts section and download its `.geode` package.

The workflow builds Windows x64 and Android ARM64 separately using the official `geode-sdk/build-geode-mod` GitHub Action.

## Install

- Windows: install the Windows `.geode` package using Geode's in-game mod installer or place it in the Geode mods folder.
- Android: install the Android ARM64 `.geode` package through Geode's mod installer, or place it in the Geode mods directory.

## Important

This is an initial interface prototype. The HOME, VISUAL, and ABOUT tabs are present; the Visual page is currently a placeholder. The first GitHub Actions run is the real compatibility check against the selected Geode SDK. If it fails, open the failed job and share the build log so the source or SDK version can be corrected.
