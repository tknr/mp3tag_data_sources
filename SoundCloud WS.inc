#----------------------------------------------------------------------------#
#----------------------------------------------------------------------------#
#---------------- Soundcloud Web Source by Artist & Title -------------------#
#----------------------------------------------------------------------------#
#----------------------------------------------------------------------------#

[Name]=SoundCloud WS
[BasedOn]=www.soundcloud.com
[IndexUrl]=https://soundcloud.com/search?q=%s
[AlbumUrl]=
[WordSeparator]=+
[IndexFormat]=% TITLE %|% UPLOADER %|%_URL%
[Encoding]=url-utf-8
[MinAppVersionWin]=3.22
[MinAppVersionMac]=1.8.4
[Settings]=SoundCloud WS &SETTINGS.settings

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

# TITLE
json_select "title"
sayrest
say "|"

# UPLOADER
json_select "url"
SayRegexp "(?<=https:\/\/soundcloud.com\/).+(?=\/)" "" ""
say "|"

# URL
json_select "url"
sayrest

saynewline
json_foreach_end

#----------------------------------------------------------------------------#
#---------------------------------A L B U M ---------------------------------#
#----------------------------------------------------------------------------#

[ParserScriptAlbum]=...

findline "<!DOCTYPE html>"
findinline "<!DOCTYPE html>"
joinuntil "</html>"

#------------------------- Remove unnecessary html --------------------------#

regexpreplace "<!DOCTYPE.+<header>" ""
regexpreplace "<section class=\"comments\">.+<\/html>" ""
regexpreplace "<\/header>" ""


#----------- Transform html into json format for simple parsing -------------#

regexpreplace "<h1.+?href.+?\">" "\{\"title\":\""
regexpreplace "<\/a>by.+?\">" "\",\"uploader\":\""
regexpreplace "<\/a><\/h1>.+?>" "\",\"release_date\":\""
regexpreplace "<\/time>.*<img src=" "\",\"image\":"
# Case 1: Genre on page
regexpreplace "(?<=\" ).*\">(.*)<\/a>" ",\"genre\":\"$1\"@"
# Case 2: No Genre on page
regexpreplace " width=.*" ",\"genre\":\"\"@"

regexpreplace "@.*" "\}"


#------------------------------- Parse Json ---------------------------------#

json "ON" "current"

IfVar "SettingParseTitle" "true"
outputto "TITLE"
json_select "title"
sayrest
EndIf

IfVar "SettingParseCoverImage" "true"
outputto "COVERURL"
json_select "image"
sayrest
EndIf

IfVar "SettingParseGenre" "true"
outputto "GENRE"
json_select "genre"
sayrest
EndIf

IfVar "SettingParseArtist" "true"
outputto "ARTIST"
json_select "uploader"
sayrest
EndIf

#-------------------- YEAR --------------------#
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