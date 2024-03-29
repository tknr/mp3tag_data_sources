﻿[Name]=Traxsource RELEASE search
[BasedOn]=https://www.traxsource.com
[IndexUrl]=https://www.traxsource.com/search/titles?term=%s
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=% ↓ TitleID%|% ↓ Release%|% ↓ ARTIST/S%|% ↓ Label%|%_URL%
[Encoding]=url-utf-8

# V0.3
# Mp3tag v3.22+ is required

#######################
[ParserScriptIndex]=...

debug "on"  "c:\Traxsource release debug index.html" 20
DebugWriteInput "c:\debug-input.out"

findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace

regexpreplace "<div class=\"search-pager\">" "<<endTrack>>"
regexpreplace ">\s+<" "><"
regexpreplace "\s+" " "
regexpreplace "\(+" "("
regexpreplace "\)+" ")"
regexpreplace "(\([^\(|\)]{1,100})\s+(\()" "$1) ("
regexpreplace "<div class=\"grid-item[^\"]+\" data-tid=\"([^\"]+)\">" "<<endTrack>><<nextTrack>><TrackTitleID=$1>"
regexpreplace "<div class=\"links ellip\"><a href=\"([^\"]+)\" class=\"com-title\">([^<]+)<\/a>" "<AlbumURL=$1><AlbumName=$2>"
regexpreplace "<AlbumName=([^<]+)>\s*([^<]+)\s*<div><a href=\"([^\"]+)\" class=\"com-label\">([^<]+)<\/a><\/div>" "<AlbumName=$1><AlbumArtist=$2><AlbumLabelURL=$3><AlbumLabel=$4>"

replace "\\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"

#:::::::::::::#


do

findinline "<<nextTrack>>"

	sayregexp "(?<=<TrackTitleID=)[^>]+" "" "<<endTrack>>"# TRACK ID
	say "|"
	sayregexp "(?<=<AlbumName=)[^>]+" "" "<<endTrack>>"# TITLE
	say "|"
	sayregexp "(?<=<AlbumArtist=)[^>]+" " " "<<endTrack>>" # ARTIST/S
    	say "|"
	sayregexp "(?<=<AlbumLabel=)[^>]+" " " "<<endTrack>>" # Label
	say "|"
	say "https://www.traxsource.com"
 	sayregexp "(?<=<AlbumURL=)[^>]+" "" "<<endTrack>>" # URL

saynewline
findinline "<<endTrack>>" 1 1
while "<<nextTrack>>" 99

#::::::::::::::#
[ParserScriptAlbum]=...
#::::::::::::::#
#DebugWriteInput "c:\debug-input.out"

#debug "on"  "c:\Traxsource Release direct debug index.html"


findline "<!DOCTYPE html>"
joinuntil "</html>"
replace "|" "$verticalBar()"
unspace


regexpreplace "class=\"com-buy\" data-cart=\"{title_id:\s(\d+)}\" title=\"Add to Cart\"><span class=\"price\">([^<]+)<\/span>" "<TitleID=$1> <TitlePrice=$2>"

regexpreplace "<meta property=\"og:image\"\s?content=\"([^\"]+)\">"  "<AlbumImageURL=$1>" "data-tracks="
regexpreplace "<meta property=\"og:url\"\s?content=\"([^\"]+)\">"  "<AlbumURL=$1>" "data-tracks="
regexpreplace "<div id=\"ttlPageCont\" data-tid=\"([^\"]+)\">"  "<TitleID2=$1>" "data-tracks="
regexpreplace "<div class=\"page-head\">\s*<h1 class=\"title\">([^<]+)<\/h1>" "<AlbumName=$1>" "data-tracks="
regexpreplace "<h1 class=\"artists\">[^<]?<a href=\"[^\"]+\" class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)<\/a>" "<AlbumArtistID=$1><AlbumArtist=$2>" "data-tracks="
regexpreplace "<AlbumArtistID=([^<]+)><AlbumArtist=([^<]+)>[^<]*<a href=\"[^\"]+\" class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)" "<AlbumArtistID=$1><AlbumArtist=$2><AlbumArtistID=$3><AlbumArtist=$4>" "data-tracks="
regexpreplace "<a class=\"com-label\"\s*href=\"([^\"]+)\">([^<]+)<\/a>\s*<\/div>" "<AlbumLabelURL=$1><AlbumLabel=$2>" "data-tracks="
regexpreplace "<div class=\"cat-rdate\">([^\s]+)\s*\$verticalBar\(\)\s*([^<]+)<\/div>" "<AlbumCatalogID=$1><AlbumReleaseDate=$2>"
regexpreplace "<div class=\"cat-rdate\">([^\s]+)\s+\$verticalBar\(\)\s+<span class='pre-ord-txt'>Pre-order for\s+([^<]*)<\/span>" "<AlbumCatalogID=$1><AlbumReleaseDate=$2>"

regexpreplace "<span class=\"price\">([^<]+)<\/span>" "<TrackPrice=$1>"

regexpreplace "<div data-trid=\"([^\"]+)\" class=" "<<EndTrack>><<NextTrack>><TrackID=$1>"

regexpreplace "<div class=\"tnum\">(\d)<\/div>" "<TrackNum=0$1>"
regexpreplace "<div class=\"tnum\">(\d\d)<\/div>" "<TrackNum=$1>"
regexpreplace "<!--DIV title--><a href=\"([^\"]+)\">([^<]+)<\/a>" "<TrackURL=$1><TrackName=$2>"
regexpreplace "<a href=\"[^\"]+\" class=\"com-artists\" data-aid=\"([^\"]+)\">([^<]+)<\/a>" "<TrackArtistID=$1> <TrackArtistName=$2>" "<<EndTrack>>"
regexpreplace "<span class=\"version\">([^<]*)<span class=\"duration\">\(([^\)]+)\)<\/span>" "<TrackMixName=$1><TrackLength=$2>" "<!--DIV artLinks-->"

regexpreplace "<!--DIV genre--><a href=\"[^\"]+\">([^<]+)<\/a>" "<TrackGenre=$1>"
regexpreplace "<!--MORE FROM-->" "<<EndTrack>><<EndAlbum>>"

regexpreplace "<TrackMixName=\s*>" "<TrackMixName=>"
regexpreplace "<TrackMixName=([^>]+)\s*>" "<TrackMixName=$1>"

#
# Main Mix >> original
regexpreplace "<TrackMixName=([^>]*)Main Mix\s*>" "<TrackMixName=$1original>"
#
# Instrumental Mix >> instrumental
regexpreplace "<TrackMixName=([^>]*)Instrumental Mix\s*>" "<TrackMixName=$1instrumental>"
#
# rework >> remix
regexpreplace "<TrackMixName=([^>]*)rework\s*>" "<TrackMixName=$1remix>"
#
# Vocal Mix >> original
regexpreplace "<TrackMixName=([^>]*)Original Vocal Mix\s*>" "<TrackMixName=>$1original>"
#
# Extended Version >> extended
regexpreplace "<TrackMixName=([^>]*)Extended Version\s*>" "<TrackMixName=$1extended>"
#
# Dub Mix >> dub
regexpreplace "<TrackMixName=([^>]*)Dub Mix\s*>" "<TrackMixName=$1dub>"
#
# Extended Mix >> extended
regexpreplace "<TrackMixName=([^>]*)Extended Mix\s*>" "<TrackMixName=$1extended>"
#
# Original Mix >> original
regexpreplace "<TrackMixName=([^>]*)Original Mix\s*>" "<TrackMixName=$1original>"
#
# Club Mix original >> Club Mix
regexpreplace "<TrackMixName=([^>]*)Club Mix original\s*>" "<TrackMixName=$1Club Mix>"
#
# original Club Mix >> Club Mix
regexpreplace "<TrackMixName=>([^>]*)original Club Mix\s*>" "<TrackMixName=$1Club Mix>"
#
# Extended Mix original >> extended
regexpreplace "<TrackMixName=([^>]*)Extended Mix original\s*>" "<TrackMixName=$1extended>"
#
# Dub Mix original >> dub
regexpreplace "<TrackMixName=([^>]*)Dub Mix original\s*>" "<TrackMixName=$1dub>"
#
# Instrumental Mix original >> instrumental
regexpreplace "<TrackMixName=([^>]*)Instrumental Mix original\s*+>" "<TrackMixName=$1instrumental>"
#
# Remix original >> remix
regexpreplace "<TrackMixName=([^>]*)Remix original\s*>" "<TrackMixName=$1 remix>"
#
# Remix >> remix
regexpreplace "<TrackMixName=([^>]*)Remix\s*>" "<TrackMixName=$1remix>"


replace " feat. " ", feat. "

replace "\\t" ""
replace "\/" "/"
replace "\\\"" "&quot;"
regexpreplace "<div class=\"search-pager\">" "<<EndTrack>>"
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

# debug "on"  "c:\Traxsource debug index.html" 20




outputto "COVERURL"
sayregexp "(?<=<AlbumImageURL=)[^>]+"
outputto "TRAXSOURCE_TITLEID"
sayregexp "(?<=<TitleID=)[^>]+"
outputto "ALBUM"
sayregexp "(?<=<AlbumName=)[^>]+"
outputto "LABEL"
sayregexp "(?<=<AlbumLabel=)[^>]+"
outputto "YEAR"
sayregexp "(?<=<AlbumReleaseDate=)[^>]+"
# ALBUMARTIST
outputto "ALBUMARTIST"
sayregexp "(?<=<AlbumArtist=)[^>]+" ", "
outputto "WWW"
sayregexp "(?<=<AlbumURL=)[^>]+"
outputto "CATALOG #"
sayregexp "(?<=<AlbumCatalogID=)[^>]+"
outputto "TRAXSOURCE_ALBUMPRICE"
sayregexp "(?<=<TitlePrice=)[^>]+"

outputto "COMMENT"
say "(Traxsourced)"

outputto "TRAXSOURCE_TRACKID"
sayregexp "(?<=<TrackID=)[^>]+" "|" "<<EndAlbum>>"

outputto "TRAXSOURCE_TRACKPRICE"
sayregexp "(?<=<TrackPrice=)[^>]+" "|" "<<EndAlbum>>"

outputto "GENRE"
sayregexp "(?<=<TrackGenre=)[^>]+" "|" "<<EndAlbum>>"

outputto "LENGTH"
sayregexp "(?<=<TrackLength=)[^>]+" "|" "<<EndAlbum>>"

outputto "TrackURL"
say "https://www.traxsource.com"
sayregexp "(?<=<TrackURL=)[^>]+" "|https://www.traxsource.com" "<<EndAlbum>>"

outputto "TRACK"
sayregexp "(?<=<TrackNum=)[^>]+" "|" "<<EndAlbum>>"

set "TOTALTRACKS" ""
outputto "TOTALTRACKS"
sayregexp "(?<=<TrackNum=)[^>]+" "|" "<<EndAlbum>>"


do
findinline "<<NextTrack>>"  1 1

set "TOTALTRACKS" ""
outputto "TOTALTRACKS"
sayregexp "(?<=<TrackNum=)[^>]+" "" "<<EndTrack>>"
say "|"
outputto "TITLE"
sayregexp "(?<=<TrackName=)[^>]+" "" "<<EndTrack>>"
say " ("
sayregexp "(?<=<TrackMixName=)[^>]+" "" "<<EndTrack>>"
say ")"
say "|"
outputto "ARTIST"
sayregexp "(?<=<TrackArtistName=)[^>]+" ", " "<<EndTrack>>"
say "|"
outputto "MIXARTIST"
sayregexp "(?<=<TrackRemixerName=)[^>]+" ", " "<<EndTrack>>"
say "|"

findinline "<<EndTrack>>" 1 1

while "<<NextTrack>>" 99



#remove dashes below if you don't want to set the field

# set "LENGTH"
# set "ALBUM"
# set "TOTALTRACKS"
# set "ALBUMARTIST"
# set "ARTIST"
# set "TRAXSOURCE_TITLEID"
# set "TRAXSOURCE_TRACKID"
 set "TRAXSOURCE_TRACKPRICE"
# set "CATALOG #"
# set "BPM"
# set "COMMENT"
# set "COVERURL"
# set "GENRE"
# set "INITIALKEY"
# set "MIXARTIST"
# set "PUBLISHER"             # RECORD LABEL
# set "TITLE"
# set "TRACK"

# set "YEAR"