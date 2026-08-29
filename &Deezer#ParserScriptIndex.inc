################################################################################
#                        LIST OF SEARCH RESULTS DIALOG                         #
################################################################################
[ParserScriptIndex]=...
## Comment/uncomment for debugging purposes:
# DebugWriteInput "deezer-index-input.out"
# Debug "ON" "deezer-index-debug.out"

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
				Say "https://www.deezer.com/album/"
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
