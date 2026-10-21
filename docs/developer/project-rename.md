# Project rename

The project name is Touchable.
The previous name was Nitenphone.
This file is the only record of the previous identifiers.
Use the table below to read old reports, logs, and history entries.

## Full rename: 2026-10-20

This rename replaces all previous identifiers.
It changes Bundle IDs, the workspace directory, local state, and code identifiers.

| Item | Previous value | Current value |
| --- | --- | --- |
| Workspace directory | `~/DEV/Nitenphone` | `~/DEV/Touchable` |
| Local state directory | `.nitenphone/` | `.touchable/` |
| Isolated state setting | `NITENPHONE_STATE_DIR` | `TOUCHABLE_STATE_DIR` |
| Silksong Bundle ID | `com.mmmz.NitenphoneProbe` | `com.mmmz.TouchableSilksong` |
| Rhythm Heaven Bundle ID | `com.mmmz.NitenphoneSilksong` | `com.mmmz.TouchableRhythmHeaven` |
| Planned Rhythm Heaven Bundle ID | `com.mmmz.NitenphoneRhythmHeaven` | `com.mmmz.TouchableRhythmHeaven` |
| CPU probe Bundle ID | `com.mmmz.NitenphoneCPUProbe` | `com.mmmz.TouchableCPUProbe` |
| CPU probe target | `NitenphoneProbe` | `TouchableProbe` |
| Runtime macro | `NITENPHONE_SWITCH_RUNTIME` | `TOUCHABLE_SWITCH_RUNTIME` |
| Game interface header | `niten_game.h` | `touchable_game.h` |
| Game interface functions | `niten_game_*` | `touchable_game_*` |
| Generated configuration | `niten_game_config.h`, `NITEN_GAME_*` | `touchable_game_config.h`, `TOUCHABLE_GAME_*` |
| iOS identifiers | `Niten*` | `Touchable*` |
| Fallback font name | `NitenphoneFallback` | `TouchableFallback` |
| Generated Bundle ID prefix | `dev.nitenphone.user` | `dev.touchable.user` |

Product names and phone save paths do not change.
They stay `Silksong` and `RhythmHeaven`, and `Documents/Saves/<product>`.

### Effect on installed apps

iOS identifies an app by its Bundle ID.
The new Bundle IDs install as new apps.
The old apps stay on the device until you remove them.
The saves in the old apps do not move to the new apps.
Back up the saves from the old apps before the first install of a new build.
Restore the backup into the new apps after the install.
Signing needs provisioning profiles for the new Bundle IDs.

On 2026-10-20 the user removed the old apps from the iPhone and the iPad mini.
Before the removal, the full `Documents` folder of each old app was copied to `.touchable/saves/<id>/<device>-before-touchable-rename-20261020-004619/`.
Each copy has a SHA-256 list next to it.
Silksong build 21 and Rhythm Heaven build 34 were then installed on both devices with the new Bundle IDs.
Xcode automatic signing made the profiles. They include both devices and expire on 2026-10-27.
The Silksong build ran the launcher's shader and first-run checks again.
The Rhythm Heaven build used the manual Xcode path.
The new apps were not started after the install.

After the install, each device's backup was copied into the new apps on the same device.
The copy wrote the committed save folder `Documents/Saves/<product>/` and `Documents/Saves/pso_user.tsv`.
The game rebuilds its `.work` copy from the committed folder at start.
A read-back from each device matched the backup SHA-256 values for all 20 files.
The iPhone Silksong save is from 2026-10-19 17:34. Silksong was running on the iPhone during the backup, so progress after that time is not included.
The games were not started after the restore.

### Effect on local files

Reports, logs, and history entries before 2026-10-20 can contain the previous absolute paths.
Read `~/DEV/Nitenphone/.nitenphone/` as `~/DEV/Touchable/.touchable/` in these files.
Documentation and history entries use the current identifiers.
Read a Bundle ID in an old entry with the table above.
Signed apps and IPA files from before this rename keep the previous Bundle IDs.
The Python environment was rebuilt at the new path.
The Silksong first-run shader check runs again on the next build, because `nvn_mtl.m` changed.

## Branding rename: 2026-10-17

This rename changed the project name and the launcher filename only.
The launcher is `Touchable.command` at the repository root.
New private Mac transfer folders and ZIP names use the `Touchable-` prefix.
It kept all compatibility identifiers.
The 2026-10-20 rename replaced them.
