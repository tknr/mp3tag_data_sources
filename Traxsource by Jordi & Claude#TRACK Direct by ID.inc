#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#   Traxsource by Jordi  -  TRACK DIRECT by ID  [v3.0 / 2026-04-26]
#   Accede: https://www.traxsource.com/track/{TRAXSOURCE_TRACKID}
#   Mismos selectores que TRACK Search [ParserScriptAlbum]
#   Requires Mp3tag v3.26+ (Win) / v1.8.4+ (Mac)
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[Name]=Traxsource by Jordi & Claude (Track Direct) [v3.0]
[BasedOn]=https://www.traxsource.com
[AlbumUrl]=https://www.traxsource.com/track/%s
[Encoding]=url-utf-8
[MinAppVersionWin]=3.26
[MinAppVersionMac]=1.8.4
[Settings]=&Traxsource by Jordi & Claude#USER SETTIN&GS.settings

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
[ParserScriptAlbum]=...
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# debug "on"  "C:\debug-ts-track-direct.html"

findline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace
regexpreplace ">\s+<" "><"
regexpreplace "\s+" " "

regexpreplace "<div class=\"tr-image\"><a href=\"[^\"]+\"><img src=\"([^\"]+)\"" "<CoverURL=$1>"
regexpreplace "<h1 class=\"title\">([^<]+)</h1>" "<TrackName=$1>"
regexpreplace "<h1 class=\"version\">([^<]+)</h1>" "<TrackMixName=$1>"
regexpreplace "class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<TrackArtistID=$1><TrackArtistName=$2>"
regexpreplace "class=\"com-remixers\" data-aid=\"([^\"]+)\">([^<]+)</a>" "<TrackRemixerID=$1><TrackRemixerName=$2>"
regexpreplace "<td>Label:</td><td class=\"det\"><a href=\"[^\"]+\">([^<]+)</a></td>" "<TrackLabel=$1>"
regexpreplace "<td>Released:</td><td class=\"det\">([^<]+)</td>" "<TrackDate=$1>"
regexpreplace "<td>Released:</td><td class=\"det\"><span class='pre-ord-txt'>Pre-order for\s+([^<]+)</span></td>" "<TrackDate=$1>"
regexpreplace "<td>Length:</td><td class=\"det\">([^<]+)</td>" "<TrackLength=$1>"
regexpreplace "<td>Genre:</td><td class=\"det\"><a href=\"[^\"]+\">([^<]+)</a></td>" "<TrackGenre=$1>"
regexpreplace "<td>Key:</td><td class=\"det\">([^<]+)</td>" "<TrackKey=$1>"
regexpreplace "<td>BPM:</td><td class=\"det\">([^<]+)</td>" "<TrackBpm=$1>"
regexpreplace "<input type=\"hidden\" id=\"tpTrid\" value=\"([^\"]+)\"" "<TrackID=$1>"
regexpreplace "data-cart=\"\{title_id: ([^,]+), track_id: ([^\}]+)\}\"" "<TitleID=$1><TrackID_cart=$2>"
regexpreplace "<div class=\"ttl-info ellip\">Release: <a href=\"[^\"]+\" title=\"([^\"]+)\">" "<ReleaseName=$1>"

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

IfVar "SettingWriteCover" "true"
outputto "COVERURL"
sayregexp "(?<=<CoverURL=)[^>]+"
EndIf

outputto "ARTIST"
sayregexp "(?<=<TrackArtistName=)[^>]+" ", "
IfVar "SettingWriteArtist" "false"
Set "ARTIST"
EndIf

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
