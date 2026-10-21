# Rhythm Heaven IPA

This folder holds the current Rhythm Heaven package and its checksum records.
The current package is build 43.
It supports iPhone, iPad, a separate in-game language setting, controller haptics, a live border glow, and an optional TV mode.
The touch buttons use Liquid Glass.
Build 43 shows the target platform in the TV mode button icon. The first alert shows `启用功能：延迟校正` or `关闭功能：延迟校正` in place of the restart line.
The current IPA is `RhythmHeaven-build43-tv-mode-ui-20261022.ipa`.
Its SHA-256 is `36bd883fba0e3944c84614b1c6cba20caf5354a792a9a46888d2cfb33c5c15b1`.
The local IPA is unsigned. Sign it with PlayCover or another installation tool before use.
Build 43 reuses the build 34 patched modules, shaders, and RomFS. The reused shader chain is `98bab944ce89`.
Build 43 was built before the merge with `origin/main`. The merged source also contains the upstream build 35 loading-screen controls. Build a new package to include them.
No device test was run.
Builds 38 and 39 were local attempts and are not used.
The build 41 and 42 IPAs were deleted at user request.
The branch build 35 fixes the Simplified Chinese per-app language mapping.
Build 29 fixes BeatSpell performance and touch.
Build 30 rewrites the border glow to make the light more natural. Build 31 makes the letterbox border follow the picture faster. Build 32 makes the transition between the picture edge and the letterbox border smoother. Build 33 declares Game Mode and stays at 60 Hz. In build 34, a pressed glass button shows only the system effect. The upstream build 35 hides the touch controls on loading screens.
The upstream build 35 IPA is `RhythmHeaven-build35-loading-controls-20261020.ipa`.
Its SHA-256 is `a27ffc633e98f11fd7e68cec3deaca6452c15a1815812cef82452a9ade16f551`.
Build 35 is installed on the iPhone and the iPad mini (2026-10-20).
The build 34 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build35/`.
Controller haptics are verified only on the iPad mini with a DualSense.
The build 33 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build34/`.
The build 32 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build33/`.
The build 31 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build32/`.
The build 30 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build31/`.
The build 29 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build30/`.
The build 28 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build29/`.
The build 27 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build28/`.
The build 26 IPA, JSON, and SHA-256 moved to `.touchable/archive/ipa/rhythm-heaven/before-build27/`.
The build 19 and build 20 IPA, JSON, and SHA-256 stay in `.touchable/archive/ipa/rhythm-heaven/before-build26/`.
The language setting is at "Settings > Apps > Rhythm Heaven > Language".
Close the game on the device. Then install with "Install existing IPA" in the top-level launcher.
The launcher build actions currently refuse Rhythm Heaven. For the reason, see the known limits in `internal/games/rhythm-heaven/README.md`.
