# Building instructions
## Materials
1x arduino (any will do - I'm using a Pro Mini)
57x WS2812/NeoPixel cut in separate strips with 4 in each segment. leave a single for decimal separator
??x connection wire (the led strips need 3 connections between each segment)
1x Front plate cut from SVG or DVG file
1x Back plate cut from SVG or DVG file
optional: opaque acrylic cut from the same pattern as the Back file.

##Program the arduino
Just like the maslow firmware we need to program the arduino. In the repository there is a .ino file which you will have to program to the arduino board of your choice.
In the first few lines of code there are a few definitions of how many pixels per segment (default is 4) and which pin you want to connect to. I chose pin 6 because - default. you can change it to whatever you want. 
Remember: If you change the number of pixels per segment you will have to update the offset values in the code. 

##Assemble the display
1. Place the front piece on top of the back piece. Using a pencil lineout the cutouts for each segment.
2. on the back-side of the front piece you will have to carve a channel for the wiring. it's not important where the channel is but from experience it's easiest if you try to hit the middle of the segments where possible
Remember: If you change the segment orientation which is quite likely if you don't route the segments like I have - you will need to update the segment-offset values in the code. othervise the segment decoder will not work properly.
3. solder time! connect all the segments with Ground, Power and Signal wires. the last pixel is the decimal indicator.
