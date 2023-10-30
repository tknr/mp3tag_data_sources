[Name]=Traxsource track search
[BasedOn]=https://www.traxsource.com
[AlbumUrl]=https://www.traxsource.com/track/%s

[Encoding]=url-utf-8

# V0.3
# Mp3tag v3.22+ is required

# debug "on"  "c:\Traxsource track direct debug 1 index.html" 20
# DebugWriteInput "c:\debug-input.out"
#######################

[ParserScriptAlbum]=...


findline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"

unspace

regexpreplace "<meta property=\"og:image\" content=\"([^\"]*)\">"  "<AlbumImageURL=$1>"
regexpreplace "<meta property=\"og:url\" content=\"https:\/\/www\.traxsource\.com\/track\/([^\"]*)\/[^\"]*\">" "<TrackTrackID2=$1>"
regexpreplace "\"@id\": \"https:\/\/www\.traxsource\.com\/title\/([^\"]*)\/[^\"]*\"" "<TrackTitleID2=$1>"

regexpreplace "<div class=\"ttl-info ellip\">Release: <a href=\"([^\"]*)\" title=\"([^\"]*)\">([^<]*)<\/a> <\/div>" "<AlbumURL=$1> <AlbumName=$3>"

regexpreplace "<h1 class=\"title\">([^<]*)<\/h1>" "<TrackName=$1>"
regexpreplace "<h1 class=\"version\">([^<]*)<\/h1>" "<TrackMixName=$1>"
#regexpreplace "<span class=\"version\">([^<]*)<span class=\"duration\">\(([^\)]*)\)<\/span>" "<TrackMixName=$1><TrackLength=$2>"
regexpreplace "<td>Length:<\/td><td class=\"det\">([^<]*)<\/td>" "<TrackLength=$1>"
regexpreplace "<a href=\"[^\"]*\" class=\"com-artists\" data-aid=\"([^\"]*)\">([^<]*)<\/a>" "<TrackArtistID=$1> <TrackArtistName=$2>"
regexpreplace "class=\"com-remixers\" data-aid=\"([^\"]*)\">([^<]*)<\/a>" "<TrackRemixerID=$1> <TrackRemixerName=$2>"
regexpreplace "<td>Label:<\/td><td class=\"det\"><a href=\"\/label\/[^\"]*\">([^<]*)<\/a><\/td>" "<AlbumLabel=$1>"
regexpreplace "<td>Key:<\/td><td class=\"det\">([^<]*)<\/td>" "<TrackKey=$1>"
regexpreplace "<td>BPM:<\/td><td class=\"det\">([^<]*)<\/td>" "<TrackBpm=$1>"
regexpreplace "<td>Genre:<\/td><td class=\"det\"><a href=\"[^\"]*\">([^<]*)<\/a><\/td>" "<TrackGenre=$1>"
regexpreplace "<td>Released:<\/td><td class=\"det\">([^<]*)<\/td>" "<TrackDate=$1>"
regexpreplace "<td>Released:<\/td><td class=\"det\"><span class='pre-ord-txt'>Pre-order for\s+([^<]*)<\/span><\/td>" "<TrackDate=$1>"
regexpreplace "<div class=\"trk-cell r-date\"><span class='pre-ord-txt'>Pre-order for\s+([^<]*)<\/span><\/div>" "<TrackDate=$1>"
regexpreplace "class=\"com-buy\" data-cart=\"\{title_id: ([^,]*), track_id: ([^\}]*)\}\"" "<TrackTitleID1=$1> <TrackTrackID1=$2>"

regexpreplace "<TrackMixName=\s*>" "<TrackMixName=>"
regexpreplace "<TrackMixName=([^>]+)\s*>" "<TrackMixName=$1>"

#
# Main Mix >> original
# regexpreplace "<TrackMixName=([^>]*)Main Mix\s*>" "<TrackMixName=$1original>"
#
# Instrumental Mix >> instrumental
# regexpreplace "<TrackMixName=([^>]*)Instrumental Mix\s*>" "<TrackMixName=$1instrumental>"
#
# rework >> remix
# regexpreplace "<TrackMixName=([^>]*)rework\s*>" "<TrackMixName=$1remix>"
#
# Vocal Mix >> original
# regexpreplace "<TrackMixName=([^>]*)Original Vocal Mix\s*>" "<TrackMixName=>$1original>"
#
# Extended Version >> extended
# regexpreplace "<TrackMixName=([^>]*)Extended Version\s*>" "<TrackMixName=$1extended>"
#
# Dub Mix >> dub
# regexpreplace "<TrackMixName=([^>]*)Dub Mix\s*>" "<TrackMixName=$1dub>"
#
# Extended Mix >> extended
# regexpreplace "<TrackMixName=([^>]*)Extended Mix\s*>" "<TrackMixName=$1extended>"
#
# Original Mix >> original
# regexpreplace "<TrackMixName=([^>]*)Original Mix\s*>" "<TrackMixName=$1original>"
#
# Club Mix original >> Club Mix
# regexpreplace "<TrackMixName=([^>]*)Club Mix original\s*>" "<TrackMixName=$1Club Mix>"
#
# original Club Mix >> Club Mix
# regexpreplace "<TrackMixName=>([^>]*)original Club Mix\s*>" "<TrackMixName=$1Club Mix>"
#
# Extended Mix original >> extended
# regexpreplace "<TrackMixName=([^>]*)Extended Mix original\s*>" "<TrackMixName=$1extended>"
#
# Dub Mix original >> dub
# regexpreplace "<TrackMixName=([^>]*)Dub Mix original\s*>" "<TrackMixName=$1dub>"
#
# Instrumental Mix original >> instrumental
# regexpreplace "<TrackMixName=([^>]*)Instrumental Mix original\s*+>" "<TrackMixName=$1instrumental>"
#
# Remix original >> remix
# regexpreplace "<TrackMixName=([^>]*)Remix original\s*>" "<TrackMixName=$1 remix>"
#
# Remix >> remix
# regexpreplace "<TrackMixName=([^>]*)Remix\s*>" "<TrackMixName=$1remix>"


replace " feat. " ", feat. "

replace "\\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"
regexpreplace "<div class=\"search-pager\">" "<<endTrack>>"
regexpreplace "<TrackMixName=>" "<TrackMixName=original>"

# replace original version to your liking
#regexpreplace "<TrackMixName=original>" "<TrackMixName=original mix>"
#regexpreplace "<TrackMixName=extended>" "<TrackMixName=extended mix>"
#regexpreplace "<TrackMixName=dub>" "<TrackMixName=dub mix>"
#regexpreplace "<TrackMixName=instrumental>" "<TrackMixName=instrumental mix>"




#::::::::::::::#
# HTML PArsing #
#::::::::::::::#

# DebugWriteInput "c:\debug-input.out"
# debug "on"  "c:\Traxsource track search debug 2 index.html" 20


outputto "COVERURL"
sayregexp "(?<=<AlbumImageURL=)[^>]*"
outputto "ALBUMARTIST"
sayregexp "(?<=<TrackArtistName=)[^>]*" ", " # Artist/S
outputto "ARTIST"
sayregexp "(?<=<TrackArtistName=)[^>]*" ", " # Artist/S
outputto "LENGTH"
sayregexp "(?<=<TrackLength=)[^>]*"
outputto "TITLE"
sayregexp "(?<=<TrackName=)[^>]*"
say " ("
sayregexp "(?<=<TrackMixName=)[^>]*"
say ")"
outputto "PUBLISHER"
sayregexp "(?<=<AlbumLabel=)[^>]*"
outputto "MIXARTIST"
sayregexp "(?<=<TrackRemixerName=)[^>]*" ", " # Remixer/S
outputto "BPM"
sayregexp "(?<=<TrackBpm=)[^>]*"
outputto "INITIALKEY"
sayregexp "(?<=<TrackKey=)[^>]*"
outputto "GENRE"
sayregexp "(?<=<TrackGenre=)[^>]*"
outputto "TRAXSOURCE_TRACKID"
sayregexp "(?<=<TrackTrackID1=)[^>]*"
IfNotOutput "TRAXSOURCE_TRACKID"
sayregexp "(?<=<TrackTrackID2=)[^>]*"
endif
outputto "TRAXSOURCE_TITLEID"
sayregexp "(?<=<TrackTitleID1=)[^>]*"
IfNotOutput "TRAXSOURCE_TITLEID"
sayregexp "(?<=<TrackTitleID2=)[^>]*"
endif
outputto "COVERURL"
sayregexp "(?<=<AlbumURL=)[^>]*"
outputto "ALBUM"
sayregexp "(?<=<AlbumName=)[^>]*"
outputto "YEAR"
sayregexp "(?<=<TrackDate=)[^>]*"
outputto "COMMENT"
say "(Traxsourced)"


#remove dashes below if you don't want to set the field

# set "LENGTH"
# set "ALBUM"
# set "ALBUMARTIST"
# set "ARTIST"
# set "TRAXSOURCE_TITLEID"
# set "TRAXSOURCE_TRACKID"
# set "BPM"
# set "COMMENT"
# set "COMPILATION"
# set "COVERURL"
# set "GENRE"
# set "INITIALKEY"
# set "MIXARTIST"
# set "PUBLISHER"             # RECORD LABEL
# set "TITLE"
# set "TRACK"
# set "YEAR"