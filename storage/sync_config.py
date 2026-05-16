#!/usr/bin/env python3
"""Synchronize project config artifacts from data/config.json.

Run from anywhere inside the repository:

    python data/sync_config.py

The canonical source of truth is the config.json next to this script. Before
copying, the script updates its top-level "version" field from the root VERSION
file. It then updates the UI data copy and the embedded firmware fallback
header.
"""

from __future__ import annotations

import argparse
import json
import shutil
from pathlib import Path


def repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def source_config() -> Path:
    # Get __file__ dir and append data/config.json to it, ensuring we get the correct path even if the script is run from a different directory.
    return Path(__file__).resolve().parent / "data" / "config.json"


def version_file(root: Path) -> Path:
    return root / "VERSION"


def ui_config(root: Path) -> Path:
    return root / "ui" / "data" / "config.json"


def default_json_header(root: Path) -> Path:
    return root / "libraries" / "engine" / "src" / "managers" / "default_json_config.hpp"


def read_version(root: Path) -> str:
    version = version_file(root).read_text(encoding="utf-8").strip()
    if not version:
        raise ValueError("VERSION is empty.")
    return version


def load_config(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as handle:
        data = json.load(handle)
    if not isinstance(data, dict):
        raise ValueError("config.json must contain a JSON object.")
    return data


def write_config(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    text = json.dumps(data, indent=2, ensure_ascii=False) + "\n"
    path.write_text(text, encoding="utf-8")


def build_default_json_header(source: Path) -> str:
    config_text = source.read_text(encoding="utf-8").strip()
    return f'#define DEFAULT_JSON_CONFIG R"json({config_text}\n)json"\n'


def sync_config(dry_run: bool = False) -> int:
    root = repo_root()
    source = source_config()
    config = load_config(source)
    config["version"] = read_version(root)

    targets = [
        ui_config(root),
        default_json_header(root),
    ]

    if dry_run:
        print(f"Would update {source.relative_to(root)} version={config['version']}")
        for target in targets:
            print(f"Would update {target.relative_to(root)}")
        return len(targets) + 1

    write_config(source, config)
    print(f"Updated {source.relative_to(root)} version={config['version']}")

    ui_target = ui_config(root)
    ui_target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(source, ui_target)
    print(f"Updated {ui_target.relative_to(root)}")

    header = default_json_header(root)
    header.parent.mkdir(parents=True, exist_ok=True)
    header.write_text(build_default_json_header(source), encoding="utf-8")
    print(f"Updated {header.relative_to(root)}")

    return len(targets) + 1


def main() -> None:
    parser = argparse.ArgumentParser(description="Synchronize config artifacts from data/config.json.")
    parser.add_argument("--dry-run", action="store_true", help="List targets without writing.")
    args = parser.parse_args()

    count = sync_config(dry_run=args.dry_run)
    print(f"Done. Targets: {count}")


if __name__ == "__main__":
    main()
