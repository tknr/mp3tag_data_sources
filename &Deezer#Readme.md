# Readme for Deezer Web Source script for Mp3tag

# Changes

## [2026-08-28]  v1.32  by @yorickausyps

- New support for cover preview button on list of search result screen since mp3tag version 3.36-beta6. For this new feature to work you need to update to mp3tag version 3.36.
- Extended standard capitalization to album and track titles and by preserving uppercase letters.

## [2026-08-16]  v1.31  by @yorickausyps

- New search script `Deezer URL` to enter a deezer album url directly.
- New configuration setting to apply standard capitalization to track titles.
- Added error processing for catching non-deezer Urls.
- Removed cover preview configuration setting, because it cannot be implemented without undesired side effects. Cover preview is available as native Mp3tag feature at the `Adjust tag information` screen via left-click on the cover.
- Improved `INVOLVEDPEOPLE` layout.
- Some code and speed improvements.

## [2026-07-24]  v1.30  by @yorickausyps

The Deezer WS has several benefits: It provides high resolution cover art with good quality, maintains extensive track information, the desired album is usually easy to find among the search results and until today it seems to be free to use without registration.

The Deezer WS script initially was written by @vikaesar in 2019 and had since then not been redesigned. In the meantime the Mp3tag web source script language was developed further and supports some new commands and features now, that could be profitably used in the Deezer script to improve it.

This new release of the Deezer WSS is intended to use most of the data the source provides and put it into appropriate tag fields. It provides several option settings, that control the processing of the script.

# New features

- 'List of search results' supports cover thumbnails, match rankings, number of tracks in an album and track title version.
Attention: no longer available: Preview controls at 'list of search results' and 'adjust tag information' optionally display cover art instead of album page in your browser.
- Tags support for original release date, compilation flag, copyright, track number/totaltracks, all contributing artists, ISRC and Deezer-ids for albums, artists and tracks.

# Deezer search scripts

- `Deezer Artist + Album` and `Deezer Artist + Title` differ only in the preset values used from stored album and title tags.
- `Deezer ID_Album`, `Deezer ID_Artist` and `Deezer ID_Title` use already stored Deezer-IDs for direct calls without searching.
- `Deezer Api Artist + Album` and `Deezer Api Artist + Title` use a different Api-based web source, that has genre information but misses many of the information the normal source contains.

# Deezer Options Settings

- Resolution of cover art can be choosen up to 1500x1500 pixels
- Previews at 'list of search results' and 'adjust tag information' display cover art instead of album page in your browser
- Append version description to track title
- Use leading zeros for track numbers (01, 02 ... 09)
- Print disc number in front of track number (only for multiple disc releases, implies leading zeros)
- Append track and disc totals to track and disc number
- Include author in composer tag field or in lyricist tag field
 
# Deezer album tags mapping to Mp3tag tag fields

- Album Title -> `ALBUM`
- Main Artist -> `ALBUMARTIST`
- Album URL -> `WWW`
- Album Picture -> `COVERURL`
- Label Name -> `COVERURL`
- UPC -> `BARCODE`
- Original Release Date (or Physical Release Date) -> `RELEASETIME`
- Year of Release Date -> `YEAR`
- Album Subtype Compilation -> `COMPILATION` Flag
- Album Subtypes Live, Studio, Karaoke -> `SETSUBTYPE`
- Deezer Album-, Artist- and Track-ID -> `DEEZER_ALBUM_ID`, `DEEZER_ARTIST_ID` and `DEEZER_TRACK_ID`
- Copyright and Production Copyright -> `COPYRIGHT` and `PRODUCTION_COPYRIGHT`

# Deezer song tags mapping to Mp3tag tag fields

- Main artist and featuring artists -> `ARTIST`
- Song title -> `TITLE`
- Title version description -> `SUBTITLE`
- Composer -> `COMPOSER`
- Conductor -> `CONDUCTOR`
- Author -> `COMPOSER` or `LYRICIST`
- All roles and names of song contributors -> `INVOLVEDPEOPLE`
- Duration (length in seconds) -> `_LENGTH`
- Gain -> `REPLAY_TRACK_GAIN`
- ISRC -> ISRC
- Explicit lyrics -> `ITUNESADVISORY` flag
 
# Version history

# [2026-07-24]  v1.30  by @yorickausyps

# [2024-05-01]  v1.20  Add ReplayGain tag

# [2022-12-19]  v1.10  Add track "version" to title instead of comment

# [2019-05-25]  v1.00  Initial release by @vikaesar
