```

#    _                   _                        _
#   | |                 | |                      | |
#   | |__    ___   __ _ | |_  _ __    ___   _ __ | |_     ___  ___   _ __ ___
#   | '_ \  / _ \ / _` || __|| '_ \  / _ \ | '__|| __|   / __|/ _ \ | '_ ` _ \
#   | |_) ||  __/| (_| || |_ | |_) || (_) || |   | |_  _| (__| (_) || | | | | |
#   |_.__/  \___| \__,_| \__|| .__/  \___/ |_|    \__|(_)\___|\___/ |_| |_| |_|      _
#                    (_)     | | |        | |                 | |                   | |
#    ___   ___  _ __  _  _ __|_| |_  ___  | |__   _   _   ___ | |_  ___ __   __ ___ | |__    ___  _ __  ___ TM
#   / __| / __|| '__|| || '_ \ | __|/ __| | '_ \ | | | | / __|| __|/ _ \\ \ / // _ \| '_ \  / _ \| '__|/ _ \
#   \__ \| (__ | |   | || |_) || |_ \__ \ | |_) || |_| | \__ \| |_|  __/ \ V /|  __/| | | ||  __/| |  | (_) |
#   |___/ \___||_|   |_|| .__/  \__||___/ |_.__/  \__, | |___/ \__|\___|  \_/  \___||_| |_| \___||_|   \___/
#                       | |                        __/ |
#                       |_|                       |___/
```
# You can buy me a 🍺 here as a thank you
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=EZVJN8ZLEU8KC&source=url

# Scripts homepage
https://community.mp3tag.de/t/beatport-com-wss-by-stevehero-release-single-track-artwork-tagging/12568

## HOW TO INSTALL
- Place scripts in `%appdata%\mp3tag\data\sources` directory. This includes the .inc and .scr files.

## UPDATE HISTORY
### **v5.12** 2021.06.08
- FIX:            Beatport had changing there pages slightly. joinuntil "</html>" changed to joinuntil "</footer>"

## UPDATE HISTORY
### **v5.11** 2021.05.07
- ADDED:            TRACK script fix for the search by scripts. There was no title in the list of search results page.

### **v5.10** 2021.03.23
- FIX:              TRACK script fix. BEATPORT_LABEL_URL, BEATPORT_RELEASE_ID, PUBLISHER, YEAR, DATE, UNSYNCEDLYRICS and WWW tags affected due to regression bug caused from earlier fix.

### **v5.09** 2021.03.03
- CHG:              COVERURL will get the highest possible artwork size available.

### **v5.08** 2021.02.11
- FIX:              Mix was getting back extra ( and ).

### **v5.07** 2021.02.10
- FIX:              WWW and BEATPORT_RELEASE_ID on the TRACK scripts were using the track ID as opposed to the release ID.

### **v5.06** 2021.02.09
- FIX:              Removed unwanted json_unselect_object in TRACK scripts.
- FIX:              replace "Original Mix" "\"\"" is now used as opposed replace "(Original Mix)" "".
- CHG:              Changed the amount of search results for TRACK scripts to 100 as opposed 25.

### **v5.05** 2021.02.09
- FIX:              _TIME_CHECK field was getting parsed when there was 1 and 10 to 19 tracks.
- FIX:              TITLE field was missing on both the TRACK and RELEASE scripts. Beatport changed something their end.

### **v5.04** 2020.11.10
- FIX:              Some short searches returned a 404 error even though they were on beatport.

### **v5.03** 2020.11.08
- ADDED:            Length to the list of search results dialog for the TRACK search scripts.
- ADDED:            Beatport by &stevehero v5.0#TRACK Search by Artist + Title script.

### **v5.02** 2020.11.08
- FIX:              _TIME_CHECK field wasn't getting output.

### **v5.01** 2020.11.08
- FIX:              Regular expression fixes to replace common unwanted words.

### **v5.0** 2020.11.03
- NEW RELEASE:      Complete rewrite for beatport.com. Script no longer uses the old classic site.

### **OLDER RELEASES**
- No longer applicable.