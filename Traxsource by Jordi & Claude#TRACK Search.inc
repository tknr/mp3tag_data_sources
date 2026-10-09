#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#   Traxsource by Jordi  -  TRACK SEARCH  [v3.0 / 2026-04-26]
#   Verified against live HTML source 2026-04-25
#   Requires Mp3tag v3.26+ (Win) / v1.8.4+ (Mac)
#
#   CHANGELOG v3.0:
#   + [MinAppVersionWin/Mac] added
#   + [Settings] file added (user-configurable YEAR format, field toggles)
#   + YEAR: format configurable YYYY / YYYY-MM-DD / DD-MM-YYYY
#   + RELEASETIME for Rekordbox (off by default, enable in settings)
#   + All fields controlled by SettingWrite* from settings
#   + TrackMixName trailing-space strip improved (non-greedy quantifier)
#   + ALBUMARTIST removed from track page (redundant, same as ARTIST)
#   + Fallback IfNotOutput for TRAXSOURCE_TRACKID and WWW
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[Name]=Traxsource by Jordi & Claude (Track Search) [v3.0]
[BasedOn]=https://www.traxsource.com
[IndexUrl]=https://www.traxsource.com/search/tracks?term=%s
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=% ↓ RELEASED%|% ↓ GENRE%|% ↓ LENGTH%|% ↓ TRACK ID%|% ↓ TITLE (Mix)%|% ↓ ARTIST/S%|% ↓ REMIXER/S%|%_URL%
[Encoding]=url-utf-8
[MinAppVersionWin]=3.26
[MinAppVersionMac]=1.8.4
[Settings]=&Traxsource by Jordi & Claude#USER SETTIN&GS.settings

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
[ParserScriptIndex]=...
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# debug "on"  "C:\debug-ts-track-index.html"

findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace
regexpreplace ">\s+<" "><"
regexpreplace "\s+" " "

# ---- Mark track boundaries ----
regexpreplace "<div data-trid=\"([^\"]+)\" class=\"trk-row[^\"]*\">" "<<endTrack>><<nextTrack>><TrackID=$1>"

# ---- Track URL and Title ----
regexpreplace "<!--DIV title--><a href=\"([^\"]+)\">([^<]+)</a>" "<TrackURL=$1><TrackName=$2>"

# ---- Mix name and duration ----
regexpreplace "<span class=\"version\">([^<]*)<span class=\"duration\">\(([^\)]*)\)</span>" "<TrackMixName=$1><TrackLength=$2>"

# ---- Artists ----
regexpreplace "class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<TrackArtistID=$1><TrackArtistName=$2>"

# ---- Remixers ----
regexpreplace "class=\"com-remixers\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<TrackRemixerID=$1><TrackRemixerName=$2>"

# ---- Label ----
regexpreplace "<!--DIV label--><a href=\"[^\"]+\">([^<]+)</a>" "<TrackLabel=$1>"

# ---- Key and BPM ----
regexpreplace "<div class=\"trk-cell key-bpm\">([^<]+)<br>([^<]+)</div>" "<TrackKey=$1><TrackBpm=$2>"

# ---- Genre ----
regexpreplace "<!--DIV genre--><a href=\"[^\"]+\">([^<]+)</a>" "<TrackGenre=$1>"

# ---- Release date ----
regexpreplace "<div class=\"trk-cell r-date\">([0-9][^<\s]+)[^<]*</div>" "<TrackDate=$1>"
regexpreplace "<div class=\"trk-cell r-date\"><span class='pre-ord-txt'>Pre-order for\s+([^<]+)</span></div>" "<TrackDate=$1>"

# ---- TitleID from cart data ----
regexpreplace "data-cart=\"\{title_id: ([^,]+), track_id: ([^\}]+)\}\"" "<TitleID=$1><TrackID2=$2>"

# ---- Normalize mix names (non-greedy to strip trailing spaces) ----
regexpreplace "<TrackMixName=\s*>" "<TrackMixName=>"
regexpreplace "<TrackMixName=([^>]+?)\s*>" "<TrackMixName=$1>"
regexpreplace "<TrackMixName=([^>]*)Main Mix>" "<TrackMixName=$1Original Mix>"
regexpreplace "<TrackMixName=([^>]*)Original Vocal Mix>" "<TrackMixName=$1Original Mix>"
regexpreplace "<TrackMixName=([^>]*)Extended Version>" "<TrackMixName=$1Extended Mix>"
regexpreplace "<TrackMixName=([^>]*)rework>" "<TrackMixName=$1Remix>"
regexpreplace "<TrackMixName=>" "<TrackMixName=Original Mix>"

# ---- End markers ----
regexpreplace "<div class=\"search-pager\">" "<<endTrack>>"
regexpreplace "</body>" "<<endTrack>>"

replace "\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
# OUTPUT LOOP
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

do
findinline "<<nextTrack>>"

    sayregexp "(?<=<TrackDate=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<TrackGenre=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<TrackLength=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<TrackID=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<TrackName=)[^>]+" "" "<<endTrack>>"
    say " ("
    sayregexp "(?<=<TrackMixName=)[^>]+" "" "<<endTrack>>"
    say ")"
    say "|"
    sayregexp "(?<=<TrackArtistName=)[^>]+" ", " "<<endTrack>>"
    say "|"
    sayregexp "(?<=<TrackRemixerName=)[^>]+" ", " "<<endTrack>>"
    say "|"
    say "https://www.traxsource.com"
    sayregexp "(?<=<TrackURL=)[^>]+" "" "<<endTrack>>"

saynewline
findinline "<<endTrack>>" 1 1
while "<<nextTrack>>" 99

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
[ParserScriptAlbum]=...
# Parses: /track/{id}/slug
# Verified against live HTML 2026-04-25
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# debug "on"  "C:\debug-ts-track-album.html"

findline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace
regexpreplace ">\s+<" "><"
regexpreplace "\s+" " "

# ---- Cover (full res from /files/images/) ----
regexpreplace "<div class=\"tr-image\"><a href=\"[^\"]+\"><img src=\"([^\"]+)\"" "<CoverURL=$1>"

# ---- Track name and mix name ----
regexpreplace "<h1 class=\"title\">([^<]+)</h1>" "<TrackName=$1>"
regexpreplace "<h1 class=\"version\">([^<]+)</h1>" "<TrackMixName=$1>"

# ---- Artists and Remixers ----
regexpreplace "class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<TrackArtistID=$1><TrackArtistName=$2>"
regexpreplace "class=\"com-remixers\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<TrackRemixerID=$1><TrackRemixerName=$2>"

# ---- Detail table ----
regexpreplace "<td>Label:</td><td class=\"det\"><a href=\"[^\"]+\">([^<]+)</a></td>" "<TrackLabel=$1>"
regexpreplace "<td>Released:</td><td class=\"det\">([^<]+)</td>" "<TrackDate=$1>"
regexpreplace "<td>Released:</td><td class=\"det\"><span class='pre-ord-txt'>Pre-order for\s+([^<]+)</span></td>" "<TrackDate=$1>"
regexpreplace "<td>Length:</td><td class=\"det\">([^<]+)</td>" "<TrackLength=$1>"
regexpreplace "<td>Genre:</td><td class=\"det\"><a href=\"[^\"]+\">([^<]+)</a></td>" "<TrackGenre=$1>"
regexpreplace "<td>Key:</td><td class=\"det\">([^<]+)</td>" "<TrackKey=$1>"
regexpreplace "<td>BPM:</td><td class=\"det\">([^<]+)</td>" "<TrackBpm=$1>"

# ---- TrackID (hidden input, most reliable) ----
regexpreplace "<input type=\"hidden\" id=\"tpTrid\" value=\"([^\"]+)\"" "<TrackID=$1>"

# ---- TitleID + TrackID (cart, fallback) ----
regexpreplace "data-cart=\"\{title_id: ([^,]+), track_id: ([^\}]+)\}\"" "<TitleID=$1><TrackID_cart=$2>"

# ---- Release name ----
regexpreplace "<div class=\"ttl-info ellip\">Release: <a href=\"[^\"]+\" title=\"([^\"]+)\">" "<ReleaseName=$1>"

# ---- Normalize mix names ----
regexpreplace "<TrackMixName=\s*>" "<TrackMixName=>"
regexpreplace "<TrackMixName=([^>]+?)\s*>" "<TrackMixName=$1>"
regexpreplace "<TrackMixName=([^>]*)Main Mix>" "<TrackMixName=$1Original Mix>"
regexpreplace "<TrackMixName=([^>]*)Original Vocal Mix>" "<TrackMixName=$1Original Mix>"
regexpreplace "<TrackMixName=([^>]*)Extended Version>" "<TrackMixName=$1Extended Mix>"
regexpreplace "<TrackMixName=([^>]*)rework>" "<TrackMixName=$1Remix>"
regexpreplace "<TrackMixName=>" "<TrackMixName=Original Mix>"

replace "\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
# OUTPUT FIELDS
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

IfVar "SettingWriteCover" "true"
outputto "COVERURL"
sayregexp "(?<=<CoverURL=)[^>]+"
EndIf

outputto "ARTIST"
sayregexp "(?<=<TrackArtistName=)[^>]+" ", "
IfVar "SettingWriteArtist" "false"
Set "ARTIST"
EndIf

# ALBUMARTIST = ARTIST for single tracks
outputto "ALBUMARTIST"
SayOutput "ARTIST"

IfVar "SettingWriteMixArtist" "true"
outputto "MIXARTIST"
sayregexp "(?<=<TrackRemixerName=)[^>]+" ", "
EndIf

IfVar "SettingWriteTitle" "true"
outputto "TITLE"
sayregexp "(?<=<TrackName=)[^>]+"
say " ("
sayregexp "(?<=<TrackMixName=)[^>]+"
say ")"
EndIf

outputto "ALBUM"
sayregexp "(?<=<ReleaseName=)[^>]+"

IfVar "SettingWritePublisher" "true"
outputto "PUBLISHER"
sayregexp "(?<=<TrackLabel=)[^>]+"
EndIf

IfVar "SettingWriteBpm" "true"
outputto "BPM"
sayregexp "(?<=<TrackBpm=)[^>]+"
EndIf

IfVar "SettingWriteKey" "true"
outputto "INITIALKEY"
sayregexp "(?<=<TrackKey=)[^>]+"
EndIf

IfVar "SettingWriteGenre" "true"
outputto "GENRE"
sayregexp "(?<=<TrackGenre=)[^>]+"
EndIf

IfVar "SettingWriteLength" "true"
outputto "LENGTH"
sayregexp "(?<=<TrackLength=)[^>]+"
EndIf

# YEAR — format configurable via settings
IfVar "SettingWriteYear" "true"
outputto "YEAR"
IfVar "SettingYearFormat" "YYYY"
sayregexp "(?<=<TrackDate=)\d{4}"
EndIf
IfVar "SettingYearFormat" "YYYY-MM-DD"
sayregexp "(?<=<TrackDate=)[^>]+"
EndIf
IfVar "SettingYearFormat" "DD-MM-YYYY"
sayregexp "(?<=<TrackDate=)[^>]+"
regexpreplace "(\d{4})-(\d{2})-(\d{2})" "$3-$2-$1"
EndIf
IfNotOutput "YEAR"
sayregexp "(?<=<TrackDate=)[^>]+"
EndIf
EndIf

# RELEASETIME for Rekordbox (ISO 8601, opt-in)
IfVar "SettingWriteReleasetime" "true"
outputto "RELEASETIME"
sayregexp "(?<=<TrackDate=)\d{4}-\d{2}-\d{2}"
say "T00:00:00Z"
EndIf

IfVar "SettingWriteTrackIds" "true"
outputto "TRAXSOURCE_TRACKID"
sayregexp "(?<=<TrackID=)[^>]+"
IfNotOutput "TRAXSOURCE_TRACKID"
sayregexp "(?<=<TrackID_cart=)[^>]+"
EndIf

outputto "TRAXSOURCE_TITLEID"
sayregexp "(?<=<TitleID=)[^>]+"
EndIf

IfVar "SettingWriteWww" "true"
outputto "WWW"
say "https://www.traxsource.com/track/"
sayregexp "(?<=<TrackID=)[^>]+"
IfNotOutput "WWW"
say "https://www.traxsource.com/track/"
sayregexp "(?<=<TrackID_cart=)[^>]+"
EndIf
EndIf

IfVar "SettingWriteComment" "true"
outputto "COMMENT"
say "Tagged with Traxsource by Jordi & Claude v3.0"
EndIf
