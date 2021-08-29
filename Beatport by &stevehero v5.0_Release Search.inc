#    _                   _                        _
#   | |                 | |                      | |
#   | |__    ___   __ _ | |_  _ __    ___   _ __ | |_     ___  ___   _ __ ___
#   | '_ \  / _ \ / _` || __|| '_ \  / _ \ | '__|| __|   / __|/ _ \ | '_ ` _ \
#   | |_) ||  __/| (_| || |_ | |_) || (_) || |   | |_  _| (__| (_) || | | | | |
#   |_.__/  \___| \__,_| \__|| .__/  \___/ |_|    \__|(_)\___|\___/ |_| |_| |_| _
#                    (_)     | | |   | |                 | |                   | |
#    ___   ___  _ __  _  _ __|_| |_  | |__   _   _   ___ | |_  ___ __   __ ___ | |__    ___  _ __  ___ TM
#   / __| / __|| '__|| || '_ \ | __| | '_ \ | | | | / __|| __|/ _ \\ \ / // _ \| '_ \  / _ \| '__|/ _ \
#   \__ \| (__ | |   | || |_) || |_  | |_) || |_| | \__ \| |_|  __/ \ V /|  __/| | | ||  __/| |  | (_) |
#   |___/ \___||_|   |_|| .__/  \__| |_.__/  \__, | |___/ \__|\___|  \_/  \___||_| |_| \___||_|   \___/
#                       | |                   __/ |
#                       |_|                  |___/
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                 .:: [v5.12 by stevehero™] [2021.06.08] ::.                              #
# https://community.mp3tag.de/t/beatport-com-wss-by-stevehero-release-single-track-artwork-tagging/12568  #
#                          .:: Please read the 'Version History + Readme' file ::.                        #
#                    .:: DO NOT COPY/MODIFY OR REDISTRIBUTE THIS WITHOUT PERMISSION ::.                   #
#                 .:: This file should be stored in your tag sources sources directory ::.                #
#                                      %APPDATA%\Mp3tag\data\sources                                      #
#                                  and requires Mp3tag v3.03d or above.                                   #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                          .:: You can buy me a beer or an EP on beatport ::.                             #
#     https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=EZVJN8ZLEU8KC&source=url       #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# TO DO: (WSF Help: https://help.mp3tag.de/main_online.html)
# 1. Leading zero user option on track no.
# 2. TRACK/TOTALTRACKS to fix. e.g. 1/10, 2/10, 3/10 etc.
# 3. Once the json_select_many "artists" "name" " ● " "5" thing has been implemented. Change back to the Original IndexFormat and add that.

[Name]=beatport.com [v5.12 by stevehero™] (◣_◢)
[BasedOn]=https://www.beatport.com
[IndexUrl]=https://www.beatport.com/search/releases?q=
[AlbumUrl]=
[WordSeparator]=+
# [IndexFormat]=% ↓ TYPE%|% ↓ EXCLUSIVE%|% ↓ ID (◣_◢)™%|% ↓ PRICE%|% ↓ RECORD LABEL%|% ↓ RELEASED%|% ↓ CATALOG%|% ↓ RELEASE%|% ↓ GENRE/S%|% ↓ A #%|% ↓ ARTIST/S%|% ↓ R #%|% ↓ REMIXER/S%|%_URL%
[IndexFormat]=% ↓ TYPE%|% ↓ EXCLUSIVE%|% ↓ ID (◣_◢)™%|% ↓ PRICE%|% ↓ RECORD LABEL%|% ↓ RELEASED%|% ↓ CATALOG%|% ↓ RELEASE%|% ↓ ARTIST/S%|% ↓ REMIXER/S%|% ↓ GENRE/S%|%_URL%
[Encoding]=url-utf-8

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[ParserScriptIndex]=...

# debug "on"  "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT debug index.html" 20 # Place before headings to want to debug
# debug "off" "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT debug index.html"    # Place after headings to want to debug (OPTIONAL)

findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</footer>"
replace "|" "$verticalBar()"
regexpreplace "(?i).*?window\.Playables\s*?=\s*?(\{.+);.*?window\.Sliders\s+=\s+\[.*" "$1"
regexpreplace "(\"exclusive\":\s+)(true\,)" "$1 "✓","
regexpreplace "(\"exclusive\":\s+)(false\,)" "$1 "╳","
regexpreplace "(\"type\":\s*\")([^\"]+)" "$1\u$2" # Upper case first letter for release type
regexpreplace "(\"artists\":\s+\[(?:{\"id\"[^\}]+\},\s){1,2}{\"id\"[^\}]+\},\s).+?(\])" "$1{\"id\": 0, \"name\": \" + others...\", \"slug\": \"no-mas\"}$2" # Limits the result of this array length to 3.
regexpreplace "(\"remixers\":\s+\[(?:{\"id\"[^\}]+\},\s){1,2}{\"id\"[^\}]+\},\s).+?(\])" "$1{\"id\": 0, \"name\": \" + others...\", \"slug\": \"no-mas\"}$2" # Limits the result of this array length to 3.
regexpreplace "(\"genres\":\s+\[(?:{\"id\"[^\}]+\},\s){1,2}{\"id\"[^\}]+\},\s).+?(\])" "$1{\"id\": 0, \"name\": \" + others...\", \"slug\": \"no-mas\"}$2" # Limits the result of this array length to 3.

json "ON" "current"

json_foreach "releases"
json_select "type" # TYPE
sayrest
say "|"
json_select "exclusive" # EXCLUSIVE
sayrest
say "|"
json_select "id" # ID
sayrest
say "|"
json_select_object "price" # PRICE
json_select "display"
sayrest
json_unselect_object
say "|"
json_select_object "label" # LABEL
json_select "name"
sayrest
json_unselect_object
say "|"
json_select_object "date" # RELEASED
json_select "released"
sayrest
json_unselect_object
say "|"
json_select "catalog" # CATALOG
sayrest
say "|"
json_select "name" # RELEASES
sayrest
say "|"
# json_foreach "artists" # ARTIST COUNT
# sayrest
# json_foreach_end
# say "|"
json_select_many "artists" "name" " ● " # ARTIST/S
sayrest
say "|"
# json_foreach "remixers" # REMIXERS COUNT
# ifnot "0"
# sayrest
# else
# say "-"
# endif
# json_foreach_end
# say "|"
json_select_many "remixers" "name" " ● " # REMIXERS/S
ifnot ""
sayrest
else
say "-"
endif
say "|"
json_select_many "genres" "name" " ● " # GENRES/S
sayrest
say "|"
say "https://www.beatport.com/release/" # _URL
json_select "slug"
sayrest
json_select "id"
say "/"
sayrest
saynewline
json_foreach_end

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[ParserScriptAlbum]=...

# debug "on"  "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT debug album.html" 20 # Place before headings to want to debug
# debug "off" "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT debug album.html"    # Place after headings to want to debug (OPTIONAL)

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                   .:: P A R S I N G   S E T U P ::.                                     #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

gotoline 5
findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</footer>"
replace "|" "$verticalBar()"
regexpreplace "(\"exclusive\":\s+)(true\,)" "$1 "✓","
regexpreplace "(\"exclusive\":\s+)(false\,)" "$1 "╳","
regexpreplace "(\"type\":\s*\")([^\"]+)" "$1\u$2"                                            # Upper case first letter for release type
regexpreplace "(\"description\":\s*\")(\s*\")" "$1No description.$2"                         # Upper case first letter for release type
regexpreplace "\"tracks\": ?\[\s*\]\," ""                                                    # Remove empty track array, IMPORTANT, DO NOT REMOVE!
regexpreplace "(\"id\": (\d+), )(\"name\": \"[^\"]+\", )(\"slug\": \"([^\"]+)\")" "\"artist_url_stevehero_custom\": \"https://www.beatport.com/artist/$5/$2\", $1$3$4" # Fix #2 for BEATPORT_ARTIST_URL to add a new element for the url, IMPORTANT, DO NOT REMOVE!
regexpreplace "(\"released\": )(\"\d+-\d+-\d+\")" "$1$2, \"releaseddate\": $2"               # Create a new item to be parsed for the date field.

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# TAG FIXES + OPTIONS

# Old regexs brought over from classic beatport. To add should www.beatport.com have the same issues.
# regexpreplace "(,\"key\":)null,\"" "$1\"shortName\":\"╳\"},\""                               # Missing key
# regexpreplace "(?i)(\",\"title\":\".+?)(\s\[Bonus Track\])(.{1,75})(\",\"slug)" "$1$3$2$4"   # Title [Bonus Track] feat. artist (Remixer) => Title feat. artist (Remixer) [Bonus Track]
# regexpreplace "(?i)\(([^\"]{1,40})\s(feat[^\"]{1,40})Remix\s*\)" "$2($1)"                    # Title (artist Remix feat. Remixer Remix) => Title feat. artist (Remixer)
# regexpreplace ",\"name(\":\"[^\"]+\",\"slug.{1,50}artist\")" ",\"artistName$1"               # Artist/s
# regexpreplace ",\"name(\":\"[^\"]+\",\"slug.{1,50}remixer\")" ",\"remixerName$1"             # Remixer/s
# replace ",\"bpm\":0," ",\"bpm\":╳,"                                                          # Zero BPM fix

regexpreplace "(?i) \bep\b" " EP"                                                            # Fix Ep to EP
regexpreplace "(?i) Feat\. " " feat. "                                                       # Fix to lower case
regexpreplace "(?i)(\(|\[)(feat\.\s.+?)(\]|\))" "$2"                                         # Title (feat. artist) => Title feat. artist
regexpreplace "(?i)(\s*(Remix|Rework)\)\s*)\(Remix\)\s*" "$1"                                # Title (Rework) Remix & Title (Remix) Remix
regexpreplace "(?i)(\w)(\(|\[)feat\.\s+(.+?)(\]|\))" "$1 feat. $3"                           # Title(Feat. artist) => Title feat. artist
regexpreplace "(?i)(feat\.\s+[^\"]{1,40})\s+feat\." "$1,"                                    # Title feat. artist feat. artist' => 'Title feat. artist, artist
regexpreplace "(?i)\s+f(ea)?t\.*\s+" " feat. "                                               # 'ft. => feat.' 'Ft. => feat.' 'Feat. => feat.'
regexpreplace "\(+" "("
regexpreplace "\)+" ")"

regexpreplace "(?i)(\"mix\":) \"\s*\(Original Mix\)\s*\(Original Mix\)\s*\"" "$1 \"Original Mix\""  # Fix double '(Original Mix)'.
regexpreplace "(?i)(\"mix\":) \"\s*\(Original\)\s*\s*\"" "$1 \"Original Mix\""                      # Fix '(Original)'.
regexpreplace "(?i)(\"mix\":) \"\(([^\"]+?)\)\"" "$1 \"$2\""                                        # Fixed the extra ( and ) in the mix json
replace "\"Original Mix\"" "\"\""                                                                   # Replace "" to your liking, block this with # to keep the original setting
replace " feat. " " feat. "                                                                         # Here you can change the way you want 'feat.' formatted
replace " Remix) (Original Mix)" " Remix)"                                                          # Fix ' Remix (Original Mix)'
replace "\"type\":\"track\"" "\"type\":\"Track\""                                                   # Capital letter
replace "\/" "/"                                                                                    # URL fixes

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# TRACK OPTIONS
# regexpreplace "(txt-grey\">\s*)(\d)( <)" "$1 0$2$3"                                      # Add leading zeros to TRACK

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# INITIALKEY TO CAMELOT NOTATION
# Please DO NOT change values
# You can delete all these replace lines below to get 'Dmin' etc.

replace "\u266d" "♭"             # Fix
replace "\u266f" "#"             # Fix

replace "\"G# min\"" "\"01A\""
replace "\"A♭ min\"" "\"01A\""   # SAME KEY #
replace "\"D# min\"" "\"02A\""
replace "\"E♭ min\"" "\"02A\""   # SAME KEY #
replace "\"A# min\"" "\"03A\""
replace "\"B♭ min\"" "\"03A\""   # SAME KEY #
replace "\"F min\"" "\"04A\""
replace "\"C min\"" "\"05A\""
replace "\"G min\"" "\"06A\""
replace "\"D min\"" "\"07A\""
replace "\"A min\"" "\"08A\""
replace "\"E min\"" "\"09A\""
replace "\"B min\"" "\"10A\""
replace "\"F# min\"" "\"11A\""
replace "\"G♭ min\"" "\"11A\""
replace "\"C# min\"" "\"12A\""
replace "\"D♭ min\"" "\"12A\""   # SAME KEY #
replace "\"B maj\"" "\"01B\""
replace "\"F# maj\"" "\"02B\""
replace "\"G♭ maj\"" "\"02B\""   # SAME KEY #
replace "\"C# maj\"" "\"03B\""
replace "\"D♭ maj\"" "\"03B\""   # SAME KEY #
replace "\"G# maj\"" "\"04B\""
replace "\"A♭ maj\"" "\"04B\""   # SAME KEY #
replace "\"D# maj\"" "\"05B\""
replace "\"E♭ maj\"" "\"05B\""   # SAME KEY #
replace "\"A# maj\"" "\"06B\""
replace "\"B♭ maj\"" "\"06B\""   # SAME KEY #
replace "\"F maj\"" "\"07B\""
replace "\"C maj\"" "\"08B\""
replace "\"G maj\"" "\"09B\""
replace "\"D maj\"" "\"10B\""
replace "\"A maj\"" "\"11B\""
replace "\"E maj\"" "\"12B\""

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# GENRE
# regexpreplace ",\"genres\":\[.+?name\":\"([^\"]+)[^]]+\]" "trackGenre>>$1>"              # Fix
# regexpreplace "(trackGenre>>)(Dee|Ele|Min|Pro|Tec)(.+?)\s?(House)?.{0}>" "$1$4 $2$3>"    # 'Progressive House' to 'House Progressive'
# replace "trackGenre>>House Progressive>" "trackGenre>>House Prog>"                       # 'Progressive House' to 'House Prog'

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                      .:: U S E R   O P T I O N S ::.                                    #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# DATE OPTION BELOW (Defaults to YYYY-MM-DD so leave this untouched for this output)
# Only leave one of these line below uncommented with no # preceding the line
regexpreplace "(releaseddate\":\s*\")(\d\d\d\d)-(\d\d)-(\d\d)" "$1$3$4"     # DATE in MMDD format
# regexpreplace "(releaseddate\":\s*\")(\d\d\d\d)-(\d\d)-(\d\d)" "$1$4$3"   # DATE in DDMM format

# YEAR OPTION BELOW (Defaults to YYYY-MM-DD so leave this untouched for this output)
# Only leave one of these line below uncommented with no # preceding the line
# regexpreplace "(released\":\s*\")(\d\d\d\d)-(\d\d)-(\d\d)" "$1$2-$4-$3"   # YEAR in YYYY-DD-MM format
# regexpreplace "(released\":\s*\")(\d\d\d\d)-(\d\d)-(\d\d)" "$1$4-$3-$2"   # YEAR in DD-MM-YYYY format
# regexpreplace "(released\":\s*\")(\d\d\d\d)-(\d\d)-(\d\d)" "$1$2"         # YEAR in YYYY format

# TOTALTRACKS OPTION BELOW
# regexpreplace "(ec-item track\" data-ec-position=\")(\d)(\")" "$1 0$2$3"    # Add leading zeros to TOTALTRACKS

# TRACK OPTION BELOW
# regexpreplace "(ec-item track\" data-ec-position=\")(\d)(\")" "$1 0$2$3"    # Add leading zeros to TRACK

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                   .:: H T M L    P A R S I N G ::.                                      #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

outputto "_TIME_CHECK"  # shows track length when only single track available, convenient to check time comparison
findinline "<!-- END LOADING OVERLAY -->" 1 1
findinline "<li class=\"bucket-item ec-item track\" data-ec-position=\"" -1 1
findinline "position=" 1 1
if "\"1\""
gotochar 1
findinline "<p class=\"buk-track-length\">" 1 1
sayuntil "<"
endif

outputto "TOTALTRACKS"
gotochar 1
findinline "<!-- END LOADING OVERLAY -->" 1 1
findinline "<li class=\"bucket-item ec-item track\" data-ec-position=\"" -1 1
findinline "position=\"" 1 1
sayuntil "\""

outputto "ALBUMARTIST"
gotochar 1
findinline "<ul class=\"interior-release-chart-content-list\">" 1 1
findinline "<span class=\"value\">" 1 1
if "Various Artists"
say "VA"
outputto "COMPILATION"
say "1"
else
outputto "COMPILATION"
say "0"
endif

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                   .:: J S O N    P A R S I N G ::.                                      #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# This will remove the last of the unwanted html code from being parsed, leave this before the start json parsing code
regexpreplace "(?i).*?window\.Playables\s*?=\s*?(\{.+);.*?window\.Sliders\s+=\s+\[.*" "$1"

json "ON" "current"
json_select_array "releases" 1
outputto "ALBUM"
json_select "name"
sayrest
outputto "ALBUMARTIST"
ifnotoutput "ALBUMARTIST"
json_select_many "artists" "name" ", "
sayrest
endif
outputto "BEATPORT_LABEL_URL"
say "https://www.beatport.com/label/"
json_select_object "label"
json_select "slug"
sayrest
json_select "id"
say "/"
json_unselect_object
sayrest
outputto "BEATPORT_RELEASE_ID"
json_select "id"
sayrest
outputto "CATALOG #"
json_select "catalog"
sayrest
outputto "COVERURL"
json_select_object "images"
json_select_object "large"
json_select "url"
regexpreplace "image_size\/\d+x\d+" "image"
json_unselect_object
json_unselect_object
sayrest
outputto "DATE"
json_select_object "date"
json_select "releaseddate"
json_unselect_object
sayrest
outputto "PUBLISHER"
json_select_object "label"
json_select "name"
json_unselect_object
sayrest
outputto "YEAR"
json_select_object "date"
json_select "released"
json_unselect_object
sayrest
outputto "UNSYNCEDLYRICS"
say "Release type:\\u0009\\u0009"
json_select "type"
sayrest
say "\\u000d\\u000a----------------------------------------------\\u000d\\u000a"
say "Exclusive to beatport:\\u0009"
json_select "exclusive"
sayrest
json_select "description"
say "\\u000d\\u000a----------------------------------------------\\u000d\\u000a"
sayrest
say "\\u000d\\u000a----------------------------------------------\\u000d\\u000a"
say "Tagged by:\\u0009\\u0009Mp3Tag w/ beatport.com scripts [v5.12 by stevehero™] (◣_◢) (http://bit.ly/2EmyidV)"
outputto "WWW"
json_select "slug"
say "https://www.beatport.com/release/"
sayrest
json_select "id"
say "/"
sayrest
json_unselect_object # Deselect releases array

json_foreach "tracks"
json_select "component"
if "Release Track Listing"

# outputto "TRACKTEMP" # TRACK/TOTALTRACK|TRACKTEMP
# # sayregexp "(?<=txt-grey\"> )[^<]+" "" "<<endTrack>>"
# sayrest
# # The bottom two lines control whether or not you have the TOTALTRACKS appended. e.g. 1/10, 2/10, 3/10 etc.
# # say "/"
# # sayoutput "TOTALTRACKS"
# say "|"

outputto "BEATPORT_ITEM_TYPE"
json_select "type"
sayrest
say "|"

outputto "BEATPORT_ARTIST_URL"
json_select_many "artists" "artist_url_stevehero_custom" "\\u005c\\u005c"
sayrest
say "|"

outputto "TITLE" # TITLE|TITLETEMP
json_select "name"
sayrest
json_select "mix"
ifnot ""
say " ("
sayrest
say ")"
endif
say "|"

outputto "ARTIST"
json_select_many "artists" "name" ", "
sayrest
say "|"

outputto "MIXARTIST"
json_select_many "remixers" "name" ", "
sayrest
say "|"

outputto "_LENGTH"
json_select_object "duration"
json_select "minutes"
sayrest
say "|"
json_unselect_object

outputto "BPM"
json_select "bpm"
sayrest
say "|"

outputto "INITIALKEY"
json_select "key"
sayrest
say "|"

outputto "GENRE"
json_select_many "genres" "name" ", "
sayrest
say "|"

outputto "BEATPORT_TRACK_ID"
json_select "id"
sayrest
say "|"

outputto "BEATPORT_TRACK_URL"
say "https://www.beatport.com/track/"
json_select "slug"
sayrest
say "/"
json_select "id"
sayrest
say "|"

# outputto "TRACKTEMP"
# say "|"

outputto "TRACKS"
say "|"

endif
json_foreach_end
json_unselect_object
json_unselect_object # Deselect tracks array

# TRACK COUNTER FIX ** DO NOT DELETE
# outputto "TITLE"
# sayoutput "TITLETEMP"
# set "TITLETEMP"
# outputto "TRACK"
# sayoutput "TRACKTEMP"
# set "TRACKTEMP"

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                            .:: T A G    C U S T O M I Z A T I O N ::.                                   #
# • You can add your own web source scripting here                                                        #
# • ONLY disable the ALBUM [CATALOG #] here                                                               #
# • Disable the other fields in the .:: R E M O V E    F I E L D S ::. section below                      #
# • To disable place a "#" in front of each line of code between the headings. e.g.                       #
# ALBUM [CATALOG #]                                                                                       #
  # outputto "ALBUM"                                                                                      #
  # say " ["                                                                                              #
  # sayoutput "CATALOG #"                                                                                 #
  # say "]"                                                                                               #
  # COMMENT                                                                                               #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# ALBUM ARTIST
outputto "ALBUM ARTIST"
sayoutput "ALBUMARTIST"
# ALBUM [CATOLOG #]
outputto "ALBUM"
say " ["
sayoutput "CATALOG #"
say "]"
# COMMENT
outputto "COMMENT"
sayoutput "INITIALKEY"
# DISCNUMBER
outputto "DISCNUMBER"
say "1"
# MEDIATYPE
outputto "MEDIATYPE"
say "BEATPORT STORE"
# RELEASETIME
outputto "RELEASETIME"
sayoutput "YEAR"
say "T00:00:00Z"
# DISCTOTAL
outputto "DISCTOTAL"
say "1"
# MEDIATYPE_ORIGINAL
outputto "MEDIATYPE_ORIGINAL"
say "WEB"
# STYLE
outputto "STYLE"
say "Electronic"

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                .:: R E M O V E    F I E L D S ::.                                       #
# • To remove fields from parsing delete the # in front of 'set'                                          #
# • Only use this section to remove fields                                                                #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# set "_LENGTH"
# set "_TIME_CHECK"
# set "ALBUM"
# set "ALBUMARTIST"          # ITUNES USES
# set "ARTIST"
# set "BEATPORT_ARTIST_URL"
# set "BEATPORT_ITEM_TYPE"
# set "BEATPORT_LABEL_URL"
# set "BEATPORT_RELEASE_ID"
# set "BEATPORT_TRACK_ID"
# set "BEATPORT_TRACK_URL"
# set "BPM"
# set "CATALOG #"
# set "COMPILATION"
# set "COVERURL"
# set "DISCNUMBER"           # EXTRA FIELD
# set "DISCTOTAL"            # EXTRA FIELD
# set "GENRE"
# set "MEDIATYPE"            # EXTRA FIELD
# set "MEDIATYPE_ORIGINAL"   # EXTRA FIELD
# set "MIXARTIST"
# set "PUBLISHER"            # RECORD LABEL
# set "STYLE"
# set "TITLE"
# set "TOTALTRACKS"          # EXTRA FIELD
# set "TRACK"                # NO'S OF TRACKS E.G. 01/06 (if set goes back to default 1, 2, 3... etc)
# set "TRACKS"               # REMOVES TRACKs (USEFUL FOR WHEN ONLY REQUIRING COVERART SO NO TRACK NUMBERS APPEAR IN ADJUST TAG INFO DIALOG)
# set "UNSYNCEDLYRICS"       # DESCRIPTION ON BEATPORT
# set "WWW"
# set "YEAR"

# SET BY DEFAULT (My own personal preferences)
set "ALBUM ARTIST"           # EXTRA FIELD, ALBUM ARTIST USED IN VLC, FOOBAR 2000 AND OTHERS PLEASE CHECK YOUR MUSIC PLAYER FIRST
set "COMMENT"                # EXTRA FIELD
set "DATE"
set "INITIALKEY"
set "RELEASETIME"

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                       .:: T H E E    E N D ::.                                          #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#    _                   _                        _
#   | |                 | |                      | |
#   | |__    ___   __ _ | |_  _ __    ___   _ __ | |_     ___  ___   _ __ ___
#   | '_ \  / _ \ / _` || __|| '_ \  / _ \ | '__|| __|   / __|/ _ \ | '_ ` _ \
#   | |_) ||  __/| (_| || |_ | |_) || (_) || |   | |_  _| (__| (_) || | | | | |
#   |_.__/  \___| \__,_| \__|| .__/  \___/ |_|    \__|(_)\___|\___/ |_| |_| |_| _
#                    (_)     | | |   | |                 | |                   | |
#    ___   ___  _ __  _  _ __|_| |_  | |__   _   _   ___ | |_  ___ __   __ ___ | |__    ___  _ __  ___ TM
#   / __| / __|| '__|| || '_ \ | __| | '_ \ | | | | / __|| __|/ _ \\ \ / // _ \| '_ \  / _ \| '__|/ _ \
#   \__ \| (__ | |   | || |_) || |_  | |_) || |_| | \__ \| |_|  __/ \ V /|  __/| | | ||  __/| |  | (_) |
#   |___/ \___||_|   |_|| .__/  \__| |_.__/  \__, | |___/ \__|\___|  \_/  \___||_| |_| \___||_|   \___/
#                       | |                   __/ |
#                       |_|                  |___/