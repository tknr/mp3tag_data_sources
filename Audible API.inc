# Audible via API Tag Source for Mp3tag
#
# Initial release by Florian.
# See CHANGELOG.md for a list of changes.

[Name]=Audible API
[WordSeperator]=+
[IndexFormat]=%_coverurl%|%_preview%|%_url%|%Album%|%Author%|%Narrator%|%Duration%|%Year%|%Language%
[SearchBy]=%artist% $regexp(%album%,'[- ]+cd ?\d+$',,1)
[Settings]=A&udible API#Settings….settings
[Encoding]=url-utf-8
[MinAppVersionMac]=1.8.6
[MinAppVersionWin]=3.22
[Version]=0.3.0

[ParserScriptIndex]=...
#debug "on" "audible-api-index.txt"

replace "|" "$verticalBar()"
json "ON" "current"

json_select "total_results"
IfNot "0"
    json_foreach "products"
        json_select_object "product_images"
        json_select "100"
        SayRest
        json_unselect_object
        Say "|"

        json_select "asin"
        Say "https://www.audible.com/pd/"
        SayRest
        Say "|"

        json_select "asin"
        SayRest
        Say "?response_groups=category_ladders,contributors,media,product_desc,product_attrs,product_extended_attrs,rating,series&image_sizes=500,1000"
        Say "|"

        json_select "title"
        RegexpReplace "(.*?) \(Narrated by .*" "$1"
        SayRest
        Say "|"

        json_select_many "authors" "name" ", "
        SayRest
        Say "|"

        json_select_many "narrators" "name" ", "
        SayRest
        Say "|"

        json_select "runtime_length_min"
        SayDuration "m" 1
        Say "|"

        json_select "release_date"
        SayRest
        Say "|"

        json_select "language"
        SayRest

        SayNewLine
    json_foreach_end
EndIf


[ParserScriptAlbum]=...
#debug "on" "audible-api-album.txt"

replace "|" "$verticalBar()"

json "ON" "current"
json_select_object "product"

OutputTo "coverurl"
json_select_object "product_images"
json_select "1000"
SayRest
json_unselect_object

OutputTo "ASIN"
json_select "asin"
SayRest

OutputTo "ALBUM"
json_select "title"
RegexpReplace "(.*?) \(Narrated by .*" "$1"
SayRest

OutputTo "SUBTITLE"
json_select "subtitle"
SayRest

OutputTo "ALBUMARTIST"
json_select_many "authors" "name" ", "
SayRest

OutputTo "COMPOSER"
json_select_many "narrators" "name" ", "
SayRest

json_select_many_count "series"
IfNot ""
    OutputTo "SHOWMOVEMENT"
    say "1"

    OutputTo "SERIES"
    json_select_array "series" 1
    json_select "title"
    SayRest

    OutputTo "SERIES-PART"
    json_select "sequence"
    SayRest

    json_unselect_object

    OutputTo "CONTENTGROUP"
    SayOutput "SERIES"
    Say ", Book #"
    SayOutput "SERIES-PART"

    OutputTo "ALBUMSORT"
    SayOutput "SERIES"
    Say " "
    SayOutput "SERIES-PART"
    Say " - "
    SayOutput "ALBUM"
Else
    IfNotOutput "SUBTITLE"
        OutputTo "ALBUMSORT"
        SayOutput "ALBUM"
    Else
        OutputTo "ALBUMSORT"
        SayOutput "ALBUM"
        Say " - "
        SayOutput "SUBTITLE"
    EndIf
Endif

OutputTo "GENRE"
json_foreach "category_ladders"
IfVar "singleGenre" "true"
    json_foreach "ladder"
        IfNotOutput "GENRE"
            json_select "name"
            SayRest
        EndIf
    json_foreach_end
Else
    IfOutput "GENRE"
        IfVar "delimiter" "\\\\"
            Say "\\\\"
        EndIf
        IfVar "delimiter" ", "
            Say ", "
        EndIf
        IfVar "delimiter" ";"
            Say ";"
        EndIf
        IfVar "delimiter" "/"
            Say "/"
        EndIf
        IfVar "delimiter" "|"
            Say "$verticalBar()"
        EndIf
        IfVar "delimiter" " "
            Say " "
        EndIf
    EndIf
    json_select_many "ladder" "name" " / "
    SayRest
EndIf
json_foreach_end
json_select_object "product"

OutputTo "RATING"
json_select_object "rating"
json_select_object "overall_distribution"
json_select "display_average_rating"
SayRest
json_unselect_object
json_unselect_object

OutputTo "COMMENT"
json_select "publisher_summary"
KillTag "*"
Unspace
RegexpReplace "  +" " "
Replace " ," ","
SayRest

OutputTo "PUBLISHER"
json_select "publisher_name"
SayRest

OutputTo "YEAR"
json_select "release_date"
SayNChars 4

OutputTo "RELEASETIME"
json_select "release_date"
SayRest

OutputTo "ARTIST"
SayOutput "ALBUMARTIST"
IfVar "narratorInArtist" "true"
    Say ", "
    SayOutput "COMPOSER"
EndIf

OutputTo "WWWAUDIOFILE"
Say "https://www.audible.com/pd/"
SayOutput "ASIN"

OutputTo "DESCRIPTION"
SayOutput "COMMENT"

OutputTo "ITUNESMEDIATYPE"
Say "Audiobook"

OutputTo "ITUNESGAPLESS"
Say "1"

OutputTo "MOVEMENTNAME"
SayOutput "SERIES"

OutputTo "MOVEMENT"
SayOutput "SERIES-PART"