# iTunes API/Apple Music parser v 2.0 for MP3Tag v3
# © 2021 All Rights Reserved
[Name]=Apple Music
[BasedOn]=https://itunes.apple.com/
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=%_url%|%Artist%|%Album%|%_preview%|%Version%|%Tracks%|%Collection ID%|%Copyright%|%Store%|%Currency%|%Price%|%Year%|%Genre%
[Encoding]=url-utf-8

[ParserScriptIndex]=...

json "on"

# debug "on" "C:\debug-index.txt"
# DebugWriteInput "C:\debug-index-in.txt"

json_select "resultCount"
IfNot "0"
	json_foreach "results"
	json_select "collectionType"
	If "Album"

		json_select "collectionViewUrl"
		Replace "?uo=4" ""
		Replace "&uo=4" ""
		SayRest
		Say "|"

		json_select "artistName"
		SayRest
		Say "|"

		json_select "collectionName"
		If ""
			json_select "collectionCensoredName"
		EndIf
		SayRest
		Say "|"

		json_select "collectionViewUrl"
		Replace "?uo=4" ""
		Replace "&uo=4" ""
		SayRest
		Say "|"

		json_select "collectionExplicitness"
		Replace "notExplicit" ""
		Replace "explicit" "Explicit"
		Replace "cleaned" "Cleaned"
		SayRest
		Say "|"

		json_select "trackCount"
		SayNextNumber
		Say "|"

		json_select "collectionId"
		SayNextNumber
		Say "|"

		json_select "copyright"
		SayRest
		Say "|"

		json_select "country"
		SayRest
		Say "|"

		json_select "currency"
		SayRest
		Say "|"

		json_select "collectionPrice"
		SayRest
		Say "|"

		json_select "releaseDate"
		SayNextNumber
		Say "|"

		json_select "primaryGenreName"
		SayRest
		SayNewline

	EndIf
	If "Compilation"

		json_select "collectionViewUrl"
		Replace "?uo=4" ""
		Replace "&uo=4" ""
		SayRest
		Say "|"

		json_select "artistName"
		SayRest
		Say "|"

		json_select "collectionName"
		If ""
			json_select "collectionCensoredName"
		EndIf
		SayRest
		Say "|"

		json_select "collectionViewUrl"
		Replace "?uo=4" ""
		Replace "&uo=4" ""
		SayRest
		Say "|"

		json_select "collectionExplicitness"
		Replace "notExplicit" "C"
		Replace "explicit" "C Explicit"
		Replace "cleaned" "C Cleaned"
		SayRest
		Say "|"

		json_select "trackCount"
		SayNextNumber
		Say "|"

		json_select "collectionId"
		SayNextNumber
		Say "|"

		json_select "copyright"
		SayRest
		Say "|"

		json_select "country"
		SayRest
		Say "|"

		json_select "currency"
		SayRest
		Say "|"

		json_select "collectionPrice"
		SayRest
		Say "|"

		json_select "releaseDate"
		SayNextNumber
		Say "|"

		json_select "primaryGenreName"
		SayRest
		SayNewline

	EndIf
	json_foreach_end
EndIf

[ParserScriptAlbum]=...

FindLine "<script type=\"fastboot/shoebox\" id=\"shoebox-media-api-cache-amp-music\">"
FindInLine "<script type=\"fastboot/shoebox\" id=\"shoebox-media-api-cache-amp-music\">"
RegexpReplace "(?i).*?id=\"shoebox-media-api-cache-amp-music\">(\{.*\"\})<\/script>.+" "$1"
Replace "\\\\\\\"" "\u0022"
Replace "\\\"" "\""
Replace "\":\"{" "\":{"
Replace "}}]}\"" "}}]}"
Replace "]}}}]}\"}" "]}}}]}}"
RegexpReplace "\.catalog\.(.*?)\.albums\.(.*?)\.art" "mp3tagalbum"
Replace ".5burl.5d.f.extend.5balbums.5d.editorialartwork.extend.offers" ""
Replace ".2cpopularity" ""
Replace ".2ceditorialvideo.fields.5balbums.3aalbums.5d.artistname" ""
Replace ".2cartisturl.2cartwork" ""
Replace ".2ccontentrating.2ceditorialartwork" ""
Replace ".2ceditorialnotes" ""
Replace ".2cname.2cplayparams.2creleasedate" ""
Replace ".2curl.fields.5bartists.5d.name.2curl.fields.5brecord-labels" ""
Replace ".5d.name.2curl.include" ""
Replace ".5bmusic-videos.5d.artists.2ccomposers.include" ""
Replace ".5bsongs.5d.artists.2ccomposers.include.tracks" ""
RegexpReplace "2crecord-labels\.l\.(.*?)\.omit" "locale"
Replace ".2cartists.locale.5bresource" ""
Replace ".5d.autos.views.appears-on.2cmore-by-artist.2crelated-videos" ""
Replace ".2cother-versions.2cyou-might-also-like.2cvideo-extras.2caudio-extras" ""
Replace ".5d.autos.platform.web.views.appears-on.2cmore-by-artist.2crelated-videos" ""

RegexpReplace "storefronts\.(.*?)\"" "storefronts\""

json "on" "current"

# debug "on" "C:\debug-album.txt"
# DebugWriteInput "C:\debug-album-in.txt"

json_select_object ".storefronts"
	json_select_array "d" 1
		json_select "id"
		IfNot ""
			Replace "br" "143503"
			Replace "cn" "143465"
			Replace "dk" "143458"
			Replace "fi" "143447"
			Replace "fr" "143442"
			Replace "de" "143443"
			Replace "in" "143467"
			Replace "it" "143450"
			Replace "jp" "143462"
			Replace "kr" "143466"
			Replace "no" "143457"
			Replace "pl" "143478"
			Replace "pt" "143453"
			Replace "ru" "143469"
			Replace "es" "143454"
			Replace "se" "143456"
			Replace "tr" "143480"
			Replace "ae" "143481"
			Replace "gb" "143444"
			Replace "ua" "143492"
			Replace "us" "143441"
			Replace "vn" "143471"
		EndIf
		OutputTo "ITUNESCOUNTRYID"
		SayRest
	json_unselect_object
json_unselect_object

json_select "mp3tagalbum"
json_select_array "d" 1
json_select "type"
If "albums"
	json_select "id"
	OutputTo "ITUNESALBUMID"
	SayRest
	json_select_object "attributes"

		json_select_object "artwork"
			json_select "url"
			IfNot ""
				Replace "{w}" "10000"
				Replace "{h}" "10000"
				Replace "{f}" "jpg"
				OutputTo "COVERURL"
				SayRest
			EndIf
		json_unselect_object

		json_select "isCompilation"
		OutputTo "COMPILATION"
		SayRest

		json_select "artistName"
		OutputTo "ALBUMARTIST"
		Replace "Various Artists" ""
		Replace "Varios Artistas" ""
		Replace "Vários Artistas" ""
		Replace "Vários intérpretes" ""
		Replace "群星" ""
		Replace "群星" ""
		Replace "Eri Esittäjiä" ""
		Replace "Multi-interprètes" ""
		Replace "Verschiedene Interpreten" ""
		Replace "Verschillende artiesten" ""
		Replace "Artisti Vari" ""
		Replace "ヴァリアス・アーティスト" ""
		Replace "Diverse Artister" ""
		Replace "Разные артисты" ""
		Replace "Разные исполнители" ""
		Replace "Διάφοροι καλλιτέχνες" ""
		Replace "Blandade Artister" ""
		Replace "אמנים שונים" ""
		Replace "Çeşitli Sanatçılar" ""
		Replace "مختلف الفنانين" ""
		Replace "فنانون متنوعون" ""
		Replace "فنانون متعددون" ""
		Replace "Різні виконавці" ""
		Replace "รวมศิลปิน" ""
		Replace "Nhiều Nghệ Sĩ" ""
		If ""
			Set "COMPILATION"
			OutputTo "COMPILATION"
			Say "1"
			OutputTo "ALBUMARTIST"
			Say ""
		Else
			SayRest
		EndIf

		json_select_array "genreNames" -1 ", "
		OutputTo "ALBUMGENRES"
		SayRest

		json_select "trackCount"
		OutputTo "TOTALTRACKS"
		SayNextNumber

		json_select "releaseDate"
		OutputTo "YEAR"
		SayRest

		json_select "name"
		OutputTo "ALBUM"
		SayRest

		json_select "recordLabel"
		OutputTo "LABEL"
		SayRest

		json_select "upc"
		OutputTo "UPC"
		SayRest

		json_select "copyright"
		OutputTo "COPYRIGHT"
		SayRest

		json_select_object "editorialNotes"
			IfNot ""
			json_select "short"
			OutputTo "COMMENT"
			SayRest
			json_select "standard"
			OutputTo "DESCRIPTION"
			SayRest
			json_unselect_object
			EndIf

		json_select "contentRating"
		IfNot ""
			Replace "notExplicit" "0"
			Replace "explicit" "1"
			Replace "cleaned" "2"
			OutputTo "ALBUMADVISORY"
			SayRest
		EndIf

	json_unselect_object

	json_select_object "relationships"
		json_select_object "tracks"
			json_foreach "data"
				json_select "type"
				If "songs"

					json_select "id"
					OutputTo "ITUNESCATALOGID"
					SayNextNumber
					Say "|"

					json_select_object "attributes"

						json_select "artistName"
						OutputTo "ARTIST"
						SayRest
						Say "|"

						json_select "discNumber"
						OutputTo "DISCNUMBER"
						SayNextNumber
						Say "|"

						json_select "discNumber"
						Set "TOTALDISCS"
						OutputTo "TOTALDISCS"
						SayNextNumber
						Say "|"

						json_select_array "genreNames" 1
						OutputTo "GENRE"
						SayRest
						Say "|"

						json_select_array "genreNames" -1 ", "
						OutputTo "GENRES"
						SayRest
						Say "|"

						json_select "durationInMillis"
						OutputTo "_LENGTH"
						SayNextNumber
						Say "|"

						json_select "releaseDate"
						OutputTo "TRACKYEAR"
						SayRest
						Say "|"

						json_select "name"
						OutputTo "TRACKS"
						SayRest
						Say "|"

						json_select "isrc"
						OutputTo "ISRC"
						SayRest
						Say "|"

						json_select "trackNumber"
						OutputTo "TRACK"
						SayNextNumber
						Say "|"

						json_select "audioLocale"
						OutputTo "LANGUAGE"
						SayRest
						Say "|"

						json_select "workName"
						IfNot ""
							OutputTo "SHOWMOVEMENT"
							Say "1"
							Say "|"
							OutputTo "WORK"
							SayRest
							Say "|"
							json_select "movementName"
							IfNot ""
								OutputTo "MOVEMENTNAME"
								SayRest
								Say "|"						
								json_select "movementNumber"
								OutputTo "MOVEMENT"
								SayNextNumber
								Say "|"
								json_select "movementCount"
								OutputTo "MOVEMENTTOTAL"
								SayNextNumber
								Say "|"
							Else
								OutputTo "MOVEMENTNAME"
								json_select "name"
								FindInLine ": " 1 1
								SayRest
								Say "|"
								OutputTo "MOVEMENT"
								Say "|"
								OutputTo "MOVEMENTTOTAL"
								Say "|"
							EndIf
						Else
							OutputTo "SHOWMOVEMENT"
							Say "0"
							Say "|"
							OutputTo "WORK"
							Say "|"
							OutputTo "MOVEMENTNAME"
							Say "|"
							OutputTo "MOVEMENT"
							Say "|"
							OutputTo "MOVEMENTTOTAL"
							Say "|"
						EndIf

						json_select "composerName"
						OutputTo "COMPOSER"
						SayRest
						Say "|"

						json_select "attribution"
						OutputTo "ATTRIBUTION"
						SayRest
						Say "|"

						json_select "contentRating"
						OutputTo "ITUNESADVISORY"
						IfNot ""
							Replace "notExplicit" "0"
							Replace "explicit" "1"
							Replace "cleaned" "2"
							SayRest
						Else
							Say "0"
						EndIf
						Say "|"

					json_unselect_object
					json_select_object "relationships"

						json_select_object "composers"
							json_foreach "data"
								IfNot "0"
									json_foreach_end
									json_select_object "relationships"	# This must clearly be a bug in mp3tag
									json_select_object "composers"
									json_select_array "data" 1
										json_select "id"
										OutputTo "ITUNESCOMPOSERID"
										SayRest
										json_select_object "attributes"
											json_select "name"
											OutputTo "ITUNESCOMPOSERIDNAME"
											SayRest
										json_unselect_object
									json_unselect_object
									json_select_many "data" "id" ", "
										OutputTo "COMPOSERIDS"
										SayRest
								Else
									json_foreach_end
								EndIf
							OutputTo "ITUNESCOMPOSERID"
							Say "|"
							OutputTo "ITUNESCOMPOSERIDNAME"
							Say "|"
							OutputTo "COMPOSERIDS"
							Say "|"
						json_unselect_object

						json_select_object "artists"
							json_foreach "data"
								IfNot "0"
									json_foreach_end
									json_select_object "relationships"	# This must clearly be a bug in mp3tag
									json_select_object "artists"
									json_select_array "data" 1
										json_select "id"
										OutputTo "ITUNESARTISTID"
										SayRest
										json_select_object "attributes"
											json_select "name"
											OutputTo "ITUNESARTISTIDNAME"
											SayRest
										json_unselect_object
									json_unselect_object
									json_select_many "data" "id" ", "
										OutputTo "ARTISTIDS"
										SayRest
								Else
									json_foreach_end
								EndIf
							OutputTo "ITUNESARTISTID"
							Say "|"
							OutputTo "ITUNESARTISTIDNAME"
							Say "|"
							OutputTo "ARTISTIDS"
							Say "|"
						json_unselect_object

					json_unselect_object

					OutputTo "WWW"
					Say "|"

					OutputTo "VIDEO4K"
					Say "|"

					OutputTo "VIDEOHDR"
					Say "|"

					OutputTo "ITUNESMEDIATYPE"
					Say "Normal|"

				EndIf

				If "music-videos"

					json_select "id"
					OutputTo "ITUNESCATALOGID"
					SayNextNumber
					Say "|"

					json_select_object "attributes"

						json_select "artistName"
						OutputTo "ARTIST"
						SayRest
						Say "|"

						json_select_array "genreNames" 1
						OutputTo "GENRE"
						SayRest
						Say "|"

						json_select_array "genreNames" -1 ", "
						OutputTo "GENRES"
						SayRest
						Say "|"

						json_select "durationInMillis"
						OutputTo "_LENGTH"
						SayNextNumber
						Say "|"

						json_select "releaseDate"
						OutputTo "TRACKYEAR"
						SayRest
						Say "|"

						json_select "name"
						OutputTo "TRACKS"
						SayRest
						Say "|"

						json_select "isrc"
						OutputTo "ISRC"
						SayRest
						Say "|"

						json_select "trackNumber"
						OutputTo "TRACK"
						SayNextNumber
						Say "|"

						json_select "has4K"
						OutputTo "VIDEO4K"
						SayRest
						Say "|"

						json_select "hasHDR"
						OutputTo "VIDEOHDR"
						SayRest
						Say "|"

						json_select "url"
						OutputTo "WWW"
						SayRest
						Say "|"

						OutputTo "ITUNESMEDIATYPE"
						Say "Music Video|"

					json_unselect_object

					json_select_object "relationships"

						json_select_object "artists"
							json_foreach "data"
								IfNot "0"
									json_foreach_end
									json_select_object "relationships"	# This must clearly be a bug in mp3tag
									json_select_object "artists"
									json_select_array "data" 1
										json_select "id"
										OutputTo "ITUNESARTISTID"
										SayRest
										json_select_object "attributes"
											json_select "name"
											OutputTo "ITUNESARTISTIDNAME"
											SayRest
										json_unselect_object
									json_unselect_object
									json_select_many "data" "id" ", "
										OutputTo "ARTISTIDS"
										SayRest
								Else
									json_foreach_end
								EndIf
							OutputTo "ITUNESARTISTID"
							Say "|"
							OutputTo "ITUNESARTISTIDNAME"
							Say "|"
							OutputTo "ARTISTIDS"
							Say "|"
						json_unselect_object

					json_unselect_object

					OutputTo "ATTRIBUTION"
					Say "|"

					OutputTo "COMPOSER"
					Say "|"

					OutputTo "COMPOSERIDS"
					Say "|"

					OutputTo "DISCNUMBER"
					Say "|"

					OutputTo "ITUNESADVISORY"
					Say "|"

					OutputTo "ITUNESCOMPOSERID"
					Say "|"

					OutputTo "ITUNESCOMPOSERIDNAME"
					Say "|"

					OutputTo "LANGUAGE"
					Say "|"

					OutputTo "MOVEMENT"
					Say "|"

					OutputTo "MOVEMENTNAME"
					Say "|"

					OutputTo "MOVEMENTTOTAL"
					Say "|"

					OutputTo "SHOWMOVEMENT"
					Say "|"

					OutputTo "WORK"
					Say "|"

				EndIf
			json_foreach_end
		json_unselect_object
	json_unselect_object
EndIf
