# GPS speedometer and Coordinate logger

## Features

* Show speed in km/h
  * Take speed from TinyGPS libary and display it on OLED using Adafruits library
  * '~' will be displayed on screen when speed logging is working
  * ~~TODO switch to a smaller OLED library.~~ DONE
* log coordinates to a sd card
  * These files will be stored in a CSV format
  * Google maps accepts csv files with the headers of name,desc,latitude,longitude
    * Wherein Name will be time string, desc will be speed in km/h 
  * Will be using https://www.google.com/maps/d/u/0/
* have battery life of at least 1 - 2hrs
  * 100mah LiPo with charge and boost controller will give around 1hr. 
  
  ![Fritzing wiring](https://github.com/Evantm/GPS-Speed-Logger-and-Tracker/blob/master/Fritzing%20wiring.png "Wiring Diagram")



## Problems encountered 

* SD Card module does not play nice with Adafruit's SSD1306 OLED library. Fixed by switching to SSD1306Ascii library from github user grieman. This has benifits of reduced size. 	

* SD Card initialization problems. Fixed by putting SD card init in a loop. Fromating SD card fixes any other bugs.

