# Dev Notes — Release workflow & commit rules (SignalTwin)

This repository uses **commit-message driven releases**: a release is triggered only when the latest commit on `main`
starts with a specific marker in parentheses.

---

## 1) When a release is created

A release (including tag + GitHub Release) happens **only if** the commit message:

- starts with exactly one of:  
  **`(major)`**, **`(minor)`**, **`(patch)`**, **`(build)`**
- follows the format:  
  **`(marker) Short description`**

Valid examples:
- `(build) Hotfix SD path`
- `(patch) Fix CSV export header`
- `(minor) Add DataBundle session metadata`
- `(major) Protocol v2 migration`

Invalid examples:
- `Hotfix (build)` (marker is not at the beginning)
- `(build)(patch) ...` (multiple markers)
- `build: Hotfix` (wrong format)
- `(build)` with no description (discouraged)

---

## 2) Marker meaning & version bump rules

Version is stored in the `VERSION` file using the format:

- **`x.y.z.b`**  
  where `b` is the **build number**

Bump rules:

- `(major)` → **(x+1).0.0.0**
- `(minor)` → x.(y+1).0.0
- `(patch)` → x.y.(z+1).0
- `(build)` → x.y.z.(b+1)

Notes:
- `(build)` is for tiny changes and deploy builds that do not justify a patch/minor bump.
- `(patch)` is for bugfixes that change behavior but do not change the public interface.

---

## 3) What the workflow does on release

If a commit matches the rules, GitHub Actions will:

1. Read the current `VERSION` and compute **NEXT** based on the marker.
2. Verify release assets exist in `bin/latest/`.
3. Write `NEXT` into `VERSION`, then commit:  
   `chore(release): bump version to x.y.z.b`
4. Create and push a git tag:  
   `vX.Y.Z.B`
5. Create a GitHub Release:
   - **Assets**: all files from directory defined in `BUILD` file, e.g: `bin/latest/*`, `ui/build/esp32.esp32.esp32s3/*`
   - **Release notes**: contents of `RELEASE_NOTES.md` (manually maintained)

---

## 4) Release notes (manual)

`RELEASE_NOTES.md` is maintained **manually**.

Rule:
- before triggering a release, ensure the top section of `RELEASE_NOTES.md` corresponds to the version you want to publish.

---

## 5) Release binaries / artifacts

The workflow attaches everything from directory defined in `BUILD` file, e.g:

- `ui/build/esp32.esp32.esp32s3/*`

Recommended contents (typical):
- `ui.ino.merged.bin`
- `ui.ino.bin`
- `ui.ino.bootloader.bin`
- `ui.ino.partitions.bin`
- (optional) `ui.ino.elf`, `ui.ino.map` for debugging/profiling

---

## 6) Recommended release procedure

1. Ensure `bin/latest/` contains the current build artifacts.
2. Update `RELEASE_NOTES.md` (manually).
3. Push a commit to `main` with one marker at the beginning, e.g.:  
   `(build) Hotfix`
4. GitHub Actions will create:
   - a bump commit updating `VERSION`
   - a tag `vX.Y.Z.B`
   - a GitHub Release with assets + release notes

---

## 7) Common pitfalls

- Marker not at the beginning → no release is triggered.
- Multiple markers in one commit → workflow fails.
- Empty `bin/latest/` → workflow fails (no release created).
- `VERSION` not in `x.y.z.b` format → workflow fails.

---
