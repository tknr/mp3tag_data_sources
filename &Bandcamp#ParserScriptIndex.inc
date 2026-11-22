[ParserScriptIndex]=...
#####################################################################
#					I  N  D  E  X
#####################################################################
## Comment/uncomment for debugging purposes:
#DebugWriteInput "bandcamp-index-input.out"
#Debug "ON" "bandcamp-index-debug.out"

# Configuration setting for inclusion of albums and tracks
set "list_album"
set "list_track"
IfVar "listAlbums" "true"
	set "list_album" "1"
endif
IfVar "listTracks" "true"
	set "list_track" "1"
else
	# Always list something
	set "list_album" "1"
endif

# Configuration settings for using the proxy
set "mp3tagProxy"
ifvar "mp3tagProxy" "true"
	set "mp3tagProxy" "http://127.0.0.1:8787/"
endif
set "browserProxy"
ifvar "browserProxy" "true"
	set "browserProxy" "http://127.0.0.1:8787/"
endif

# Configuration setting for Cover Quality only used for browser preview
IfVar "maxCoverQuality" "true"
	set "CoverQuality" "_0.jpg"
else
	set "CoverQuality" "_10.jpg"
EndIf

# Processing the search results
findline "<!-- search result type" 1 1
do
	# Type of element
	outputto "type"
	set "type"
	findinline "=" 1 1
	sayuntil " "

	outputto "output"
	Ifoutput "list_%type%"
		joinuntil "</li>"
		unspace
		# Remove unneeded parts
		regexpreplace "(?:<a href=\".*?\">)(.*?)(?:</a>)" "$1"

		# Cover URL
		sayoutput "mp3tagProxy"
		sayregexp "(?<=div class=\"art\"><img src\=\").+?(?=\">)"
		say "|"

		# Cover Full URL
		sayoutput "browserProxy"
		sayregexp "(?<=div class=\"art\"><img src\=\").+?(?=_\d+.jpg\">)"
		sayoutput "CoverQuality"
		say "|"

		# Preview
		sayoutput "browserProxy"
		sayregexp "(?<=div class\=\"itemurl\">).+?(?=<)"
		say "|"

		# URL
		sayoutput "mp3tagProxy"
		sayregexp "(?<=div class\=\"itemurl\">).+?(?=<)"
		say "|"

		# Type
		sayformat "$caps(%type%)"
		say "|"

		# Artist
		sayregexp "(?<=div class\=\"subhead\">by ).+?(?=<)"
		say "|"

		# Album
		sayregexp "(?<=div class\=\"heading\">).+?(?=<)"
		say "|"

		# Date
		sayregexp "(?<=released\">released ).+?(?=<)"
		saynewline
	EndIf			

	# Loop
	findline "<!-- search result type=" 2 1
	unspace
while "<!-- search result type=" 200
