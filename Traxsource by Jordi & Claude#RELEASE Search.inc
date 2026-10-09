#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#   Traxsource by Jordi  -  RELEASE SEARCH  [v3.0 / 2026-04-26]
#   Busca releases: https://www.traxsource.com/search/titles?term=
#   Requires Mp3tag v3.26+ (Win) / v1.8.4+ (Mac)
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[Name]=Traxsource by Jordi & Claude (Release Search) [v3.0]
[BasedOn]=https://www.traxsource.com
[IndexUrl]=https://www.traxsource.com/search/titles?term=%s
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=% ↓ TitleID%|% ↓ RELEASE%|% ↓ ARTIST/S%|% ↓ LABEL%|%_URL%
[Encoding]=url-utf-8
[MinAppVersionWin]=3.26
[MinAppVersionMac]=1.8.4
[Settings]=&Traxsource by Jordi & Claude#USER SETTIN&GS.settings

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
[ParserScriptIndex]=...
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# debug "on"  "C:\debug-ts-release-index.html"

findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace
regexpreplace ">\s+<" "><"
regexpreplace "\s+" " "

regexpreplace "<div class=\"search-pager\">" "<<endTrack>>"
regexpreplace "</body>" "<<endTrack>>"
regexpreplace "<div class=\"grid-item[^\"]*\" data-tid=\"([^\"]+)\">" "<<endTrack>><<nextTrack>><TitleID=$1>"
regexpreplace "<div class=\"links ellip\"><a href=\"([^\"]+)\" class=\"com-title\">([^<]+)</a>" "<ReleaseURL=$1><ReleaseName=$2>"
regexpreplace "<ReleaseURL=[^>]+><ReleaseName=[^>]+><br />([^<]+)<br />" "<ReleaseURL=$1>"
regexpreplace "<a href=\"/label/[^\"]+\" class=\"com-label\">([^<]+)</a>" "<ReleaseLabel=$1>"

replace "\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"

do
findinline "<<nextTrack>>"

    sayregexp "(?<=<TitleID=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<ReleaseName=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<ReleaseArtist=)[^>]+" "" "<<endTrack>>"
    say "|"
    sayregexp "(?<=<ReleaseLabel=)[^>]+" "" "<<endTrack>>"
    say "|"
    say "https://www.traxsource.com"
    sayregexp "(?<=<ReleaseURL=)[^>]+" "" "<<endTrack>>"

saynewline
findinline "<<endTrack>>" 1 1
while "<<nextTrack>>" 99

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
[ParserScriptAlbum]=...
# Parses: /title/{TitleID}/slug
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# debug "on"  "C:\debug-ts-release-album.html"

findline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace
regexpreplace ">\s+<" "><"
regexpreplace "\s+" " "

regexpreplace "<meta property=\"og:image\" content=\"([^\"]+)\">" "<CoverURL=$1>"
regexpreplace "<div id=\"ttlPageCont\" data-tid=\"([^\"]+)\">" "<TitleID=$1>"
regexpreplace "<meta property=\"og:url\" content=\"([^\"]+)\">" "<ReleaseURL=$1>"
regexpreplace "<div class=\"page-head\"><h1 class=\"title\">([^<]+)</h1>" "<ReleaseName=$1>"
regexpreplace "class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<ArtistID=$1><ArtistName=$2>"
regexpreplace "<a class=\"com-label\" href=\"[^\"]+\">([^<]+)</a>" "<ReleaseLabel=$1>"
regexpreplace "<div class=\"cat-rdate\">([^\s<]+)\s+\$verticalBar\(\)\s+([^<]+)</div>" "<CatalogID=$1><ReleaseDate=$2>"
regexpreplace "<div class=\"cat-rdate\">([^\s<]+)\s+\$verticalBar\(\)\s+<span class='pre-ord-txt'>Pre-order for\s+([^<]+)</span>" "<CatalogID=$1><ReleaseDate=$2>"

replace "\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"

IfVar "SettingWriteCover" "true"
outputto "COVERURL"
sayregexp "(?<=<CoverURL=)[^>]+"
EndIf

outputto "ALBUMARTIST"
sayregexp "(?<=<ArtistName=)[^>]+" ", "

outputto "ALBUM"
sayregexp "(?<=<ReleaseName=)[^>]+"

IfVar "SettingWritePublisher" "true"
outputto "PUBLISHER"
sayregexp "(?<=<ReleaseLabel=)[^>]+"
EndIf

IfVar "SettingWriteYear" "true"
outputto "YEAR"
IfVar "SettingYearFormat" "YYYY"
sayregexp "(?<=<ReleaseDate=)\d{4}"
EndIf
IfVar "SettingYearFormat" "YYYY-MM-DD"
sayregexp "(?<=<ReleaseDate=)[^>]+"
EndIf
IfVar "SettingYearFormat" "DD-MM-YYYY"
sayregexp "(?<=<ReleaseDate=)[^>]+"
regexpreplace "(\d{4})-(\d{2})-(\d{2})" "$3-$2-$1"
EndIf
IfNotOutput "YEAR"
sayregexp "(?<=<ReleaseDate=)[^>]+"
EndIf
EndIf

IfVar "SettingWriteReleasetime" "true"
outputto "RELEASETIME"
sayregexp "(?<=<ReleaseDate=)\d{4}-\d{2}-\d{2}"
say "T00:00:00Z"
EndIf

outputto "CATALOG #"
sayregexp "(?<=<CatalogID=)[^>]+"

IfVar "SettingWriteTrackIds" "true"
outputto "TRAXSOURCE_TITLEID"
sayregexp "(?<=<TitleID=)[^>]+"
EndIf

IfVar "SettingWriteWww" "true"
outputto "WWW"
sayregexp "(?<=<ReleaseURL=)[^>]+"
EndIf

IfVar "SettingWriteComment" "true"
outputto "COMMENT"
say "Tagged with Traxsource by Jordi & Claude v3.0"
EndIf
