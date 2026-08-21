# Changelog — Bandcamp Tag Source for Mp3tag

## 0.4.8 (2026-07-02) @yorickausyps

- Added configuration setting to select track numbers with leading zeros and to append total track count to track numbers.
- Redesigned configuration settings and changed setting for `YEAR` format to type boolean.
- Added support for custom tag field `COMMENT URL`, that will get bandcamp urls if there are any included in the `COMMENT` tag field respectively the web sources description json object.

## 0.4.7 (2026-06-22) @yorickausyps

- Fixed artist from track URLs may have multiple values.

## 0.4.6 (2026-06-16) @yorickausyps

- Added support for `WWW TRACK` to store Bandcamp track URLs.
- Added script `Bandcamp Album` to request Bandcamp Album URL, using existing contents from `WWW` (same as first part of `Bandcamp URL`).
- Added script `Bandcamp Track` to request Bandcamp Track URL, using existing contents from `WWW TRACK`. This enables to use the Bandcamp Album results to afterwards request Bandcamp Track info and artwork for one track after another.
- Added configuration setting to use separate `COMMENT`, `CREDITS`, `GENRE` and `KEYWORDS` tags for track Urls, to allow easy comparison of track tags with corresponding album tags.
- Moved script ParserScriptAlbum to a separate include file to be used by `BANDCAMP ALBUM` and `BANDCAMP TRACK` (avoiding code duplication).
- Introduced separate branches in ParserScriptAlbum for album and track URL requests to handle them accordingly.
- Added support for `IRSC` for track URL results only.
- Added support for `PUBLISHER`.
- Changed to use standard capitalization for `KEYWORDS`.
- Added a missing json_unselect_object command.

## 0.4.5 (2026-06-03) @yorickausyps

- Use existing contents from `WWW` when searching by Bandcamp URL.

## 0.4.4 (2026-05-05) @Florian

- Added support for searching by Bandcamp URL.
- Changed to use standard capitalization for `GENRE`.
- Fixed `GENRE` contained Bandcamp discover URL.

## 0.4.3 (2024-05-24) @Florian

- Added support for cover thumbnails in search results.

## 0.4.2 (2023-09-17) @Florian

- Added support for cover thumbnails in search results (macOS-only).
- Added configuration setting for `YEAR` format.
- Added configuration setting to use highest quality for cover art.
- Added configuration settings to omit albums or tracks from search results.

## 0.4.1 (2023-09-13) @Florian

- Added error handling in case search term returns no results.
- Fixed typo in `WordSeparator` configuration setting, which effectively removed every space character from the query before it was sent to Bandcamp.

## 0.4.0 (2023-09-12) @Florian

- Changed to use JSON representation for parsing album part.

## 0.3.4 (2023-09-11) @Florian

- Fixed track numbers and titles are not always listed correctly.

## 0.3.3 (2021-06-13) @ms6676749

- Changed to return artist and track results only.

## 0.3.2 (2021-04-26) @ms6676749

- Updated to modified search page.

## 0.3.1 (2020-10-08) @ms6676749

- Fixed broken Tag Source.

## 0.3.0 (2020-09-28) @ms6676749

- Fixed broken Tag Source.

## 0.2.0 (2019-01-16) @Florian

- Fixed broken Tag Source.

## 0.1.3 (2017-06-18) @ms6676749

- Minor fixes.

## 0.1.2 (2016-06-26) @ms6676749

- Minor fixes.

## 0.1.1 (2016-05-16) @ms6676749

- Minor fixes.

## 0.1.0 (2016-04-11) @ms6676749

- Initial release.