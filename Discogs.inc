# ###################################################################
# Mp3tag Tag Source for Discogs API (Official)
#
# Search by is specified in host tag source that includes this file.
# 
# This file should be stored in your tag sources sources directory
# %APPDATA%\Mp3tag\data\sources
# and requires Mp3tag v2.97b or above.
#
# [2025-11-12] v1.19 NEW: added support for importing COMPOSER from extra artist roles "Composed-By", "Music-By", "Songwriter", and "Written-By" (and variants thereof).
#                    NEW: added support for importing MIXARTIST from extra artist role "Remix".
#                    CHG: automatically remove disambiguation numbers from artist names.
#                    FIX: DISCNUMBER was not imported correctly for tracklistings which used the - character only for some of the tracks.
#                    NEW: added support for importing COMPOSER from extra artist roles of index tracks and use them for subtracks.
# [2024-05-28] v1.18 NEW: added column Cover with cover dimensions to list of query results.
# [2024-05-19] v1.17 NEW: added column BARCODE to list of query results.
#                    NEW: added support for importing BARCODE.
#                    NEW: added support for importing ALBUMARTISTSORT.
#                    CHG: CATALOGNUMBER set to `none` is now ignored.
#                    CHG: multiple media per release are now separated by comma.
#                    FIX: further improvements at detecting implicit artists for subtracks.
# [2024-05-15] v1.16 FIX: further improvements at detecting implicit artists for subtracks.
#                    FIX: release titles containing hyphens were not displayed correctly in query results list.
# [2024-05-08] v1.15 NEW: added support for cover thumbnails in list of query results. 
#                    FIX: subtracks with implicit artist from index track were not identified correctly.
#                    FIX: comma used as join separator had extra whitespace prefix.
# [2022-03-22] v1.14 CHG: renamed CATALOGID to CATALOGNUMBER to improve compatibility with other software.
# [2019-09-09] v1.13 CHG: moved common Discogs tag source structure to this tag source include file.
# [2019-05-23] v1.12 FIX: titles with pipe symbol were identified as different titles.
# [2019-03-15] v1.11 NEW: added support for subtracks.
# [2018-11-24] v1.10 FIX: artist and albumartist were wrong for releases where albumartist consists of multiple artists.
# [2018-05-07] v1.09 CHG: added all formats and catalog number
# [2018-04-10]       CHG: added support for track numbers and disc numbers from Discogs
# [2017-12-20] v1.08 FIX: releases with missing year had YEAR set to 0.
# [2017-10-16] v1.07 FIX: format descriptions were not imported into MEDIATYPE field.
# [2017-06-30] v1.06 CHG: added fallback detection of secondary images if no primary image exists
# [2017-06-28]       CHG: added check if query gave any results
# [2017-06-26] v1.05 CHG: extended result list to current API maximum of 100 releases
# [2017-06-15] v1.04 FIX: added a workaround for single artists with join
# [2016-01-31] v1.03 CHG: changed to use HTTPS
# [2014-10-26] v1.02 CHG: changed to use %20 as word separator
# [2014-10-03] v1.01 CHG: added image caching through proxy 
# [2014-09-28]       FIX: join was not interpreted correctly for artists
# [2014-09-20] v1.00
#
# ###################################################################

[Name]=Discogs.com
[BasedOn]=www.discogs.com
[IndexUrl]=https://api.discogs.com/database/search?type=release&per_page=100
[AlbumUrl]=
[WordSeparator]=%20
[IndexFormat]=%Cover%|%Artist%|%Album%|%_url%|%Format%|%Year%|%Label%|%Catalog%|%Country%|%Barcode%|%_coverurl%|%_preview%
[UserAgent]=1
[Encoding]=url-utf-8

[ParserScriptIndex]=...
# ###################################################################
#					I  N  D  E  X
# ###################################################################
replace "|" "$verticalBar()"
json "ON" "current"

# First, check if we've found anything
json_select_object "pagination"
json_select "items"

# Then iterate over all releases
ifnot "0"
	json_unselect_object
	json_foreach "results"

		# Cover Dimensions
		json_select "cover_image"
		RegexpReplace ".+st.discogs.com.+" ""
		RegexpReplace ".+?h:(\d+)\/w:(\d+).+" "$2x$1"
		SayRest
		say "|"

		# Artist
		json_select "title"
		sayuntil " - "
		say "|" 

		# Album
		movechar 3
		sayrest
		say "|"

		# URL
		json_select "resource_url"
		sayrest
		say "|"

		# Format
		json_select_array "format" -1 ", "
		sayrest
		say "|"

		# Year
		json_select "year"
		sayrest
		say "|"

		# Label
		json_select_array "label" 1
		sayrest
		say "|"

		# Catno
		json_select "catno"
		sayrest
		say "|"

		# Country
		json_select "country"
		sayrest
		say "|"

		# Barcode (may be missing)
		json_select_many_count "barcode" 
		ifnot "0"
			json_select_array "barcode" 1
			sayrest
		endif
		say "|"

		# Cover Preview
		json_select "thumb"
		ifnot ""
			say "http://cache.mp3tag.de/coxy"
			sayregexp "(?<=\.com).*"
		endif
		say "|"

		# Preview
		say "https://www.discogs.com/release/"
		json_select "id"
		sayrest

		saynewline

	json_foreach_end
endif


[ParserScriptAlbum]=...
# ###################################################################
#					A  L  B  U  M
# ###################################################################
replace "|" "$verticalBar()"
json "ON" "current"

# Album
outputto "ALBUM"
json_select "title"
sayrest

# Discogs_Release_ID
outputto "DISCOGS_RELEASE_ID"
json_select "id"
sayrest

# Coverurl
json_foreach "images"
	json_select "type"
	if "primary"
		ifnotoutput "TMP_PRIMARY"
			outputto "TMP_PRIMARY"
			json_select "uri"
			say "http://cache.mp3tag.de/coxy"
			sayregexp "(?<=\.com).*"
		endif
	endif
	if "secondary"
		ifnotoutput "TMP_SECONDARY"
			outputto "TMP_SECONDARY"
			json_select "uri"
			say "http://cache.mp3tag.de/coxy"
			sayregexp "(?<=\.com).*"
		endif
	endif
json_foreach_end

outputto "coverurl"
ifoutput "TMP_SECONDARY"
	set "coverurl"
	sayoutput "TMP_SECONDARY"
	set "TMP_SECONDARY"
endif
ifoutput "TMP_PRIMARY"
	set "coverurl"
	sayoutput "TMP_PRIMARY"
	set "TMP_PRIMARY"
endif

# Catalog Number
outputto "CATALOGNUMBER"
json_foreach "labels"
	json_select "catno"
	ifnot "none"
		ifoutput "CATALOGNUMBER"
			say ", "
		endif
		sayrest
	endif
json_foreach_end

# Publisher
outputto "PUBLISHER"
json_select_many "labels" "name" ", "
sayrest

# Barcode Number
outputto "BARCODE"
json_foreach "identifiers"
	json_select "type"
	if "Barcode"
		json_select "value"
		ifnotoutput "BARCODE"
			regexpreplace "\D" ""
			sayrest
		endif
	endif
json_foreach_end

# Media Type
outputto "MEDIATYPE"
json_foreach "formats"
	json_select "name"
	ifoutput "MEDIATYPE"
		say ", "
	endif
	sayrest
	json_select_array "descriptions" -1 ", "
	ifnot ""
		say " ("
		sayrest
		say ")"
	endif
json_foreach_end

# Genre
outputto "GENRE"
json_select_array "genres" -1 ", "
sayrest

# Style
outputto "STYLE"
json_select_array "styles" -1 ", "
sayrest

# Country
outputto "COUNTRY"
json_select "country"
sayrest

# Year
outputto "YEAR"
json_select "year"
ifnot "0"
	sayrest
endif

# Www
outputto "WWW"
json_select "uri"
sayrest

# Sortkey
outputto "ALBUMARTISTSORT"
json_select "artists_sort"
sayrest

# Artist or Albumartist
set "TEMP_Join"
outputto "TEMP_Artist"
json_foreach "artists"
	sayoutput "TEMP_Join"
	set "TEMP_Join"
	json_select "anv"
	if ""
		json_select "name"
		regexpreplace "\\s*\\(\\d+\\)$" ""
	endif
	sayrest
	outputto "TEMP_Join"
	json_select "join"
	ifnot ""
		ifnot ","
			say " "
		endif
		sayrest
		say " "
	endif
	outputto "TEMP_Artist"
json_foreach_end

json_foreach "tracklist"
	json_select_many "artists" "name" ", "
json_foreach_end

ifnot ""
	## Various Artists
	outputto "ALBUMARTIST"
	sayoutput "TEMP_Artist"
endif

json_foreach "extraartists"
	json_select "role"

	# Normalize Composer role variants used by Discogs
	regexpreplace "(?i).*composed[\s\.-]?by.*" "Composed-By"
	regexpreplace "(?i).*music[\s\.-]?by.*" "Composed-By"
	regexpreplace "(?i).*written[\s\.-]?by.*" "Composed-By"
	regexpreplace "(?i).*songwriter*" "Composed-By"

	regexpreplace "(?i).*remix.*" "Remix"

	if "Composed-By"
		outputto "TEMP_IndexComposer"
		sayoutput "TEMP_IndexComposerJoin"
		set "TEMP_IndexComposerJoin"

		json_select "name"
		regexpreplace "\\s*\\(\\d+\\)$" ""
		sayrest

		json_select "tracks"
		ifnot ""
			say " ("
			sayrest
			say ")"
		endif

		outputto "TEMP_IndexComposerJoin"
		json_select "join"
		ifnot ""
			ifnot ","
				say " "
			endif
			sayrest
			say " "
		else
			say ", "
		endif
	endif
	if "Remix"
		outputto "TEMP_IndexMixArtist"
		sayoutput "TEMP_IndexMixArtistJoin"
		set "TEMP_IndexMixArtistJoin"

		json_select "name"
		regexpreplace "\\s*\\(\\d+\\)$" ""
		sayrest

		outputto "TEMP_IndexMixArtistJoin"
		json_select "join"
		ifnot ""
			ifnot ","
				say " "
			endif
			sayrest
			say " "
		else
			say ", "
		endif
	endif
json_foreach_end

# Tracks
json_foreach "tracklist"
	json_select "type_"

	if "index"
		json_select "title"
		set "TEMP_IndexTitle"
		outputto "TEMP_IndexTitle"
		sayrest
		say ", "

		set "TEMP_IndexArtist"
		json_select_many "artists" "name" ", "
		ifnot ""
			outputto "TEMP_IndexArtist"
			set "TEMP_Join"
			json_foreach "artists"
				sayoutput "TEMP_Join"
				set "TEMP_Join"
				json_select "anv"
				ifnot ""
					sayrest
				else
					json_select "name"
					regexpreplace "\\s*\\(\\d+\\)$" ""
					sayrest
				endif

				outputto "TEMP_Join"
				json_select "join"
				ifnot ""
					ifnot ","
						say " "
					endif
					sayrest
					say " "
				endif
				outputto "TEMP_IndexArtist"
			json_foreach_end
		else
			outputto "TEMP_IndexArtist"
			sayoutput "TEMP_Artist"
		endif

		set "TEMP_ComposerJoin"
		set "TEMP_MixArtistJoin"
		json_foreach "extraartists"
			json_select "role"

			# Normalize Composer role variants used by Discogs
			regexpreplace "(?i).*composed[\s\.-]?by.*" "Composed-By"
			regexpreplace "(?i).*music[\s\.-]?by.*" "Composed-By"
			regexpreplace "(?i).*written[\s\.-]?by.*" "Composed-By"
			regexpreplace "(?i).*songwriter*" "Composed-By"

			regexpreplace "(?i).*remix.*" "Remix"

			if "Composed-By"
				outputto "TEMP_Composer"
				sayoutput "TEMP_ComposerJoin"
				set "TEMP_ComposerJoin"

				json_select "name"
				regexpreplace "\\s*\\(\\d+\\)$" ""
				sayrest

				outputto "TEMP_ComposerJoin"
				json_select "join"
				ifnot ""
					ifnot ","
						say " "
					endif
					sayrest
					say " "
				else
					say ", "
				endif
			endif
			if "Remix"
				outputto "TEMP_MixArtist"
				sayoutput "TEMP_MixArtistJoin"
				set "TEMP_MixArtistJoin"

				json_select "name"
				regexpreplace "\\s*\\(\\d+\\)$" ""
				sayrest

				outputto "TEMP_MixArtistJoin"
				json_select "join"
				ifnot ""
					ifnot ","
						say " "
					endif
					sayrest
					say " "
				else
					say ", "
				endif
			endif
		json_foreach_end

		json_foreach "sub_tracks"
			json_select "type_"
			if "track"
				json_select "position"
				findinline "-" 1 1
				outputto "TEMP_Track"
				movechar -1
				if "-"
					movechar 1
				else
					gotochar 1
				endif
				sayrest
				say "|"

				gotochar 1
				findinline "-" 1 1
				outputto "TEMP_Discnumber"
				movechar -1
				if "-"
					gotochar 1
					sayregexp ".+" "" "-"
				endif
				say "|"

				outputto "TEMP_Title"
				sayoutput "TEMP_IndexTitle"
				json_select "title"
				sayrest
				say "|"
				outputto "tracks"
				say "|"

				outputto "_LENGTH"
				json_select "duration"
				sayrest
				say "|"

				json_select_many "artists" "name" ", "
				ifnot ""
					outputto "ARTIST"
					set "TEMP_Join"
					json_foreach "artists"
						sayoutput "TEMP_Join"
						set "TEMP_Join"
						json_select "anv"
						ifnot ""
							sayrest
						else
							json_select "name"
							regexpreplace "\\s*\\(\\d+\\)$" ""
							sayrest
						endif

						outputto "TEMP_Join"
						json_select "join"
						ifnot ""
							ifnot ","
								say " "
							endif
							sayrest
							say " "
						endif
						outputto "ARTIST"
					json_foreach_end
				else
					outputto "ARTIST"
					sayoutput "TEMP_IndexArtist"
				endif
				say "|"

				set "TEMP_ComposerJoin"
				set "TEMP_MixArtistJoin"
				json_foreach "extraartists"
					json_select "role"

					# Normalize Composer role variants used by Discogs
					regexpreplace "(?i).*composed[\s\.-]?by.*" "Composed-By"
					regexpreplace "(?i).*music[\s\.-]?by.*" "Composed-By"
					regexpreplace "(?i).*written[\s\.-]?by.*" "Composed-By"
					regexpreplace "(?i).*songwriter*" "Composed-By"

					regexpreplace "(?i).*remix.*" "Remix"

					if "Composed-By"
						set "TEMP_Composer"
						outputto "TEMP_Composer"
						sayoutput "TEMP_ComposerJoin"
						set "TEMP_ComposerJoin"

						json_select "name"
						regexpreplace "\\s*\\(\\d+\\)$" ""
						sayrest

						outputto "TEMP_ComposerJoin"
						json_select "join"
						ifnot ""
							ifnot ","
								say " "
							endif
							sayrest
							say " "
						else
							say ", "
						endif
					endif
					if "Remix"
						set "TEMP_MixArtist"
						outputto "TEMP_MixArtist"
						sayoutput "TEMP_MixArtistJoin"
						set "TEMP_MixArtistJoin"

						json_select "name"
						regexpreplace "\\s*\\(\\d+\\)$" ""
						sayrest

						outputto "TEMP_MixArtistJoin"
						json_select "join"
						ifnot ""
							ifnot ","
								say " "
							endif
							sayrest
							say " "
						else
							say ", "
						endif
					endif
				json_foreach_end

				outputto "COMPOSER"
				ifoutput "TEMP_Composer"
					sayoutput "TEMP_Composer"
					ifoutput "TEMP_IndexComposer"
						sayoutput "TEMP_IndexComposerJoin"
					endif
				endif
				sayoutput "TEMP_IndexComposer"
				say "|"

				outputto "MIXARTIST"
				ifoutput "TEMP_MixArtist"
					sayoutput "TEMP_MixArtist"
					ifoutput "TEMP_IndexMixArtist"
						sayoutput "TEMP_IndexMixArtistJoin"
					endif
				endif
				sayoutput "TEMP_IndexMixArtist"
				say "|"
			endif
		json_foreach_end
	endif

	set "TEMP_Composer"
	set "TEMP_MixArtist"

	if "track"
		json_select "position"
		findinline "-" 1 1
		outputto "TEMP_Track"
		movechar -1
		if "-"
			movechar 1
		else
			gotochar 1
		endif
		sayrest
		say "|"

		gotochar 1
		findinline "-" 1 1
		outputto "TEMP_Discnumber"
		movechar -1
		if "-"
			gotochar 1
			sayregexp ".+" "" "-"
		endif
		say "|"

		outputto "TEMP_Title"
		json_select "title"
		sayrest
		say "|"
		outputto "tracks"
		say "|"

		outputto "_LENGTH"
		json_select "duration"
		sayrest
		say "|"

		json_select_many "artists" "name" ", "
		ifnot ""
			outputto "ARTIST"
			set "TEMP_Join"
			json_foreach "artists"
				sayoutput "TEMP_Join"
				set "TEMP_Join"
				json_select "anv"
				ifnot ""
					sayrest
				else
					json_select "name"
					regexpreplace "\\s*\\(\\d+\\)$" ""
					sayrest
				endif

				outputto "TEMP_Join"
				json_select "join"
				ifnot ""
					ifnot ","
						say " "
					endif
					sayrest
					say " "
				endif
				outputto "ARTIST"
			json_foreach_end
		else
			outputto "ARTIST"
			sayoutput "TEMP_Artist"
		endif
		say "|"

		set "TEMP_ComposerJoin"
		set "TEMP_MixArtistJoin"
		json_foreach "extraartists"
			json_select "role"

			# Normalize Composer role variants used by Discogs
			regexpreplace "(?i).*composed[\s\.-]?by.*" "Composed-By"
			regexpreplace "(?i).*music[\s\.-]?by.*" "Composed-By"
			regexpreplace "(?i).*written[\s\.-]?by.*" "Composed-By"
			regexpreplace "(?i).*songwriter*" "Composed-By"

			regexpreplace "(?i).*remix.*" "Remix"

			if "Composed-By"
				outputto "TEMP_Composer"
				sayoutput "TEMP_ComposerJoin"
				set "TEMP_ComposerJoin"

				json_select "name"
				regexpreplace "\\s*\\(\\d+\\)$" ""
				sayrest

				outputto "TEMP_ComposerJoin"
				json_select "join"
				ifnot ""
					ifnot ","
						say " "
					endif
					sayrest
					say " "
				else
					say ", "
				endif
			endif
			if "Remix"
				outputto "TEMP_MixArtist"
				sayoutput "TEMP_MixArtistJoin"
				set "TEMP_MixArtistJoin"

				json_select "name"
				regexpreplace "\\s*\\(\\d+\\)$" ""
				sayrest

				outputto "TEMP_MixArtistJoin"
				json_select "join"
				ifnot ""
					ifnot ","
						say " "
					endif
					sayrest
					say " "
				else
					say ", "
				endif
			endif
		json_foreach_end

		outputto "COMPOSER"
		ifoutput "TEMP_Composer"
			sayoutput "TEMP_Composer"
			ifoutput "TEMP_IndexComposer"
				sayoutput "TEMP_IndexComposerJoin"
			endif
		endif
		sayoutput "TEMP_IndexComposer"
		say "|"

		outputto "MIXARTIST"
		ifoutput "TEMP_MixArtist"
			sayoutput "TEMP_MixArtist"
			ifoutput "TEMP_IndexMixArtist"
				sayoutput "TEMP_IndexMixArtistJoin"
			endif
		endif
		sayoutput "TEMP_IndexMixArtist"
		say "|"
	endif
json_foreach_end

outputto "Title"
sayoutput "TEMP_Title"

outputto "Track"
sayoutput "TEMP_Track"

outputto "Discnumber"
sayoutput "TEMP_Discnumber"

set "TEMP_Artist"
set "TEMP_Composer"
set "TEMP_ComposerJoin"
set "TEMP_Discnumber"
set "TEMP_IndexArtist"
set "TEMP_IndexComposer"
set "TEMP_IndexComposerJoin"
set "TEMP_IndexMixArtist"
set "TEMP_IndexMixArtistJoin"
set "TEMP_IndexTitle"
set "TEMP_Join"
set "TEMP_MixArtistJoin"
set "TEMP_Title"
set "TEMP_Track"