# ###################################################################
# Mp3tag Tag Source for Cover Art from Discogs API (Official)
#
# Search by is specified in host tag source that includes this file.
# 
# This file should be stored in your tag sources sources directory
# %APPDATA%\Mp3tag\data\sources
# and requires Mp3tag v2.97b or above.
#
# [2024-05-28] v1.08 NEW: added column Cover with cover dimensions to list of query results.
# [2024-05-19] v1.07 NEW: added column BARCODE to list of query results.
# [2024-05-15] v1.06 FIX: release titles containing hyphens were not displayed correctly in query results list.
# [2024-05-13] v1.05 NEW: added support for cover thumbnails in list of query results. 
# [2019-09-09] v1.04 CHG: moved common Discogs tag source structure to this tag source include file.
# [2018-05-07] v1.03 CHG: added all formats and catalog number
# [2017-06-30] v1.02 CHG: added fallback detection of secondary images if no primary image exists
# [2017-06-27] v1.01 CHG: improved detection if release has cover art
# [2017-06-26] v1.00
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

		# Check if release has cover art
		json_select "thumb"

		ifnot ""

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
			say "http://www.discogs.com/release/"
			json_select "id"
			sayrest

			saynewline

		endif

	json_foreach_end
endif


[ParserScriptAlbum]=...
# ###################################################################
#					A  L  B  U  M
# ###################################################################
json "ON"

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