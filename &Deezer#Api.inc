################################################################################
#                        LIST OF SEARCH RESULTS DIALOG                         #
################################################################################
[ParserScriptIndex]=...
## Comment/uncomment for debugging purposes:
# DebugWriteInput "deezer-api-index-input.out"
# Debug "ON" "deezer-api-index-debug.out"

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

json "ON" "current"

Set "ORDER" "ALBUM,TRACK"

# Loop in the given order over albums and tracks that match
Use " %ORDER%"
Do
	# Extract next key from order list
	MoveChar 1
	Set "KEY"
	OutputTo "KEY"
	SayUntil ","
	# Save current input buffer for While command
	Set "ORDER"
	OutputTo "ORDER"
	SayRest

	OutputTo "Output"
	json_select_object "%KEY%"

		## First, check if we've found anything
		json_select "count"
	 
		## Then iterate over all releases
		IfNot "0"
			json_foreach "data"

				# Cover Thumbnail
				json_select "ALB_PICTURE"
				Say "https://e-cdns-images.dzcdn.net/images/cover/"
				SayRest
				Say "/250x250.jpg"
				Say "|"

				# Preview full cover in browser
				json_select "ALB_PICTURE"
				Say "https://e-cdns-images.dzcdn.net/images/cover/"
				SayRest
				Say "/%CoverResolution%.jpg"
				Say "|"

				# Preview album in  browser
				json_select "ALB_ID"
				Say "https://www.deezer.com/album/"
				SayRest
				Say "|"

				# URL
				json_select "ALB_ID"
				Say "https://api.deezer.com/2.0/album/"
				SayRest
				Say "|"

				# Counter
				json_select "ALB_ID"
				If "%TOP_ALBUM%"
					Say " *Top* "
				Endif
				json_foreach_counter
				SayFormat ". $caps(%KEY%)"
				Say "|"

				# Artist
				json_select "ART_NAME"
				SayRest
				Say "|"

				# Album Title
				json_select "ALB_TITLE"
				SayRest
				Say "|"

				# Song Title
				json_select "SNG_TITLE"
				SayRest
				json_select "VERSION"
				ifnot ""
					Say " "
					SayRest
				endif
				Say "|"

				# Number of tracks or track number
				json_select "NUMBER_TRACK"
				If ""
					json_select "TRACK_NUMBER"
					Say "Track "
					SayRest
				Else
					SayRest
					Say " Track(s)"
				Endif
				Say "|"

				# Release Date
				json_select "PHYSICAL_RELEASE_DATE"
				IfNot ""
					SayRest
				Endif

				SayNewline
			json_foreach_end
		Endif
	json_unselect_object

	# Restore input buffer for While command
	Use "%ORDER%"
While ","

################################################################################
#                        ADJUST TAG INFORMATION DIALOG                         #
################################################################################
[ParserScriptAlbum]=...
## Comment/uncomment for debugging purposes:
# DebugWriteInput "deezer-api-album-input.out"
# Debug "ON" "deezer-api-album-debug.out"

# Configuration settings processing
Set "CoverResolution"
OutputTo "CoverResolution"
SayVar "CoverResolution"

json "ON" "current"

# Deezer Album ID
OutputTo "DEEZER_ALBUM_ID"
json_select "id"
SayRest

# Album URL
OutputTo "WWW"
json_select "link"
SayRest

# Album artist
json_select_object "artist"
	json_select "id"
	OutputTo "DEEZER_ARTIST_ID"
	SayRest
	json_select "name"
	OutputTo "ALBUMARTIST"
	SayRest
json_unselect_object

# Publisher
OutputTo "PUBLISHER"
json_select "label"
SayRest

# Album title
OutputTo "ALBUM"
json_select "title"
SayRest

# Cover
OutputTo "COVERURL"
json_select "md5_image"
Say "https://e-cdns-images.dzcdn.net/images/cover/"
SayRest
Say "/%CoverResolution%.jpg"

# Release date
OutputTo "RELEASETIME"
json_select "release_date"
SayRest

# Year
OutputTo "YEAR"
GotoChar 1
SayUntil "-"

# UPC -> Barcode
OutputTo "BARCODE"
json_select "upc"
SayRest

OutputTo "GENRE"
Set "Join"
json_select_object "genres"
	json_foreach "data"
		json_select "name"
		SayOutput "Join"
		SayRest
		Set "Join" ", "
	json_foreach_end
json_unselect_object

# Track count
OutputTo "TOTALTRACKS"
json_select "nb_tracks"
SayRest

# Explicitness
OutputTo "ITUNESADVISORY"
json_select "explicit_lyrics"
IfNot "0"
	Say "1"
Endif

## Then iterate over all tracks
json_select_object "tracks"

# Track loop
json_foreach "data"

	# Track ID
	OutputTo "DEEZER_TRACK_ID"
	json_select "id"
	SayRest
	Say "|"

	# Artist
	OutputTo "ARTIST"
	json_select_object "artist"
		json_select "name"
		SayRest
	json_unselect_object	
	Say "|"

	# Length (seconds)
	OutputTo "_LENGTH"
	json_select "duration"
	SayRest
	Say "|"

	# Track number
	OutputTo "TRACK"
	json_foreach_counter
	SayRest
	Say "|"

	# Title
	OutputTo "TITLE"
	json_select "title_short"
	IfVar "standardUppercase" "true"
		SayFormat "$caps(%_current%)"
	Else
		SayRest
	EndIf

	# Title version
	IfVar "titleVersion" "true"
		json_select "title_version"
		IfNot ""
			Say " "
			SayRest
		Endif
	Endif
	Say "|"

	# Subtitle
	OutputTo "SUBTITLE"
	json_select "title_version"
	SayRest
	Say "|"
	
	# Explicitness
	OutputTo "ITUNESADVISORY"
	json_select "explicit_lyrics"
	IfNot "0"
		Say "1"
	Endif
	Say "|"

	## Placeholder to indicate the tracks 'array' length
	OutputTo "TRACKS"
	Say "|"

json_foreach_end

## Void temp tags/variables:
Set "CoverResolution"
Set "Join"
