# RELEASE_NOTES

## 🥳 SignalTwin — First Release (v1.0.0) — 2026-01-20

This is the first public “deploy-ready” release of **SignalTwin Display**: an ESP32-S3 HMI client for real-time sensor visualization, inspection, and logging over **VSCP**. The display itself is intentionally module-agnostic; educational modules live above it (e.g., EduBoxHub / PC emulator / custom HW).

---

## Highlights

* Real-time **raw vs. processed** data inspection.
* **DataBundle** recording and **CSV export to SD** for offline analysis.
* Built-in **Sensor Wiki** as a quick reference layer.
* **VSCP** (text-based, REST-like) connectivity to PC emulator, EduBoxHub, or custom HW.
* **Runtime pin mapping** and **bidirectional I/O** (telemetry + actuator/config writes).

---

## Build / Flash artifacts (binaries)

Release binaries are provided for Arduino/ESP32 flashing workflows:

* `ui.ino.merged.bin`
  Single “all-in-one” image intended for the simplest flashing path.

* `ui.ino.bin`
  Application binary (firmware payload).

* `ui.ino.bootloader.bin`
  Bootloader image.

* `ui.ino.partitions.bin`
  Partition table image.

Developer artifacts (for debugging/profiling):

* `ui.ino.elf` 
* `ui.ino.map` 

> Flashing instructions and offsets are documented in `docs/INSTALL.md`.

---

## Compatibility matrix (v1.0.0)

Upstream connectivity is based on **VSCP** (Virtual Sensors Communication Protocol):

* **PC Emulator**: supported (VSCP over the emulator/host link).
* **EduBoxHub**: supported (VSCP upstream integration).
* **Custom HW**: supported if it implements the open-source VSCP spec.

Protocol reference:

* VSCP repository: [https://github.com/sgtkingo/VirtualSensors_protocol](https://github.com/sgtkingo/VirtualSensors_protocol)

---

## Target hardware

* Verified target: **Elecrow ESP32 Display 7" HMI (ESP32-S3 + RGB TFT + Touch, LVGL-ready)**.

---

## Known limitations (v1.0.0)

* Visualization widgets and rendering policies depend on upstream channel metadata and current UI maturity.
* Heavy UI effects (e.g., excessive alpha/shadows) may be constrained by embedded rendering budget (frame time).
* “Supports any sensor” is bounded by what the upstream source can expose over VSCP and how channels are described.

---

## Upgrade / migration notes

* First release: no migration steps.

---

## Next (planned)

* Expanded visualization widgets (trend views, event timeline, markers).
* DataBundle/CSV schema stabilization and validation tooling.
* Improved sensor Wiki authoring workflow + contribution guidelines.
