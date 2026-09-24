#!/usr/bin/env python3
"""Provider-neutral boot menu prototype.

This is a host-side UI/protocol prototype. It deliberately does not load kernels,
modify firmware variables, or bypass signature verification. A real boot manager
must connect this presentation layer to a provider-specific verified backend.
"""
from __future__ import annotations

import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class Entry:
    identifier: str
    title: str
    version: str
    provider: str
    verified: bool
    recovery: bool = False


def load_entries(path: Path) -> list[Entry]:
    document = json.loads(path.read_text(encoding="utf-8"))
    if document.get("schema") != 1:
        raise ValueError("unsupported boot menu schema")
    entries = []
    for item in document.get("entries", []):
        required = ("id", "title", "version", "provider", "verified")
        if any(key not in item for key in required):
            raise ValueError("boot entry is missing a required field")
        entries.append(
            Entry(
                identifier=str(item["id"]),
                title=str(item["title"]),
                version=str(item["version"]),
                provider=str(item["provider"]),
                verified=bool(item["verified"]),
                recovery=bool(item.get("recovery", False)),
            )
        )
    return entries


def render(entries: list[Entry]) -> None:
    print("xnu++ boot manager prototype")
    print("----------------------------")
    for index, entry in enumerate(entries, 1):
        state = "verified" if entry.verified else "REJECTED"
        suffix = " [recovery]" if entry.recovery else ""
        print(f"{index}. {entry.title} {entry.version} ({entry.provider}) - {state}{suffix}")
    print("q. quit")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path, help="JSON boot-menu manifest")
    parser.add_argument("--non-interactive", action="store_true")
    args = parser.parse_args()
    try:
        entries = load_entries(args.manifest)
    except (OSError, ValueError, json.JSONDecodeError) as error:
        print(f"boot menu error: {error}", file=sys.stderr)
        return 2
    render(entries)
    if args.non_interactive:
        return 0 if any(entry.verified and not entry.recovery for entry in entries) else 1
    while True:
        choice = input("select entry: ").strip().lower()
        if choice == "q":
            return 0
        if choice.isdigit() and 1 <= int(choice) <= len(entries):
            entry = entries[int(choice) - 1]
            if not entry.verified:
                print("refusing unverified entry")
                continue
            print(f"selected {entry.identifier}; verified backend handoff is not implemented")
            return 0
        print("invalid selection")


if __name__ == "__main__":
    raise SystemExit(main())
