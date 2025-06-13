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
#                             .::  [v6.007 by stevehero™] [2024.10.08] ::.                                #
#  https://community.mp3tag.de/t/ws-beatport-com-by-stevehero-release-single-track-artwork-tagging/12568  #
#                          .:: Please read the 'Version History + Readme' file ::.                        #
#                    .:: DO NOT COPY/MODIFY OR REDISTRIBUTE THIS WITHOUT PERMISSION ::.                   #
#                .:: This file should be stored in your tag sources, sources directory ::.                #
#                                      %APPDATA%\Mp3tag\data\sources                                      #
#                          and requires Mp3tag v3.26 [Win] / 1.8.4 [Mac] or above                         #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#       .:: You can buy me a 🍺's, 🍔's or an EP on beatport as a thank you (See link below) ::.         #
#                                    https://revolut.me/stephen147                                        #
#                                    https://revolut.me/stephen147                                        #
#                                    https://revolut.me/stephen147                                        #
#                                    https://revolut.me/stephen147                                        #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# TO DO: NA
# WSF Help: https://help.mp3tag.de/main_online.html

[Name]=beatport.com  [v6.007 by stevehero™] (◣_◢)
[MinAppVersionMac]=1.8.4
[MinAppVersionWin]=3.26
[Settings]=Beatport by &stevehero v6#USER SETTIN&GS.settings
[BasedOn]=https://www.beatport.com
[IndexUrl]=https://www.beatport.com/search/releases?q=
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=%_coverurl%|% ↓ EXCLUSIVE%|% ↓ ID (◣_◢)™%|% ↓ TRACK COUNT%|% ↓ RECORD LABEL%|% ↓ RELEASED%|% ↓ CATALOG%|% ↓ RELEASE NAME%|% ↓ ARTIST/S%|% ↓ REMIXER/S%|% ↓ GENRE/S%|%_URL%
[Encoding]=url-utf-8

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[ParserScriptIndex]=...

# debug "on"  "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT RELEASE ParserScriptIndex.html" # Place before headings to want to debug
# debug "off" "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT RELEASE ParserScriptIndex.html" # Place after headings to want to debug (OPTIONAL)

# Remove non JSON from HTML, leaving only valid JSON (Must be first section of code below [ParserScriptIndex] or [ParserScriptAlbum]
JoinUntil "</html>"
RegexpReplace "<!DOCTYPE\s+.+?json\">" ""          # Remove before JSON
RegexpReplace "</script><noscript>.+?</html>" ""   # Remove after JSON

RegexpReplace "(\")artist_name(\":\s*\"[^\"]*\",\s*\"artist_type_name\":\s*\"Artist\")" "$1actual_artist_name$2" # Gives the a new property for the Artist "actual_artist_name"
RegexpReplace "(\")artist_name(\":\s*\"[^\"]*\",\s*\"artist_type_name\":\s*\"Remixer\")" "$1remixer_artist_name$2" # Gives the a new property for the Remixer "remixer_artist_name"
Replace "Deadmau5" "deadmau5"
Replace "|" "$verticalBar()"

# Turn on JSON for the remainder of the script
json "ON" "current"

# Set it up to go into the data object
json_select_object "props"
json_select_object "pageProps"
json_select_object "dehydratedState"
json_select_array "queries" 1 # Select "queries" 1 array (Album info)
json_select_object "state"
json_select_object "data"

# Loop through the data array
json_foreach "data"

json_select "release_image_uri" # _COVERURL
SayRest
Say "|"

json_select "exclusive" # EXCLUSIVE
Replace "0" "❎"
Replace "1" "✅"
SayRest
Say "|"

json_select "release_id" # ID
SayRest
Say "|"

json_select "track_count" # TRACK COUNT
SayRest
Say "|"

json_select_object "label" # RECORD LABEL
json_select "label_name"
SayRest
json_unselect_object
Say "|"

json_select "release_date" # RELEASED
RegexpReplace "T\d+:\d+:\d+" ""
SayRest
Say "|"

json_select "catalog_number" # CATALOG
SayRest
Say "|"

json_select "release_name" # RELEASE NAME
SayRest
Say "|"

json_select_many_count "artists" "actual_artist_name" # ARTIST/S
IfGreater 3
json_select_many "artists" "actual_artist_name" " ● " " ● " 3
SayRest
Say " & more..."
Else
json_select_many "artists" "actual_artist_name" " ● " " ● "
SayRest
Endif
Say "|"

json_select_many_count "artists" "remixer_artist_name" # REMIXER/S
If "0"
Say "-"
Endif
IfGreater 3
json_select_many "artists" "remixer_artist_name" " ● " " ● " 3
SayRest
Say " & more..."
Else
json_select_many "artists" "remixer_artist_name" " ● " " ● "
SayRest
Endif
Say "|"

json_select_many_count "genre" "genre_name" # GENRE/S
IfGreater 3
json_select_many "genre" "genre_name" " ● " " ● " 3
SayRest
Say " & more..."
Else
json_select_many "genre" "genre_name" " ● " " ● "
SayRest
Endif
Say "|"

Say "https://www.beatport.com/release/releases/" # _URL (REQUIRED FOR PREVIEW BUTTON TO WORK)
json_select "release_id"
SayRest

SayNewline
json_foreach_end

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

[ParserScriptAlbum]=...

# debug "on"  "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT RELEASE ParserScriptAlbum.html" # Place before headings to want to debug
# debug "off" "W:\Apps (Portable)\Music Apps\Mp3tag\data\sources\BEATPORT RELEASE ParserScriptAlbum.html" # Place after headings to want to debug (OPTIONAL)

# Remove non JSON from HTML, leaving only valid JSON (Must be first section of code below [ParserScriptIndex] or [ParserScriptAlbum]
JoinUntil "</html>"
RegexpReplace "<!DOCTYPE\s+.+?json\">" ""          # Remove before JSON
RegexpReplace "</script><noscript>.+?</html>" ""   # Remove after JSON
Replace "|" "$verticalBar()"

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
#                                   .:: J S O N    P A R S I N G ::.                                      #
#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#

# Turn on JSON for the remainder of the script
json "ON" "current"

# Set it up to go into the data object
json_select_object "props"
json_select_object "pageProps"
json_select_object "dehydratedState"
json_select_array "queries" 1 # Select "queries" 1 array (Album info)
json_select_object "state"
json_select_object "data"

OutputTo "COVERURL"
json_select_object "image"
json_select "uri"
SayRest
json_unselect_object

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