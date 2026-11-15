[ParserScriptIndex]=...
# ###################################################################
#					I  N  D  E  X
# ###################################################################

## Comment/uncomment for debugging purposes:
#DebugWriteInput "bandcamp-index-input.out"
#Debug "ON" "bandcamp-index-debug.out"

findline "<!-- search result type" 1 1

do
	findinline "=" 1 1
	If "album"
	IfVar "listAlbums" "true"
		joinuntil "</li>"
		unspace
		regexpreplace ">\s+<" "><"
		regexpreplace ">\s+" ">"
		regexpreplace "\s+<" "<"
		regexpreplace "\s{2,}" ""
		regexpreplace "(<div class\=\"heading).*?search_sig.*?(\">)" "$1albumtitle$2"
		# Cover URL
			ifvar "mp3tagProxy" "true"
				say "http://127.0.0.1:8787/"
			endif
			sayregexp "(?<=div class=\"art\"><img src\=\").+?(?=\">)" ", " "</li>"
			say "|"
		# Cover Full URL
			ifvar "browserProxy" "true"
				say "http://127.0.0.1:8787/"
			endif
			sayregexp "(?<=div class=\"art\"><img src\=\").+?(?=_3.jpg\">)" ", " "</li>"
			say "_1.jpg"
			say "|"
		# Preview
			ifvar "browserProxy" "true"
				say "http://127.0.0.1:8787/"
			endif
			sayregexp "(?<=div class\=\"itemurl\"><a href\=\").+?(?=\?)" ", " "</li>"
			say "|"
		# URL
			ifvar "mp3tagProxy" "true"
				say "http://127.0.0.1:8787/"
			endif
			sayregexp "(?<=div class\=\"itemurl\"><a href\=\").+?(?=\?)" ", " "</li>"
			say "|"
		# Type
			say "Album"
			say "|"
		# Artist
			sayregexp "(?<=div class\=\"subhead\">by ).+?(?=<)" ", " "</li>"
			say "|"
		# Album
			sayregexp "(?<=albumtitle\">).+?(?=<)" ", " "</li>"
			say "|"
		# Date
			sayregexp "(?<=released\">released ).+?(?=<)" ", " "</li>"
			saynewline
	EndIf			
	Else
		If "track"
		IfVar "listTracks" "true"
			joinuntil "</li>"
			unspace
			regexpreplace ">\s+<" "><"
			regexpreplace ">\s+" ">"
			regexpreplace "\s+<" "<"
			regexpreplace "\s{2,}" ""
			regexpreplace "(<div class\=\"heading).*?search_sig.*?(\">)" "$1albumtitle$2"
			regexpreplace "subhead\">from.*?by" "subhead\">by"
			# Cover URL
				ifvar "mp3tagProxy" "true"
					say "http://127.0.0.1:8787/"
				endif
				sayregexp "(?<=div class=\"art\"><img src\=\").+?(?=\">)" ", " "</li>"
				say "|"
			# Cover Full URL
				ifvar "browserProxy" "true"
					say "http://127.0.0.1:8787/"
				endif
				sayregexp "(?<=div class=\"art\"><img src\=\").+?(?=_3.jpg\">)" ", " "</li>"
				say "_1.jpg"
				say "|"
			# Preview
				ifvar "browserProxy" "true"
					say "http://127.0.0.1:8787/"
				endif
				sayregexp "(?<=div class\=\"itemurl\"><a href\=\").+?(?=\?)" ", " "</li>"
				say "|"
			# URL
				ifvar "mp3tagProxy" "true"
					say "http://127.0.0.1:8787/"
				endif
				sayregexp "(?<=div class\=\"itemurl\"><a href\=\").+?(?=\?)" ", " "</li>"
				say "|"
			# Type
				say "Track"
				say "|"
			# Artist
				sayregexp "(?<=div class\=\"subhead\">by ).+?(?=<)" ", " "</li>"
				say "|"
			# Album
				sayregexp "(?<=albumtitle\">).+?(?=<)" ", " "</li>"
				say "|"
			# Date
				sayregexp "(?<=released\">released ).+?(?=<)" ", " "</li>"
				saynewline
		EndIf
		EndIf
	EndIf
		
	# Loop
	findline "<!-- search result type=" 2 1
	unspace

while "<!-- search result type=" 200

