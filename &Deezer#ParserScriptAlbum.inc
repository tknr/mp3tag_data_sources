################################################################################
#                        ADJUST TAG INFORMATION DIALOG                         #
################################################################################
[ParserScriptAlbum]=...
## Comment/uncomment for debugging purposes:
# DebugWriteInput "deezer-album-input.out"
# Debug "ON" "deezer-album-debug.out"

# Check for Deezer URL
Use "%CurrentUrl%"
OutputTo "Temp"
SayRegexp "https?://.+?.deezer.com/"
IfNotOutput "Temp"
	OutputTo "Message:"
	Say "No Deezer URL: "
	SayOutput "CurrentURL"
	OutputTo "Fatal:"
	# force abort
	json "ON" "current"
EndIf

# Configuration settings processing
Set "CoverResolution"
OutputTo "CoverResolution"
SayVar "CoverResolution"

# Find lang (currently unused)
OutputTo "Lang"
FindLine "<html lang=" 1 1
SayRegexp "(?<=lang=).+?(?=>|\s)"

# Extract json data
FindLine "<script>window.__DZR_APP_STATE__" 1 1
RegexpReplace "^.*?<script>window.__DZR_APP_STATE__\s*=" ""
RegexpReplace "</script>.*" ""
# Remove json objects not needed
RegexpReplace ",\"ALBUMS\":.*$" "}"
Replace "|" "$verticalBar()"

# Prepare for meaningful error message in case of no JSON data found
if ""
	OutputTo "Message:"
	Say "JSON data not found in "
	SayOutput "CurrentURL"
	OutputTo "Fatal:"
EndIf

# Extract contributing artists json object for later
# using it to build list of roles of involved people
OutputTo "SNG_CONTRIBUTORS"
Say "["
SayRegexp "(?<="SNG_CONTRIBUTORS":)\{.+?\}" ","
Say "]"

json "ON" "current"

## First, parse album data
json_select_object "DATA"

# Deezer Album ID
OutputTo "DEEZER_ALBUM_ID"
json_select "ALB_ID"
SayRest

# Deezer Artist ID
OutputTo "DEEZER_ARTIST_ID"
json_select "ART_ID"
SayRest

# Album URL
OutputTo "WWW"
SayOutput "CurrentUrl"

# Album artist (only main_artist)
OutputTo "ALBUMARTIST"
json_select "__TYPE__"
If "artist"
	json_select "ART_NAME"
	SayRest
Else
	json_select_object "ALB_CONTRIBUTORS"
	IfNot ""
		json_select_array "main_artist" -1 ", "
		SayRest
	json_unselect_object # ALB_CONTRIBUTORS
	EndIf
EndIf

# Publisher
OutputTo "PUBLISHER"
json_select "LABEL_NAME"
SayRest

# Album title
OutputTo "ALBUM"
json_select "ALB_TITLE"
IfVar "standardUppercase" "true"
	SayFormat "$caps2(%_current%)"
Else
	SayRest
EndIf

# Album Cover or Artist Picture
OutputTo "COVERURL"
json_select "__TYPE__"
If "artist"
	json_select "ART_PICTURE"
	Say "https://cdn-images.dzcdn.net/images/artist/"
Else
	json_select "ALB_PICTURE"
	Say "https://e-cdns-images.dzcdn.net/images/cover/"
EndIf
SayRest
Say "/%CoverResolution%.jpg"

# Release date
OutputTo "RELEASETIME"
json_select "ORIGINAL_RELEASE_DATE"
If ""
	json_select "PHYSICAL_RELEASE_DATE"
EndIf
SayRest

# Year
OutputTo "YEAR"
GotoChar 1
SayUntil "-"

# UPC -> Barcode
OutputTo "BARCODE"
json_select "UPC"
SayRest

json_select_object "SUBTYPES"
	# Subtypes are: isStudio, isLive, isCompilation, isKaraoke
	OutputTo "COMPILATION"
	json_select "isCompilation"
	If "1"
		SayRest
	EndIf
	OutputTo "SETSUBTITLE"
	json_select "isStudio"
	If "1"
		Say "Studio "
	EndIf
	json_select "isLive"
	If "1"
		Say "Live "
	EndIf
	json_select "isKaraoke"
	If "1"
		Say "Karaoke"
	EndIf
json_unselect_object # SUBTYPES

OutputTo "PRODUCTION_COPYRIGHT"
json_select "PRODUCER_LINE"
RegexpReplace "^(?:[Ⓟ℗ⓟ]|\([pP]\))\s?(\d{4}\b)" "$1"
SayRest

OutputTo "COPYRIGHT"
json_select "COPYRIGHT"
RegexpReplace "^(?:[Ⓒ©ⓒ]|\([cC]\))\s?(\d{4}\b)" "$1"
SayRest

json_unselect_object # DATA

# Build list of contributing artist roles
Use "%SNG_CONTRIBUTORS%"
OutputTo "Role_List"
SayRegexp "(?<=\")[^\"]+?(?=\":)" ";"
UseFormat "$sort($dedup(%Role_List%,;),;)"
Set "Role_List"
SayRest

## Then iterate over all tracks
json_select_object "SONGS"
IfNot ""
	# Track count
	OutputTo "Temp_TotalTracks"
	json_select "total"
	SayRest

	# Disc count
	Set "Temp_TotalDiscs"
	Set "Temp_Multidisc"
	json_foreach "data"
		OutputTo "Temp_TotalDiscs"
		json_select "DISK_NUMBER"
		IfNot ""
			IfNotOutput "Temp_Multidisc"
				IfNot "1"
					Set "Temp_Multidisc" "1"
				EndIf
			EndIf
			Set "Temp_TotalDiscs"
			SayRest
		EndIf
	json_foreach_end

	# Multi-disc track count
	IfOutput "Temp_Multidisc"
		Set "Temp_Discnumber"
		Set "Temp_Items"
		json_foreach_reverse "data"
			json_select "DISK_NUMBER"
			IfNot "%Temp_Discnumber%"
				OutputTo "Temp_Discnumber"
				Set "Temp_Discnumber"
				SayRest
				json_select "TRACK_NUMBER"
				OutputTo "Tracks_Disc_%Temp_Discnumber%"
				SayRest
				OutputTo "Temp_Items"
				Say "|Tracks_Disc_%Temp_Discnumber%"
			EndIf
		json_foreach_end
	EndIf

	# Track loop
	json_foreach "data"

		# Track ID
		OutputTo "DEEZER_TRACK_ID"
		json_select "SNG_ID"
		SayRest
		Say "|"

		# Artist
		OutputTo "ARTIST"
		Set "Join"
		Set "Feat" " feat. "
		json_select_object "SNG_CONTRIBUTORS"
			json_select_array "main_artist" -1 ", "
			IfNot ""
				SayRest
				Set "Join" ", "
			EndIf
			json_select_array "featuring" -1 ", "
			IfNot ""
				SayOutput "Feat"
				SayRest
				Set "Feat" ", "
			EndIf
		json_unselect_object
		Say "|"

		# Length (seconds)
		OutputTo "_LENGTH"
		json_select "DURATION"
		SayRest
		Say "|"
		
		# Replay track gain
		OutputTo "REPLAYGAIN_TRACK_GAIN"
		json_select "GAIN"
		SayRest
		Say "|"

		# Disc number
		json_select "DISK_NUMBER"
		Set "Temp_Discnumber"
		OutputTo "Temp_Discnumber"
		SayRest
		OutputTo "DISCNUMBER"
		SayOutput "Temp_Discnumber"
		IfVar "appendTotals" "true"
			Say "/"
			SayOutput "Temp_TotalDiscs"
		EndIf
		Say "|"

		# Track number
		OutputTo "TRACK"
		json_select "TRACK_NUMBER"
		IfOutput "Temp_Multidisc"
			IfVar "prepenDiscnumber" "true"
				SayOutput "Temp_Discnumber"
				SayFormat "$num(%_current%,2)"
			Else
				IfVar "leadingZero" "true"
					SayFormat "$num(%_current%,2)"
				Else
					SayRest
				EndIf
			EndIf	
		Else
			IfVar "leadingZero" "true"
				SayFormat "$num(%_current%,2)"
			Else
				SayRest
			EndIf
		EndIf
		
		IfVar "appendTotals"  "true"
			Say "/"
			IfOutput "Temp_Multidisc"
				SayOutput "Tracks_Disc_%Temp_Discnumber%"
			Else
				SayOutput "Temp_TotalTracks"
			EndIf
		EndIf
		Say "|"

		# Version
		OutputTo "Temp"
		Set "Temp"
		json_select "VERSION"
		IfNot ""
			IfVar "standardUppercase" "true"
				SayFormat "$caps2(%_current%)"
			Else
				SayRest
			EndIf
		EndIf

		# Title
		OutputTo "TITLE"
		json_select "SNG_TITLE"
		IfVar "standardUppercase" "true"
			SayFormat "$caps2(%_current%)"
		Else
			SayRest
		EndIf

		# Title version
		IfVar "titleVersion" "true"
			IfOutput "Temp"
				Say " %Temp%"
			EndIf
		EndIf
		Say "|"

		# Subtitle
		OutputTo "SUBTITLE"
		json_select "VERSION"
		SayOutput "Temp"
		Say "|"
		
		# Explicitness
		OutputTo "ITUNESADVISORY"
		json_select "EXPLICIT_LYRICS"
		If "1"
			Say "1"
		EndIf
		Say "|"

		# ISRC
		OutputTo "ISRC"
		json_select "ISRC"
		SayRest
		Say "|"
		
		## Placeholder to indicate the tracks 'array' length
		OutputTo "TRACKS"
		Say "|"

		## All kind of contributors
		# Involved people can be: artist, main_artist, author, composer, producer,
		# engineer, conductor, concertmaster, musicdirector, orchestra, arranger, etc

		Set "Join"
		json_select_object "SNG_CONTRIBUTORS"
		Use ";%Role_List%"
		Do
			MoveChar 1
			Set "Temp_Role" 
			OutputTo "Temp_Role"
			SayUntil ";"
			Set "Temp_LIST"
			OutputTo "Temp_LIST"
			SayRest
			OutputTo "INVOLVEDPEOPLE"
			json_select_array "%Temp_Role%" -1 ", "
			IfNot ""
				SayOutput "Join"
				SayFormat "$caps2(%Temp_Role%, -_): "
				SayRest
				Say ";"
				Set "Join" " "
			EndIf
			Use "%Temp_LIST%"
		While ";"
		Say "|"	

		# Composer / Author
		OutputTo "Temp_Composer"
		Set "Temp_Composer"
		Set "Join"
		json_select_array "composer" -1 ", "
		IfNot ""
			SayRest
			Set "Join" ", "
		EndIf
		## We may also add authors as composers
		IfVar "includeAuthor" "true"
			json_select_array "author" -1 ", "
			IfNot ""
				SayOutput "Join"
				SayRest
				Set "Join" ", "
			EndIf
		EndIf
		OutputTo "COMPOSER"
		SayFormat "$dedup(%Temp_Composer%,', ')"
		Say "|"

		# Conductor
		OutputTo "CONDUCTOR"
		json_select_array "conductor" -1 ", "
		IfNot ""
			SayRest
		EndIf
		Say "|"

		# Lyricist / Writer (actually never seen)
		OutputTo "Temp_Lyricist"
		Set "Temp_Lyricist"
		Set "Join"
		json_select_array "lyricist" -1 ", "
		IfNot ""
			SayRest
			Set "Join" ", "
		EndIf
		## We'll also add songwriters as lyricists (actually never seen)
		json_select_array "writer" -1 ", "
		IfNot ""
			SayOutput "Join"
			SayRest
			Set "Join" ", "
		EndIf
		## We may also add authors as lyricist
		IfVar "includeAuthor" "false"
			json_select_array "author" -1 ", "
			IfNot ""
				SayOutput "Join"
				SayRest
				Set "Join" ", "
			EndIf
		EndIf
		OutputTo "LYRICIST"
		SayFormat "$dedup(%Temp_Lyricist%,', ')"
		Say "|"

		json_unselect_object # SNG_CONTRIBUTORS
	json_foreach_end
EndIf # SONGS

# Garbage Collection: Check for leftover temp variables
IfOutput "Temp_Items"
	Use "%Temp_Items%"
	OutputTo "Temp_Items"
	Do
		MoveChar 1
		Set "Temp_Items"
		SayUntil "|"
		Set "%Temp_Items%"
	While "|"
EndIf
Set "Temp_Items"

## Void temp tags/variables:
Set "CoverResolution"
Set "Feat"
Set "Join"
Set "Lang"
Set "Temp"
Set "Role_List"
Set "SNG_CONTRIBUTORS"
Set "Temp_Composer"
Set "Temp_Lyricist"
Set "Temp_Discnumber"
Set "Temp_Multidisc"
Set "Temp_Role"
Set "Temp_TotalDiscs"
Set "Temp_TotalTracks"
