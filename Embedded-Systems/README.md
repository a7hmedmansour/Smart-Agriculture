# Smart-Agriculture-Project
my graduation project for Faculty of Computers and Artificial Intelligence -Fayoum University
with help of arduino uno + esp-01 i introduce this project that do the following
gather information from sensor (soil moisture-Temperature and humidity-rain -tank-flame)
its take decsions at iot application on thingspeak platform which put commands in talkback queue that can be (turn_ON_pump)or (turn_off_pump)
or (close_roof) or (open_roof) according to data esp01 send .
esp have serial interface with arduino uno which with its help can send data of sensors and listen to talkback queue in same http post request
turn on pump when the soil moisture level is high means plant wants water othewise turn off pump and close roof when rain sensor detect rain and the soil moisture level is low means plants dont want rain water or the temperature is too high as for specifi plants data otherwise the roof is open
note that all of this controled by an mobile application whether user can select plant he grow or custom plant 
note that all libraries for sensors/motors/interrput are done using c language for customizing and gain more control on system
