# SignalTwin ✨🧬

**Current development version:** `v1.2.0.63`

SignalTwin Display is a lightweight **HMI/visualization client** for exploring sensor data in real time 🖥️.
It focuses on **raw vs. processed comparisons**, **interactive inspection**, and **data capture** for later analysis
— while educational modules live *above* this layer (e.g., in EduBoxHub or external course content).

## 🧩 Target Hardware

![SignalTwin Display – Use-case](docs/img/panel_apps.png)

SignalTwin Display is designed and tested for the **Elecrow ESP32 Display 7" HMI (ESP32-S3 + RGB TFT + Touch, LVGL-ready)**:

- 🔗 Distributor / purchase link:  
  https://www.elecrow.com/esp32-display-7-inch-hmi-display-rgb-tft-lcd-touch-screen-support-lvgl.html

---

## 🚀 Core Capabilities

### 🔎 Sensor Data Exploration
- 📊 Real-time visualization of **raw/real values** and derived/processed values (side-by-side comparison).
- 👀 Interactive browsing of streams and channels for quick inspection and debugging.
- 🌐 Designed as a generic viewer: **theoretically supports any sensor** as long as the upstream platform can provide it.

### 💾 Logging & Export
- 💽 Recording into **DataBundle** for structured capture sessions.
- 📝 Export to **CSV on SD card** for offline analysis (Python/Excel/Matlab workflows).

### 📚 Sensor Knowledge Base (Wiki)
- 📖 Built-in **Wiki** of sensors: quick reference for principles, typical ranges, pitfalls, and usage notes.
- ➕ Extensible content model (add new sensors without coupling to firmware logic).

---

## 🔗 Connectivity Model

SignalTwin Display does not read sensors directly in the general case.
It requires an upstream source that speaks our open protocol:

### 🔌 Supported Upstream Sources
- 🖥️ **PC connection (emulator / host tooling)**
- 🧩 **EduBoxHub** platform
- 🔧 **Custom hardware** implementing the same protocol

### 📡 Protocol: VSCP (Virtual Sensors Communication Protocol)
Communication is done via a **text-based, REST-like protocol**:

- 📚 Spec / reference repo: https://github.com/sgtkingo/VSCP

---

## ⚙️ Configuration & I/O (Bidirectional)

Beyond passive viewing, SignalTwin Display supports operational control via the upstream platform:

- 🔌 **Dynamic pin mapping**: set which physical pins a real sensor is connected to (runtime configuration).
- 🔁 **Bidirectional messaging**:
  - 📈 sensor reads (telemetry / streaming),
  - 🎚️ actuator control (sending values/commands),
  - 🛠️ configuration pushes (calibration, sampling, modes).

Runtime values can be explicitly marked as **read** or **write** in `DB.json`:

- `access: "read"` values are fetched through `UPDATE` and can be charted/recorded.
- `access: "write"` values are sent through `CONTROL` and shown as editable orange controls.
- `configs` are sent through `CONFIG` and shown separately from values in the runtime panel.

This enables true hybrid devices that combine telemetry, control and configuration in one device entry.
The bundled test catalog includes `H00` / **Temperature Regulator**:

- `temp`: read value returned by `UPDATE`,
- `set_point`: write value sent by `CONTROL`,
- `speed`: config value sent by `CONFIG`, controlling how quickly the emulated temperature reaches the set point.

---

## 📖 Documentation

- 🧩 **Installation & deployment** are in **INSTALL.md** (toolchain, flashing, SD layout, emulator wiring).
- 🧭 **Developer map** is in `DEV_MAP.md` (architecture, VSCP flow, DB schema, emulator notes).
- 📝 **Release notes** are in `RELEASE_NOTES.md`.
- Recommended (optional) docs to add:
  - 🧱 `docs/ARCHITECTURE.md` — dataflow & message types (VSCP mapping)
  - 🗃️ `docs/FORMATS.md` — DataBundle + CSV schema
  - 📝 `docs/WIKI_GUIDE.md` — how to add/edit sensor Wiki entries

---

## 🧭 Typical Workflow

1. 🔗 Connect SignalTwin Display to **PC Emulator** or **EduBoxHub** (or a VSCP-capable custom device).
2. 🔎 Select a sensor/channel and inspect **raw vs. processed** outputs.
3. 💾 Record a session into **DataBundle** and export **CSV to SD**.
4. 📊 Analyze captured data offline (Python/Excel/Matlab).

---

## 🔄 Recommended: Automatic Firmware Updates (Firmupdater)

SignalTwin Display is designed for **frequent deployments** (labs, classrooms, hotfix builds), so **automatic firmware updates are strongly recommended** via **Firmupdater**: https://github.com/sgtkingo/Firmupdater.

Firmupdater automatically checks for newer versions and applies updates **with minimal manual intervention**—perfect for managing multiple devices in parallel. 🚀

📖 Learn more: https://github.com/sgtkingo/Firmupdater
🌐 Online live-app: https://sgtkingo.github.io/Firmupdater/

---

## Versioning

Project builds use `MAJOR.MINOR.PATCH.BUILD`.

- Current development build: `1.2.0.63`.
- `MAJOR` stays at `1` for this product line.
- `MINOR` was bumped for the refactoring branch because it adds major runtime, DataBundle, device catalog, settings, transfer-mode and protocol-facing functionality.
- `BUILD` is the number of commits on the current development branch since `main` (`63` at the time of this version bump).

---

## 🗂️ Repository Structure (Recommended)

- 🖼️ `ui/` — firmware + UI logic + .INO file (LVGL app)
- 📚 `libraries` — all headers and libraries (engine) 
- 🐍 `emulator` — Python-based emulator for testing
- 📄 `docs/` — diagrams, screenshots, Wiki sources, installation instruction
- 📦 `bin` — exported binary files
- 🧾 `data` — data files files (configurations, CSV)
- 📝 `RELEASE_NOTES` — latest release notes
- 📄 `LICENSE` — MIT

---

## 🐞 Troubleshooting 

- ❌ If you can connect but see no data: verify the upstream device speaks **VSCP** and is streaming the expected channels.
- 💽 If SD export fails: check card formatting and required folder structure (see `INSTALL.md` in `docs/`).

---

## 🤝 Contributing

Contributions are welcome, especially:
- 📈 additional visualizations (plots, trend views, event markers),
- 🧱 DataBundle/CSV improvements and schema stability,
- 📖 new sensor Wiki pages.

📨 Please include: device/source type (PC/EduBoxHub/custom), VSCP message example, and expected output.

---

## 📄 License

MIT — see `LICENSE`.
