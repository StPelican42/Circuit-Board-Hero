# ENGR103-Final-Project
A game to be played on the Adafruit Circuit Playground Express Board using the Arduino IDE
# Author
Noah Fontenot

# Circuit Board Hero
A handheld game on the Adafruit Circuit Playground Express Board inspired by the video game Guitar Hero.

# Controls
Hold the board with the USB port facing away from you. Tilt the board to the left, right, away from you, or toward you based on what directions you are given. Press the left or right buttons to play different notes. Each combination of tilt and button plays a different note, with a total of 8 different possible notes to play.
# Rules
When starting, the Arduino IDE Serial Monitor will print instructions for the game. Press either button to start. Notes will appear from the bottom to the top of the screen, and you will have until the note disappears in the top of the Serial Monitor to play it correctly. Notes will be represented as directions (Left, Right, Up, Down) on one of two sides in the serial monitor.
If a direction is displayed on the left side of the serial monior, the correct note will be played using the left button. Likewise, if a direction is displayed on the right side of the serial monitor, the correct note will be played using the right button. Each round uses a sequence of 10 randomly picked notes. 
Your score will be represented by the percentage of notes you play correctly. Your percentage of correct notes played will be represented by the flashing LEDs on your board. Gold is 100% correct, Green is at least 90%, Green and yellow is at least 80%, yellow and orange is at least 70%, orange and red is at least 60%, and anything below 60% is red. At the end of a sequence, your score will be reported in the serial monitor as a percentage of correct notes played out of the sequence, and you can press either button to play again. 
# Inputs
The board's accelerometer, ranging from -20 to 20 in each axis. Used to determine which direction the board is tilting.
The board's buttons, having values of 0 or 1. Used to determine what note to play and activates notes to be played.
The board's slide switch, used to toggle having sound or not.
# Outputs
The board's speaker used for playing notes in the range of midi pitches between 60 and 74. (Frequencies between 261.63 and 587.33 Hz)
The board's LEDs with RGB ranges from 0 to 255, used to display colors in response to the player's performance
Serial monitor: used to print instructions, display notes, and display a final score. Initialized at 9600 baud.

# Updates
6/12/2023- Added a mute mode: slide the switch on the board and the game mutes for when you want to play without sound. Switch it back to get sound back.
         - Added levels: 1, 2, and 3.

