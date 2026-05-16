#!/usr/bin/env python3
"""Synchronize project DB artifacts from data/DB.json.

Run from anywhere inside the repository:

    python data/sync_db.py

The canonical source of truth is the DB.json next to this script. The script
updates all secondary DB.json files and the embedded firmware fallback header.
"""

from __future__ import annotations

import argparse
import json
import shutil
from pathlib import Path


IGNORED_DIRS = {
    ".git",
    ".venv",
    "__pycache__",
    "node_modules",
}


def repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def source_db() -> Path:
    return Path(__file__).resolve().parent / "data" / "DB.json"



def default_json_header(root: Path) -> Path:
    return root / "libraries" / "engine" / "src" / "devices" / "default_json_db.hpp"


def iter_db_files(root: Path) -> list[Path]:
    db_files: list[Path] = []
    for path in root.rglob("DB.json"):
        if any(part in IGNORED_DIRS for part in path.parts):
            continue
        db_files.append(path)
    return sorted(db_files)


def validate_json(path: Path) -> None:
    with path.open("r", encoding="utf-8") as handle:
        json.load(handle)


def build_default_json_header(source: Path) -> str:
    db_text = source.read_text(encoding="utf-8").strip()
    return f'#define DEFAULT_JSON_DB R"json({db_text}\n)json"\n'


def sync_default_json_header(root: Path, source: Path, dry_run: bool = False) -> int:
    header = default_json_header(root)
    relative = header.relative_to(root)

    if dry_run:
        print(f"Would update {relative}")
        return 1

    header.write_text(build_default_json_header(source), encoding="utf-8")
    print(f"Updated {relative}")
    return 1


def sync_db(dry_run: bool = False) -> int:
    root = repo_root()
    source = source_db()
    validate_json(source)

    updated = 0
    targets = [path for path in iter_db_files(root) if path.resolve() != source.resolve()]
    source_pics = source.parent / "pics"
    if not targets:
        print("No DB.json targets found.")

    for target in targets:
        relative = target.relative_to(root)
        if dry_run:
            print(f"Would update {relative}")
        else:
            shutil.copyfile(source, target)
            print(f"Updated {relative}")
        updated += 1

        # Also copy the pics folder from the source data/ to the target's folder
        if source_pics.exists() and source_pics.is_dir():
            target_pics = target.parent / "pics"
            rel_pics = target_pics.relative_to(root)
            if dry_run:
                print(f"Would copy pics to {rel_pics}")
            else:
                if target_pics.exists():
                    shutil.rmtree(target_pics)
                shutil.copytree(source_pics, target_pics)
                print(f"Copied pics to {rel_pics}")
    updated += sync_default_json_header(root, source, dry_run=dry_run)
    return updated


def main() -> None:
    parser = argparse.ArgumentParser(description="Synchronize all project DB artifacts from data/DB.json.")
    parser.add_argument("--dry-run", action="store_true", help="List targets without copying.")
    args = parser.parse_args()

    count = sync_db(dry_run=args.dry_run)
    print(f"Done. Targets: {count}")


if __name__ == "__main__":
    main()
