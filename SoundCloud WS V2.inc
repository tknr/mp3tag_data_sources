

#----------------------------------------------------------------------------#
#----------------------------------------------------------------------------#
#---------------- Soundcloud Web Source by Artist & Title -------------------#
#-------------------------- V1 by Audioman2000 ------------------------------#
# ------------------------ V2 by PagitosDiablos -----------------------------#
#----------------------------------------------------------------------------#
# Changelog:
# - Moved to parsing 'album' from the javascript SC hydration script.
# - Added multiple fields that can be collected from.  
# - 
#
#
#
#
#

[Name]=SoundCloud WS V2
[BasedOn]=www.soundcloud.com
[IndexUrl]=https://soundcloud.com/search?q=%s
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=% TITLE %|% UPLOADER %|%_URL%
[Encoding]=url-utf-8
[MinAppVersionWin]=3.32
[MinAppVersionMac]=1.8.4
[Settings]=SoundCloud V2#SoundCloud WS V2&SETTINGS.settings

#----------------------------------------------------------------------------#
#-------------------------------- I N D E X ---------------------------------#
#----------------------------------------------------------------------------#



[ParserScriptIndex]=...

findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</html>"

#------------------------- Remove unnecessary html --------------------------#

regexpreplace "<!DOCTYPE.+People<\/a><\/li><\/ul>" ""
regexpreplace "<\/noscript><style>.+<\/html>" ""


#----------- Transform html into json format for simple parsing -------------#


regexpreplace "<ul>\s+" "["
regexpreplace "\s+<\/ul>" "\]\}"
regexpreplace "<li><h2><a href=\"" ",{\"url\":\"https://soundcloud.com"
regexpreplace "\">" "\",\"title\":\""
regexpreplace "<\/a><\/h2><\/li>\s*" "\"}"
regexpreplace "\[," "\{\"array\":\["


#----------------------------- Parse results --------------------------------#

json "ON" "current"
json_foreach "array"

# TITLE #
json_select "title"
sayrest
say "|"

# UPLOADER #
json_select "url"
SayRegexp "(?<=https:\/\/soundcloud.com\/).+(?=\/)" "" ""
say "|"

# URL #
json_select "url"
sayrest
saynewline
json_foreach_end

#----------------------------------------------------------------------------#
#---------------------------------A L B U M ---------------------------------#
#----------------------------------------------------------------------------#

[ParserScriptAlbum]=...

debug "on" "\debug\debug_soundcloud.txt"

# Use hydration script to retrieve data #
findline "<script>window.__sc_hydration ="
findinline "<script>window.__sc_hydration ="
joinuntil ";</script>"
regexpreplace "<script>window.__sc_hydration =" ""
regexpreplace ";</script>" ""
unspace

json "on" "current"
json_select_array "" 9

# from JSON > data #
json_select_object "data"

    # Artwork #
    IfVar "SettingParseCoverImage" "true"
    outputto "COVERURL"
    json_select "artwork_url"
    Replace "large" "original"
    sayrest
    EndIf

    # Title #
    IfVar "SettingParseTitle" "true"
    outputto "TITLE"
    json_select "title"
    sayrest
    EndIf

    # Tags #
    #Always fill in tags. Uncheck in GUI to not use#
    outputto "TAGS"
    json_select "tag_list"
    RegexpReplace "(?<!\")\b([A-Za-z]+)\b(?!\")" "\"$1\""
    RegexpReplace "\s+\"" "\""
    RegexpReplace "\"\"" ", "
    RegexpReplace "\"" ""
    sayrest

    # Genre #
    IfVar "SettingGenreSource" "Genre (Top Right)"
    outputto "GENRE"
    json_select "genre"
    sayrest
    EndIf
 
    
    IfVar "SettingGenreSource" "All tags"
    outputto "GENRE"
    Use "%tags%"
    SayRest
    EndIf

    IfVar "SettingGenreSource" "First tag"
    outputto "GENRE"
    Use "%tags%"
    RegexpReplace ",.*$" ""
    SayRest
    EndIf
    # Genre - END #

    # Label / Publisher name #
    IfVar "SettingParseLabel" "true"
    outputto "PUBLISHER"
    json_select "label_name"
    sayrest
    EndIf

    # License #
    outputto "COPYRIGHT"
    json_select "license"

    # Permalink #
    IfVar "SettingParseUrl" "true"
    outputto "WWW"
    json_select "permalink_url"
    sayrest
    EndIf

#publisher_metadata#
json_select_object "publisher_metadata"

    #Artist of the original song remixed or bootlegged by uploader#
    outputto "ORIGARTIST"
    json_select "artist"
    sayrest

json_unselect_object

    # Purchase or download url #

    outputto "WWWPAYMENT"
    json_select "purchase_url"
    sayrest

    # Comment / Description #
    outputto "COMMENT"
    json_select "description"
    sayrest

json_select_object "user"

    # Artist #
    IfVar "SettingParseArtist" "true"
    outputto "ARTIST"
    json_select "username"
    sayrest
    EndIf

    #mixartist / remixer#
    IfVar "SettingParseMixArtist" "true"
    outputto "MIXARTIST"
    json_select "username"
    Sayrest
    EndIf

    #uploader url#
    outputto "WWWARTIST"
    json_select_object "user"
    json_select "permalink_url"
    sayrest

json_unselect_object

    # SUBTITLE / Mixname #
    outputto "SUBTITLE"
    json_select "title"
    SayRegexp "(?<=\().*\b"

    # formatted release date for Rekordbox #
    outputto "RELEASETIME"
    json_select "release_date"
    SayRegexp "\d\d\d\d-\d\d-\d\d" "" ""

    # YEAR #
    IfVar "SettingParseYear" "true"

    outputto "YEAR"
    json_select "release_date"
    RegexpReplace "T.+Z" ""

    IfVar "SettingFormatYear" "YYYY"
    SayRegexp "\d\d\d\d" "" ""
    EndIf

    IfVar "SettingFormatYear" "YYYY-MM-DD"
    SayRegexp "\d\d\d\d-\d\d-\d\d" "" ""
    EndIf

    IfVar "SettingFormatYear" "DD-MM-YYYY"
    RegexpReplace "(\d\d\d\d)-(\d\d)-(\d\d)" "$3-$2-$1"
    sayrest
    EndIf

EndIf


