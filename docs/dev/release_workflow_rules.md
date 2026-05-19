# Dev Notes - Release workflow & commit rules (SignalTwin)

This repository uses a **commit-message driven automated release flow**.
A release is triggered only when the latest commit on `main` starts with a
specific marker in parentheses.

---

## 1) When a release is created

The automated release flow runs only if the commit message:

- starts with exactly one of:
  `"(major)"`, `"(minor)"`, `"(patch)"`, `"(build)"`
- follows the format:
  `"(marker) Short description"`

Valid examples:
- `(build) Hotfix SD path`
- `(patch) Fix CSV export header`
- `(minor) Add DataBundle session metadata`
- `(major) Protocol v2 migration`

Invalid examples:
- `Hotfix (build)` (marker is not at the beginning)
- `(build)(patch) ...` (multiple markers)
- `build: Hotfix` (wrong format)

---

## 2) Marker meaning & version bump rules

Version is stored in the [VERSION](/D:/Prace/MTA/SignalTwinProject/VERSION) file
using the format:

- `x.y.z.b`

Where `b` is the build number.

Bump rules:

- `(major)` -> `(x+1).0.0.0`
- `(minor)` -> `x.(y+1).0.0`
- `(patch)` -> `x.y.(z+1).0`
- `(build)` -> `x.y.z.(b+1)`

---

## 3) Automated release flow

The new release workflow is defined in
[.github/workflows/release-auto.yml](/D:/Prace/MTA/SignalTwinProject/.github/workflows/release-auto.yml)
and orchestrates the release in this order:

1. Read the current `VERSION` and compute `NEXT`.
2. Write `NEXT` to [VERSION](/D:/Prace/MTA/SignalTwinProject/VERSION).
3. Run [storage/sync_config.py](/D:/Prace/MTA/SignalTwinProject/storage/sync_config.py)
   so config artifacts match the bumped version.
4. Commit and push the version/config update.
5. Call the reusable build workflow
   [.github/workflows/build.yml](/D:/Prace/MTA/SignalTwinProject/.github/workflows/build.yml)
   to compile the bumped revision.
6. Download the generated `.bin` artifacts and refresh `bin/latest/*`.
7. Commit and push the refreshed `bin/latest`.
8. Create and push tag `vX.Y.Z.B`.
9. Create the GitHub Release using `RELEASE_NOTES.md` and files from `bin/latest/*`.

This order guarantees that the published firmware is built from the bumped
version, not from the previous one.

---

## 4) Build workflow

The reusable build workflow is defined in
[.github/workflows/build.yml](/D:/Prace/MTA/SignalTwinProject/.github/workflows/build.yml).

Current defaults:

- board: `ESP32S3 Dev Module`
- core: `esp32` `3.1.1`
- output directory: `ui/build/esp32.esp32.esp32s3`

The workflow uploads compiled firmware as workflow artifacts and the release
workflow later republishes them into `bin/latest`.

---

## 5) Release notes

[RELEASE_NOTES.md](/D:/Prace/MTA/SignalTwinProject/RELEASE_NOTES.md) is still
maintained manually.

Before triggering a release, update the top section so it matches the version
you are about to publish.

---

## 6) Release binaries / artifacts

The release expects these firmware outputs:

- `ui.ino.merged.bin`
- `ui.ino.bin`
- `ui.ino.bootloader.bin`
- `ui.ino.partitions.bin`

The external auto-update tool consumes the mirrored copies in
`bin/latest/*`.

---

## 7) Recommended release procedure

1. Update [RELEASE_NOTES.md](/D:/Prace/MTA/SignalTwinProject/RELEASE_NOTES.md).
2. Push a commit to `main` whose message starts with exactly one release marker.
3. Let `release-auto.yml` perform the bump, build, asset refresh, tag, and release.

---

## 8) Legacy workflow

[.github/workflows/release.yml](/D:/Prace/MTA/SignalTwinProject/.github/workflows/release.yml)
is now only a manual placeholder so the repository keeps a stable entry point
for anyone looking for the old workflow name.
