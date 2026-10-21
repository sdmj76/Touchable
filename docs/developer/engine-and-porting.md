# Engine and porting pipeline

This note records the original game technology and the host pipeline.
It covers Rhythm Heaven Groove and Silksong.
It summarizes data in the game records and the host source.
It does not replace the game records.

## Short conclusion

- The host runs the original AArch64 code directly. It does not translate instructions.
- The host replaces the platform environment. The main costs are graphics, audio, timing, and missing features.
- The real SDK module runs in the guest. The host replaces selected imports and kernel calls.

## Package facts

| Item | Rhythm Heaven Groove | Silksong |
| --- | --- | --- |
| Input | v0 base package (XCI converted to NSP) | v0 base package and the v655360 update |
| ExeFS modules | `main`, `sdk` | `main`, `subsdk0`, `sdk` |
| Programming language | C++ (libc++, `std::__1` symbols) | C# (Unity), compiled ahead of time with IL2CPP |
| Engine | NintendoSDK 21.4.5 native stack | Unity 6000.0.50f1 |
| Graphics | `nn::gfx` and NVN, BNSH shaders | NVN, Maxwell shaders |
| Audio | AudioRenderer | Unity audio |
| Content size | 7,237 files, about 3.17 GiB | 2,099 files, about 6.65 GB (v0) |

### Rhythm Heaven

- The title ID is `0100d9f01d474000`.
- The package version is 0. The SDK version is 21.4.5.0.
- `main` has 283 dynamic symbol entries: 20 defined and 262 imported.
- `sdk` has 23,140 defined symbols and 8 imported symbols.
- `main` is a thin game layer. Most system code is in the real `sdk` module.
- The main import groups are `nn::audio` (37), `nn::os` (30), `nn::prepo` (27), `nn::hid` (19), `nn::fs` (17), `nn::oe` (14), `nn::fontll` (13), `nn::mem` (10), `nn::util` (9), `nn::swkbd` (6), `nn::vi` (5), `nn::account` (5), `nn::pl` (2), and `nn::diag` (2).
- `nn::prepo` is the PlayReport telemetry interface. It is not a game engine.
- The binaries have no Unity, Unreal, or Godot strings.
- The code uses the NintendoSDK graphics library `NintendoSDK_gfx-21_4_0-Release` and the low-level NVN call `nv::InitializeGraphics`.
- RomFS uses Nintendo formats: `graph/*.tex`, `*.spr`, `mesg/*.msbt`, `*.bfttf.zs` fonts, `*.cwv` audio, `*.wmb` beat markers, and `voicetext/*.vtdb2` speech data.
- The game uses VoiceText TTS data for speech.
- The game has no video files.
- The shader file has 20 control blocks and 35 stages: 33 pixel stages and 2 vertex stages.
- Audio uses AudioRenderer at 48 kHz, 240 samples per frame, 64 voices, 1 submix, I3DL2 reverb, and stereo output.

### Silksong

- The game uses Unity `6000.0.50f1-uum-100966-branch1`.
- The IL2CPP metadata version is 31.
- `main` plus bss is about 132 MB.
- The game has 2,599 shader variants.
- The v655360 update changes only `main`. `sdk`, `subsdk0`, and `rtld` are byte-identical to v0.
- Game logic is C#. IL2CPP compiles it to C++ and then to AArch64 before the app build. No managed virtual machine runs on the phone.

## Porting pipeline

### 1. Extract the package

`internal/tools/prepare_game.py` decrypts the NCA files with `prod.keys`.
It writes ExeFS and RomFS to the local work directory.
The tool reads a base package and an update package as separate inputs.
An update uses patch storage and cumulative content.
The tool checks the module hashes and Build IDs before it continues.

### 2. Load the modules

The host loader plays the role of the Switch `rtld` module:

- It decompresses the NSO segments.
- It maps the modules at arbitrary addresses.
- It resolves cross-module symbols.
- It applies RELR relocations.
- It does not execute the guest `rtld` module.

On iOS, the build embeds the modules as signed code segments.
The loader maps the embedded image.
The app does not use a JIT.

### 3. Rewrite the CPU environment

The code is already AArch64. The rewriter changes only environment-specific instructions:

- `x18` reads and writes go to a thread slot through `tpidrro_el0`.
- `tpidr_el0` and `tpidrro_el0` accesses go to guest TLS slots.
- `svc` calls and `mrs cntpct_el0` reads go to host gates.
- Far branches use veneer words.
- Indirect branches through `x18` use a `BRK` trap and a `SIGTRAP` handler.

The rewrite result for Rhythm Heaven is small:

- `main`: 0 fixups, 0 gates, and 0 indirect traps.
- `sdk`: 58 `svc` gates and 44 `cntpct` gates, 102 gates in total.

The rewrite result for Silksong is larger:

- `main` has 49,778 rewrite points.
- `main` has 14,332 veneer words.
- The three executed modules have 7 indirect traps: 2 `br x18`, 1 `blr x18`, and 4 `mov sp, x18`.
- The full-view modules also have 6 registered fixed-address rewrites.

`structck.py` checks the module structure.
`equiv.py` executes the original and rewritten blocks in Unicorn.
The check uses 100 mutations.
These checks show CPU equivalence only. They do not show game compatibility.

### 4. Supply host services

The real `sdk` module continues to run in the guest.
The host forces a small set of imports to its own code:

- Memory: the host reserves 5 GB of guest address space.
- Heap: the host reports a 3285 MB application pool plus 512 MB.
- Threads and time: the host maps guest threads to pthreads and supplies a guest clock.
- File system: RomFS comes from the app bundle. Saves go to `Documents/Saves/<game>`.
- Input: `nn::hid` reads touch, virtual buttons, or a physical controller. A new press stays visible for at least 25 ms.
- Accounts: the host reports one local user.
- Keyboard: `nn::swkbd` returns Cancel.
- Fonts: the host uses a generated fallback font in place of the console shared fonts.
- Telemetry: `nn::prepo` PlayReport saves return success, but no report leaves the device.
- Network and online services: not implemented.

### 5. Translate NVN to Metal

The graphics layer reimplements the NVN API. It does not run the Switch driver.

- The host takes over `nvnBootstrapLoader`.
- Each NVN call goes to a host thunk.
- The host records commands in the game command memory.
- The host replays the records as Metal commands at submit time.
- Memory pools become zero-copy `MTLBuffer` objects on the same guest memory.
- Textures are deswizzled from Tegra block-linear layout. BC textures are decompressed when necessary.
- Render pass merging and pipeline caches reduce the translation cost.
- The game swapchain is drawn to a `CAMetalLayer` at 60 Hz or 120 Hz.

### 6. Translate shaders offline

The host does not translate shaders at run time.
The build uses this chain:

```
Maxwell SASS (Switch)
  -> SPIR-V (Ryujinx shader library, build time)
  -> MSL (SPIRV-Cross)
  -> metallib (xcrun metal)
```

The runtime finds each shader by the SHA-256 of its control block.
Rhythm Heaven includes the shader code in the key because different code can share one control block.
Rhythm Heaven also flips clip-space Y for its lower-left window origin.

### 7. Audio and display

- Rhythm Heaven uses the AudioRenderer host: PCM16 mix, Hermite resampling, submix, and an I3DL2 reverb approximation.
- The iOS audio session uses a 5 ms buffer for Rhythm Heaven.
- Rhythm Heaven presents 720 lines by default. On an iPhone aspect ratio, the output is about 1564x720.
- Silksong sets `SS_RENDER_HEIGHT=1080`. The output is about 1916x882 on the iPhone and 1644x1080 on the iPad.
- Both games use a native-size drawable. The lower output resolution is upscaled to the screen.

## Losses

| Area | Loss | Details |
| --- | --- | --- |
| CPU | Small | Native AArch64 execution. Rhythm Heaven needs 0 rewrites in `main` and 102 gates in `sdk`. Silksong needs 49,778 rewrite points and 14,332 veneers. |
| GPU | Main cost | Every draw uses command record and replay. Pass merging and pipeline caches add CPU work. Some NVN paths are not implemented. |
| Shaders | Fidelity | Maxwell to SPIR-V to MSL is not bit-exact. Precision, border color, quad primitives, and some clear and copy forms have limits. |
| Resolution | Sharpness | Rhythm Heaven presents 720 lines. Silksong presents at most 1080 lines. Both images are upscaled to the panel. |
| Audio | Fidelity | The I3DL2 reverb is an approximation. Resampling and the audio buffer change the signal path. |
| Timing | Gameplay | Rhythm Heaven game time probably follows frames. A dropped frame slows music and picture together. |
| Features | Missing | Silksong skips cutscene videos. The software keyboard returns Cancel. Shared fonts become a fallback font. Network and telemetry are not available. |
| Verification | Confidence | The CPU checks do not prove a full playthrough. Many device and level cases remain untested. |

Measured results after optimization:

- Rhythm Heaven Remix 8: median frame time fell from 66.92 ms to 16.68 ms. Texture reuploads fell from 15 per frame to 0.
- Rhythm Heaven BeatSpell: busy time per frame fell from about 14.6 ms to about 9.5 ms. Render passes fell from 21 to 13.
- Rhythm Heaven steady play: p50 16.67 ms, p99 19.03 ms. GPU p50 is about 2.1 ms.
- Silksong at 120 Hz: p50 8.33 ms, GPU p50 6.2 to 6.8 ms, and no dropped presents.
- Silksong render passes fell from 16 to 14. Attachment loads fell from 67 MB to 61 MB.
- Silksong memory footprint is about 2.3 GB.

These numbers show the optimized cost. They do not prove parity with the Switch.

## Known limits

### Rhythm Heaven

- The software keyboard returns Cancel.
- The font is a fallback font.
- The reverb is approximate.
- Score digit behavior after the block-height fix is not confirmed.
- Multiplayer is not tested.
- The full game flow is not verified.
- The output stays at 720 lines.
- Build 37 has open Music Room problems.

### Silksong

- Cutscene videos are skipped.
- Shops, yes or no boxes, fast travel, and quest boards do not take touches.
- Two shader control sections are not mapped.
- One program has no vertex shader.
- The full game flow is not verified.
- The output height is at most 1080 lines.
- The app uses about 2.3 GB of memory.

## Update packages

Switch updates match a title ID.
The update file for `Rhythm Heaven Groove Starter Demo` has title ID `0100107027310000`.
Rhythm Heaven Groove has title ID `0100d9f01d474000`.
These are different applications.
The demo update cannot update the full game.

The Rhythm Heaven v0 base package is a complete application.
It needs no update for the current build.
Silksong uses the v0 base package and the latest cumulative update, v655360.
`internal/tools/prepare_game.py` reads a base package and an update package as separate inputs.

## References

- [Rhythm Heaven game record](../../internal/games/rhythm-heaven/README.md)
- [Rhythm Heaven profile](../../internal/games/rhythm-heaven/profile.json)
- [Silksong game record](../../internal/games/silksong/README.md)
- [Silksong profile](../../internal/games/silksong/profile.json)
- [CPU rewrite tool](../../internal/tools/rewrite_game.py)
- [Package extraction tool](../../internal/tools/prepare_game.py)
- [Shader build tool](../../internal/tools/build_shaders.py)
- [Host boot and service code](../../internal/runtime/common/ss_host.c)
- [NVN to Metal mapping](../../.nitenphone/dependencies/d2r-mobile/dm/gfx/nvn_metal_mapping.md)
- [Graphics layer](../../.nitenphone/dependencies/d2r-mobile/dm/gfx/README.md)
