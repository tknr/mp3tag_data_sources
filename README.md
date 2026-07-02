# SoundCloud Web Source Script for Mp3tag
## Maintained by Jordi & Claude — v2.28 (2026-04-26)

Based on **SoundCloud WS V2.28** by **LNDN** (January 2026).
Original script by **Audioman2000**, updated by **Florian** (Mp3tag developer, July 2024).
Bugs fixed and maintained by **Jordi & Claude** (Anthropic, April 2026).

---

## What this does

Connects Mp3tag directly to **SoundCloud** so you can tag your tracks with
metadata from the SoundCloud platform:

- **Track/Mix Search** — search by artist name, track title, or both

**Tags written:** TITLE, ARTIST, MIXARTIST (uploader), ALBUMARTIST, GENRE,
TAGS (SoundCloud tags), PUBLISHER (label/rights holder), COPYRIGHT,
ORIGARTIST (original artist for remixes), YEAR, RELEASETIME, COVERURL
(high resolution), WWW (track URL), WWWARTIST (artist profile URL),
WWWPAYMENT (purchase URL), COMMENT (track description), UNSYNCEDLYRICS
(track description, long form).

---

## Requirements

| Platform | Minimum version |
|----------|----------------|
| Mp3tag for Windows | **v3.26** |
| Mp3tag for Mac | **v1.8.4** |

---

## Installation

1. Extract the contents of this ZIP (all files) into your Mp3tag sources folder:
   - **Windows:** `%APPDATA%\Mp3tag\data\sources`
   - **Mac:** `~/Library/Application Support/de.mp3tag.Mp3tag/Sources`
2. Restart Mp3tag (or press F5 to refresh).
3. The script appears in the menu under **Tag Sources → SoundCloud WS V2.28 by Jordi & Claude**.

---

## How to use

1. Select one or more tracks in Mp3tag.
2. Go to **Tag Sources → SoundCloud WS V2.28 by Jordi & Claude**.
3. Choose to search by **Artist**, **Title**, or both.
4. A dialog opens showing results (track title and uploader name).
5. Select the matching result and click **OK**.
6. Tags are written to your files.

**Tip:** If you already have the SoundCloud URL of a track, you can paste it
directly into the search box — Mp3tag will load that exact page.

---

## Configuration (User Settings)

Go to **Tag Sources → SoundCloud WS V2.28 by Jordi & Claude → Settings**
to open the settings panel. Key options:

| Option | Default | Description |
|--------|---------|-------------|
| Parse Title | ON | Write TITLE tag |
| Parse Artist | ON | Write ARTIST tag |
| Parse Cover Image | ON | Write COVERURL tag |
| Parse Genre | ON | Write GENRE tag |
| Parse Label | ON | Write PUBLISHER tag |
| Parse MixArtist | ON | Write MIXARTIST tag |
| Parse Year | ON | Write YEAR tag |
| Parse URL | ON | Write WWW tag |
| Year Format | YYYY-MM-DD | YYYY-MM-DD, YYYY, or DD-MM-YYYY |
| Genre Source | SoundCloud | Source for genre metadata |

---

## Technical notes

SoundCloud uses **server-side JSON hydration** (`window.__sc_hydration`) embedded
in the page HTML. This script extracts and parses that JSON directly — giving
access to the full structured metadata rather than scraping visible HTML.

This is the same technique used by the Beatport scripts and provides much
richer data than static HTML parsing.

**Important limitation:** SoundCloud search results in the no-JavaScript HTML
are limited to "popular" tracks. If a track doesn't appear in results, try
searching by the exact SoundCloud URL of the track instead.

The script targets position 9 in the hydration array for track data. If
SoundCloud updates their page structure this may need adjustment — a comment
in the `.inc` file flags this with debug instructions.

---

## Bug fixes applied in this version (vs original V2.28)

- **WWWARTIST was always empty:** The script was navigating to `user.user.permalink_url`
  (a path that doesn't exist) instead of `user.permalink_url`. Fixed.
- **Inconsistent `EndIf` casing:** Normalized all `endif`/`Endif` occurrences to
  `EndIf` for consistency.
- **Fragile array position:** Added inline comment flagging `json_select_array "" 9`
  as position-dependent, with debug instructions.

---

## Debugging

The `.inc` file contains commented-out `debug` lines near the top.
To enable, uncomment a debug line and set a valid output path:
```
# debug "on"  "%APPDATA%\Mp3tag\data\sources\debug_soundcloud.html"
```

---

## Credits

Original script: **Audioman2000**
Updated (Jul 2024): **Florian** (Mp3tag developer)
V2.28 (Jan 2026): **LNDN**
Bug fixes & maintenance: **Jordi & Claude** (Anthropic) — Apr 2026

Mp3tag forum thread: https://community.mp3tag.de/t/ws-soundcloud-com/63508
