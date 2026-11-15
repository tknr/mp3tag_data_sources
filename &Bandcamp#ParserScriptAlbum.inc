[ParserScriptAlbum]=...
#####################################################################
#					A  L  B  U  M
#####################################################################
## Comment/uncomment for debugging purposes:
#DebugWriteInput "bandcamp-album-input.out"
#Debug "ON" "bandcamp-album-debug.out"

# Check for Bandcamp URL
Use "%CurrentUrl%"
RegexpReplace "^https?://.*?(bandcamp\.com/).*" "$1"
ifnot "bandcamp.com"
	Set "<Error>" "No Bandcamp URL %CurrentUrl%"
	OutputTo "<Fatal>"
	# force abort
	json "ON" "current"
EndIf

# Extract embedded JSON data
FindLine "<script type=\"application/ld+json\">" 1 1
If ""
	# Prepare a meaningful error message for no JSON data found
	Set "<Error>" "No JSON data found in %CurrentUrl%"
	OutputTo "<Fatal>"
	# force abort
	json "ON" "current"
EndIf
JoinUntil "</script>"
RegexpReplace "^.*?json\">\s*(.*)</script>.*" "$1"
# Vertical Bar is used internally and therefore has to be replaced in the input data
Replace "|" "$verticalBar()"

json "ON" "current"

Set "_JOIN"
OutputTo "_TEMP_ARTIST"
json_select_object "byArtist"
json_select "name"
SayRest
Set "_JOIN" "\\\\"
json_unselect_object

OutputTo "_TEMP_YEAR"
json_select "datePublished"
Replace " Jan " ".01."
Replace " Feb " ".02."
Replace " Mar " ".03."
Replace " Apr " ".04."
Replace " May " ".05."
Replace " Jun " ".06."
Replace " Jul " ".07."
Replace " Aug " ".08."
Replace " Sep " ".09."
Replace " Oct " ".10."
Replace " Nov " ".11."
Replace " Dec " ".12."
RegexpReplace "(\d+).(\d+).(\d+)\s(.+?)\sGMT" "$3-$2-$1"
SayRest

OutputTo "YEAR"
IfVar "fullDateFormat" "true"
	SayOutput "_TEMP_YEAR"
Else
	GotoChar 1
	Sayuntil "-"
EndIf

OutputTo "RELEASETIME"
json_select "datePublished"
RegexpReplace "(\d+).(.+).(\d+)\s(.+?)\sGMT" "T$4"
SayOutput "_TEMP_YEAR"
SayRest

OutputTo "COVERURL"
json_select "image"
IfVar "maxCoverQuality" "true"
	RegexpReplace "_\d+" "_0"
EndIf
SayRest

Set "_POSTFIX"
json_select "@type"
If "MusicAlbum"

	OutputTo "ALBUM"
	json_select "name"
	SayRest

	OutputTo "TOTALTRACKS"
	json_select "numTracks"
	SayRest

	# Album URL
	IfVar "WWWBandcamp" "true"
		OutputTo "WWW BANDCAMP"
	Else
		OutputTo "WWW"
	EndIf
	json_select "mainEntityOfPage"
	SayRest

	json_select_object "track"

	json_foreach "itemListElement"
		json_select "position"

		OutputTo "TRACK"
		IfVar "leadingZero" "true"
			SayFormat "$num(%_current%,2)"
		Else
			SayRest
		Endif
		IfVar "appendTotals"  "true"
			Say "/"
			SayOutput "TOTALTRACKS"
		Endif
		Say "|"

		json_select_object "item"

		OutputTo "TITLE"
		json_select "name"
		SayRest
		Say "|"

		OutputTo "TRACKS"
		Say "|"

		OutputTo "ARTIST"
		json_select_object "byArtist"
		If "byArtist"
			json_select "name"
			SayRest
			json_unselect_object
		EndIf
		Say "|"

		OutputTo "_LENGTH"
		json_select "duration"
		RegexpReplace "P00H(\d+)M(\d+)S" "$1:$2"
		RegexpReplace "P(\d+)H(\d+)M(\d+)S" "$1:$2:$3"
		SayRest
		Say "|"

		OutputTo "WWW TRACK"
		json_select "mainEntityOfPage"
		SayRest
		Say "|"
		
		json_unselect_object

	json_foreach_end

	json_unselect_object

	# One Artist or Various Artists
	Use "%ARTIST%"
	Replace "|" ""
	If ""
		Set "ARTIST" "%_TEMP_ARTIST%"
	Else
		Set "ALBUMARTIST" "%_TEMP_ARTIST%"
	EndIf

Else

	IfVar "SeparateTags" "true"
		Set "_POSTFIX" " TRACK"
	EndIf
	
	json_select_object "inAlbum"
	If "inAlbum"
		OutputTo "ALBUM"
		json_select "name"
		SayRest
		json_select_object "byArtist"
		If "byArtist"
			OutputTo "_TEMP_ARTIST"
			SayOutput "_JOIN"
			json_select "name"
			SayRest
			json_unselect_object
		EndIf
		json_unselect_object
	EndIf

	OutputTo "TRACK"
	json_foreach "additionalProperty"
		json_select "name"
		If "tracknum"
			json_select "value"
			IfVar "leadingZero" "true"
				SayFormat "$num(%_current%,2)"
			Else
				SayRest
			Endif
			IfVar "appendTotals"  "true"
				Say "/1"
			Endif
		EndIf
	json_foreach_end
	Say "|"
	
	OutputTo "TITLE"
	json_select "name"
	SayRest
	Say "|"

	OutputTo "TRACKS"
	Say "|"

	OutputTo "ISRC"
	json_select "isrcCode"
	SayRest
	Say "|"

	OutputTo "_LENGTH"
	json_select "duration"
	RegexpReplace "P00H(\d+)M(\d+)S" "$1:$2"
	RegexpReplace "P(\d+)H(\d+)M(\d+)S" "$1:$2:$3"
	SayRest
	Say "|"

	Set "ARTIST" "%_TEMP_ARTIST%"

	OutputTo "WWW TRACK"
	json_select "mainEntityOfPage"
	SayRest

EndIf

json_select_object "publisher"
If "publisher"
	OutputTo "PUBLISHER"
	json_select "name"
	SayRest
	OutputTo "GENRE%_POSTFIX%"
	json_select "genre"
	RegexpReplace "https://bandcamp.com/discover/(.+?)" "$1"
	RegexpReplace "https://bandcamp.com/tag/(.+?)" "$1"
	SayFormat "$caps2(%_current%)"
	json_unselect_object
EndIf

OutputTo "KEYWORDS%_POSTFIX%"
json_select_array "keywords" -1 ", "
SayFormat "$caps2(%_current%, -)"

OutputTo "COMMENT%_POSTFIX%"
json_select "description"
ifnot ""
	SayRest
	GotoChar 1
	FindInLine "http" 1 1
	MoveChar -4
	If "http"
		OutputTo "COMMENT URL"
		SayRegexp "(?:https?://)?(?:[a-zA-Z0-9-]+\.)+bandcamp\.com(?:/[^\s]*)?" "\\\\"
	Endif
Endif

OutputTo "CREDITS%_POSTFIX%"
json_select "creditText"
SayRest

# Postprozessing for standard uppercase tags
IfVar "standardUppercase" "true"
	Use "%ALBUM%"
	Set "ALBUM"
	OutputTo "ALBUM"
	SayFormat "$caps2(%_current%,' -(|')" 1
	IfOutput "ALBUMARTIST"
		Use "%ALBUMARTIST%"
		Set "ALBUMARTIST"
		OutputTo "ALBUMARTIST"
		SayFormat "$caps2(%_current%,' -(|')" 1
	EndIf
	Use "%ARTIST%"
	Set "ARTIST"
	OutputTo "ARTIST"
	SayFormat "$caps2(%_current%,' -(|')" 1
	Use "%TITLE%"
	Set "TITLE"
	OutputTo "TITLE"
	SayFormat "$caps2(%_current%,' -(|')" 1
EndIf

Set "_JOIN"
Set "_POSTFIX"
Set "_TEMP_ARTIST"
Set "_TEMP_YEAR"