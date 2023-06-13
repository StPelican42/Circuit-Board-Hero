#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
float X, Y, Z; // variables for accelerometer
int R,G,B,r,g,b;// variables to control LED colors
int iGlobal;// variable to keep track of iterations of the game loop 
int pitches[8]={60,62,64,66,68,70,72,74};// Array of pitches for use in the game
int correctNotes=0;// initializes the number of notes the user hits correctly at zero.
int totalNoteSequence[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};// intitializes an array to keep track of the sequence of notes to play in order.
int goodNoteSequence[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};// initializes an array to keep track of the notes to play in order in terms of 1-8, representing the possible directions and buttons the user inputs.
float midi[127];// Part of the midi code borrowed from Chet Udell, to be used for converting frequencies of the speaker into midi pitches for easier use.
double score;// intializes a score variable as a double: a decimal value.
int color;// intializes the integer variable responsible for coloring the LEDs in response to final score.
int A_four = 440; // a is 440 hz... //Part of the midi code borrowed from Chet Udell, to be used for converting frequencies of the speaker into midi pitches for easier use.
int level;// initialized variable for keeping track of what level the user chooses to play in.
int switchPin=7;// initialized variable for keeping track of what digital pin is used for the switch.
// Timing: 500ms is one beat.// Lines 16 and 17 are part of the midi code borrowed from Chet Udell, to be used for converting frequencies of the speaker into midi pitches for easier use.
const int beat = 500;//     ---------------^^^
bool correctNote,wrongNote;// boolean values for tracking user performance in the game.
int correctNoteCount=0;// integer variable for tracking how many notes the user has correctly
int tiltVar;// integer variable for tracking the overall tilt of the board.
float x,y;// float variables for tracking the overal tilt of x and y axes.
bool negX,negY;// booleans for dealing with negative values taken from the accelerometer.
volatile bool buttonAFlag, buttonBFlag;// flags that alert the code when buttons are pressed to trigger an action.
int noteNum;// integer variable representing what number note 1-8 is played.
int loopNum;// integer variable representing how many loops are needed for a specific level.
bool switchState;// boolean declaring which state the slide switch is in.
volatile bool soundFlag;// flag that alerts the code if the switch has been used in order to trigger an action.
int speedNum;// integer variable deciding how much time the user has to play each note.

AsyncDelay delay_1s; // initializing the AsyncDelay timer, used from the AsyncDelay library.

void setup() {
  Serial.begin(9600);// activates serial monitor
  CircuitPlayground.begin();// activates commands specific to the CircuitPlayground Express Board.
  generateMIDI(); // Custom function for calculating and storing the frequency value associated for each MIDI pitch number- borrowed from Chet Udell.
  randomSeed(analogRead(0)); // Seed random with garbage unknown analog reading
  attachInterrupt(digitalPinToInterrupt(4),AFlag,FALLING);// interrupts that trigger an Interrupt Service Routine called Aflag or BFlag whenever buttons 4 or 5 are pressed respectively.
  attachInterrupt(digitalPinToInterrupt(5),BFlag,FALLING);// ---^^^
  attachInterrupt(digitalPinToInterrupt(switchPin), off, CHANGE);// interrupt that triggers an Interrupt Service Routine called "off" when switch has been changed.
}

void loop() {
  switchState=CircuitPlayground.slideSwitch();// updates the switchState to update whether game is muted or not.
  level=1;// initializes level at 1, and score at 0.
  score=0;
  correctNotes=0;
  while(!Serial);// Waits until serial monitor is opened to start the game.
  Serial.println("C I R C U I T   B O A R D");// Game start screen
  Serial.println("         H E R O         ");
  Serial.println("_________________________");
  Serial.println("_________________________");
  Serial.println("Press Any Button to Start");
  menuSound();// Sound that means the game has started.
  delay(1000);// delay so things don't progress if user accidentally presses the button too fast.
  while(!digitalRead(4)&& !digitalRead(5)){// prevents game from proceeding until either button is pressed.
    sparkleTwo(20);// board LEDs light up with random colors while start screen is on.
    if (soundFlag){// checking for if switch has been activated for muting the game.
    delay(5);
    switchState=CircuitPlayground.slideSwitch();// updates the switchState when the flag is activated.
    soundFlag=0;
  }
  }
  selectSound();// Sound effect for moving to the next screen.
  delay(1000);//delay so things don't progress if user accidentally presses the button too fast.
  Serial.println("Tilt board in direction of arrows");// Directions for user to read before starting.
  Serial.println("Press left or right buttons to match left or right columns. Press as soon as they appear at bottom screen.");
  Serial.println("LEDS Reflect Score: Gold: 100%, Green: >90%, Green/Yellow: >80%, Orange: >70%, Orange/Red: >60%, Red: 60 and lower");
  Serial.println("Your Score = correct notes out of total notes");
  Serial.println("Press any button to begin.");
  while(!digitalRead(4)&& !digitalRead(5));// prevents game from proceeding until either button is pressed.
  selectSound();
  delay(1000);
  Serial.println("------------------------------------------");// further instructions for user before starting.
  Serial.println("Turn switch right to mute, left for sound.");
  Serial.println("Your Score = (correct notes/number of notes in round.");
  Serial.println("");
  Serial.println("Press left button for level 1, right button for level 2, or hold both at the same time for level 3.");
  if (soundFlag){// checking for if switch has been activated for muting the game.
    delay(5);
    switchState=CircuitPlayground.slideSwitch();// updates the switchState when the flag is activated.
    soundFlag=0;
  }
  while(!digitalRead(4)&& !digitalRead(5));
  if (digitalRead(4)){
    delay(200);
    if(digitalRead(5)){// if both buttons are activated at the same time, level is set to 3 with a faster speed and 20 notes instead of 10.
    level=3;
    loopNum=20;
    speedNum=1000;
    }else{// if only the left button is pressed, game starts at level 1 with 10 notes and 2 seconds to play each note.
     level=1;
    loopNum=10;
    speedNum=2000;
    }
  }else if(digitalRead(5)){
    delay(200);
      if(digitalRead(4)){// if both buttons are activated at the same time, level is set to 3 with a faster speed and 20 notes instead of 10.
       level=3;
       loopNum=20;
       speedNum=1000;
      }else{// if only the right button is pressed, game starts at level 2 with 10 notes and 1 second to play each note.
       level=2;
      loopNum=10;
      speedNum=1000;
    }
  }
  selectSound();// sound effect for when screen changes
  delay(1000);
  switch(level){// prints which level is selected so user knows what they are playing.
    case 0:
    break;
    case 1:
    Serial.println("Level 1");
    break;
    case 2:
    Serial.println("Level 2");
    break;
    case 3:
    Serial.println("Level 3");
    break;
    default:
    break;
  }
  Serial.println("Will start in 3");// Countdown for user to get ready
  if (switchState){
  CircuitPlayground.playTone(midi[pitches[0]],50);// speaker plays a beep for user to get a sense of when the game will start. Will not happen if game is muted.
  }
  delay(1000);
  Serial.println("              2");
  if (switchState){
  CircuitPlayground.playTone(midi[pitches[0]],50);
  }
  delay(1000);
  Serial.println("              1");
  if (switchState){
  CircuitPlayground.playTone(midi[pitches[0]],50);
  }
  delay(1000);
  Serial.println("            |            ");// Screen is cleared and ready for game to begin.
  Serial.println("            |            ");
  Serial.println("            |            ");
  Serial.println("            |            ");
  Serial.println("            |            ");
  for(int i=0;i<loopNum;i++){
  totalNoteSequence[i]=pitches[random(8)];// random sequence of pitches is generated for user to play.
} 
  goodNoteSeqGenerator();// the random sequence of pitches is converted into notes 1-8 for checking user input.
  for (int i = 0; i<loopNum; i++){// game begins
  
  iGlobal=i;// makes a copy of the iteration as a global variable for use with other functions.
  CircuitPlayground.clearPixels();// LEDs are cleared
  //Serial.println(goodNoteSequence[i]);// Line of code used for debugging-> prints the note number each iteration.
  symbols(goodNoteSequence[i]);// converts the note number 1-8 into a graphic with arrows for user to react to.   
  
  //Serial.println(totalNoteSequence[i]);// Line of code used for debugging-> prints the pitch value each iteration.
  delay_1s.start(speedNum, AsyncDelay::MILLIS);// timer for user to play correct note starts.
  while(!delay_1s.isExpired()){// before the timer stops, these things happen:
  X = CircuitPlayground.motionX();// The board's accelerometer returns values dictating which direction the board has been tilted in.
  Y = CircuitPlayground.motionY();//  ---^^^
  Z = CircuitPlayground.motionZ();//  ---^^^
  tiltCheck();//-------------------------^^^
  if(buttonAFlag || buttonBFlag){// if the interrupt for either button has been activated, check which button has been pressed and play the note accordingly.
  buttonCheck();//------------------^^^
  }
  // The timer has stopped.
  }
  if(correctNote){// if the note played was correct, the number of notes played correctly increases, the board lights up green as positive user feedback.
    correctNotes+=1;//----^^^
    lightUp(1);//---------^^^
    scoreUpdate();// The user's score is updated.
    correctNote=0;// the boolean checking if the note is correct is recet back to zero for the next note to be played.
    //Serial.println(correctNotes);// Line of code used for debugging-> prints the user's current count of correct notes played.

  }else{// if the note was played incorrectly, the board lights up red for user feedback.
    lightUp(0);
    //Serial.println(correctNotes);//Line of code used for debugging-> prints the user's current count of correct notes played.
  // Serial.print("X: ");// Line of code used for debugging-> Prints the values that the accelerometer returns when tilting.
  // Serial.print(X);//--------^^^
  // Serial.print("  Y: ");//--^^^
  // Serial.print(Y);//--------^^^
  // Serial.print("  Z: ");//--^^^
  // Serial.println(Z);//------^^^
  // Serial.print("  ");//-----^^^
  // Serial.print(tiltVar);//--^^^
  // Serial.print(" ");//------^^^
  // Serial.print(noteNum);// Line of code used for debugging-> Prints the note number 1-8 for the turn.

  //delay(1000);// Line of code used for debugging-> gives a delay between iterations.
}
}// Game is over
 scoreUpdate();// Score is updated with latest count of correct notes
 Serial.print("Your Score: ");// Prints the user's score out of how many notes were in that level.
 Serial.print(correctNotes);//---^^^
 Serial.print("  out of  ");//---^^^
 Serial.println(loopNum);//------^^^
Serial.println("Press any button to play again.");// Tells user to press button to play again.
delay(1000);// Delays code so accidental button press doesn't change the screen.
 while(!digitalRead(4)&& !digitalRead(5)){// LEDS sparkle a color or combination of colors reflecting the user's score until the user starts a new round.
   sparkle(20);
 }
}
// END OF GAME LOOP

void tiltCheck(){// Function for funneling accelerometer values into 4 discrete tilt directions.
  if (X<0){// if X is negative, it activates a boolean to keep track of that value and takes the absolute value of x-axis tilt.
    x=-X;
    negX=1;
  }else{
    x=X;
    negX=0;
  }
  if(Y<0){//  if Y is negative, it activates a boolean to keep track of that value and takes the absolute value of y-axis tilt.
    y=-Y;
    negY=1;
  }else{
    y=Y;
    negY=0;
  }
  if(x<2 && y<2){// If there isn't enough difference between x and y axis tilt, it will be regarded as no tilt at all.
    tiltVar=0;
  }else if (x>y){// Cases for when x-axis values are greater than y-axis values:
    if(negX){// If x is in the negative direction, the tilt is set to direction 2.
      tiltVar=2;
    }else{// If x was in the positive direction, the tilt is set to direction 4.
      tiltVar=4;
    }
  }else if(y>x){// Cases for when y-axis values are greater than x-axis values:
    if(negY){// If y is in the negative direction, the tilt is set to direction 1.
      tiltVar=1;
    }else{// If y is in the positive direction, the tilt is set to direction 3.
      tiltVar=3;
    }
  }else{// if none of these cases apply, there is no tilt.
    tiltVar=0;
  }
}// The directions go clockwise, so directions 1, 2, 3, and 4 correspond to tilting away from you, to the right, towards you, and to the left respectively.

void buttonCheck(){// Takes the direction value from tiltCheck and combines it with which button is pressed to give a note value.
  if (buttonAFlag){// If left button is pressed, tilt values 1-4 correspond to note values 1-4.
    switch(tiltVar){
      case 0:
        break;
      case 1:
        noteNum=1;
        break;
      case 2:
        noteNum=2;
        break;
      case 3:
        noteNum=3;
        break;
      case 4:
        noteNum=4;
        break;
      default:
        break;
    
    }

    }
  if (buttonBFlag){// If right button is pressed, direction values 1-4 correspond to button values 5-8 respectively.
    switch(tiltVar){
      case 0:
        break;
      case 1:
        noteNum=5;
        break;
      case 2:
        noteNum=6;
        break;
      case 3:
        noteNum=7;
        break;
      case 4:
        noteNum=8;
        break;
      default:
        break;
  
  }
  
  }
  playPitch();// Note values are then used to play the corresponding pitch through the speaker.
  correctNote=noteCheck(goodNoteSequence[iGlobal]);// sets the correctNote boolean to true or false if the note matches.
  noteNum=0;// resets the note number to get ready for next note
  buttonAFlag=0;// resets the flags for both buttons to get ready for next note
  buttonBFlag=0;//---^^^
}
void AFlag(){// Interrupt Service Routine triggered by button 4 being pressed.
  delay(5);// delay to debounce the button value
  buttonAFlag=1;// activates the buttonAFlag to trigger other actions.
}
void BFlag(){// Interrupt Service Routine triggered by button 5 being pressed.
  delay(5);// delay to debounce the button value
  buttonBFlag=1;// activates the buttonBFlag to trigger other actions.
}

void generateMIDI(){// generateMIDI function borrowed from Chet Udell for converting frequencies into midi pitches for easier use with the speaker.
  for (int x = 0; x < 127; ++x){
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    //Serial.println(midi[x]);// Line of code used for debugging-> prints each midi value created.
  }
}

void playPitch(){// Plays the pitch dictated by note values 1-8
  if (noteNum!=0){// Function continues only if noteNum is not zero, which means the board has to have SOME tilt.
  if(switchState){// the tone does not play if game is muted.
  CircuitPlayground.playTone(midi[(pitches[noteNum-1])], 50);// noteNum is used to access the corresponding midi value in the pitches array, which is then converted into a frequency using the midi function.
  }
}
}

bool noteCheck(int goodNote){// checks the note and returns true (1) or false (0) if the note played matches the correct expected note. Takes expected note value 1-8 as an input.
  if (noteNum==goodNote){// returns true if note values 1-8 match
    return 1;
    //break;
  }else{// returns false if notes do not match.
    return 0;
  }
}

int noteReturn(int num){// takes the number of the iteration of the loop the game is in, takes the pitch associated with that iteration, and returns the note value 1-8 associated with that pitch.
  for (int i=0;i<8;i++){
  if (pitches[i]==totalNoteSequence[num]){
    return (i+1);// returns i+1 because arrays work with values 0-7, and the rest of the code works with 1-8.
  }
  }

  }


void goodNoteSeqGenerator(){// takes the array of pitches in totalNoteSequence and creates an array with those same values in terms of note values 1-8.
  for (int i=0;i<loopNum;i++){
    goodNoteSequence[i]=noteReturn(i);
  }
}

void lightUp(int num){// Lights up the Circuit Playground LEDs green or red. Takes a 0 or 1 input.
  switch(num){
    case 0:// Zero means red. Lights all 10 NeoPixel LEDs red for 100 milliseconds and turns them off.
    for (int i=0;i<10;i++){
    CircuitPlayground.setPixelColor(i,255,0,0);
    }
    delay(100);
    for (int i=0;i<10;i++){
    CircuitPlayground.setPixelColor(i,0,0,0);
    }
    delay(100);
    break;
    case 1:// One means green. Lights all 10 NeoPixel LEDs green for 100 milliseconds and turns them off.
    for (int i=0;i<10;i++){
    CircuitPlayground.setPixelColor(i,0,255,0);
    }
    delay(100);
    for (int i=0;i<10;i++){
    CircuitPlayground.setPixelColor(i,0,0,0);
    }
    delay(100);
    break;
  }  
}

void symbols(int goodNote){// Takes the expected note value 1-8 as an input and returns an arrow graphic for the user to interpret.
  switch(goodNote){
    case 0:
    break;
    case 1:
    Serial.println("     ↑↑↑     |");
    break;
    case 2:
    Serial.println("     →→→     |");
    break;
    case 3:
    Serial.println("     ↓↓↓     |");
    break;
    case 4:
    Serial.println("     ←←←     |");
    break;
    case 5:
    Serial.println("             |     ↑↑↑     ");
    break;
    case 6:
    Serial.println("             |     →→→     ");
    break;
    case 7:
    Serial.println("             |     ↓↓↓     ");
    break;
    case 8:
    Serial.println("             |     ←←←     ");
    break;
    default:
    break;

  }// arrows on the left side means left button must be pressed, arrows on the right side means right button must be pressed. Arrow direction depicts tilt.
}

void scoreUpdate(){// Updates the user's score by dividing their number of correct notes by the total notes in the level. Sets what color the LEDS should be in response.
  double score = ((correctNotes*10)/loopNum);
  if ((score)==10){
    color=1;
  }else if ((score)>=9){
    color=2;
  }else if((score)>=8){
    color=3;
  }else if((score)>=7){
    color=4;
  }else if((score)>=6){
    color=5;
  }else{
    color=6;
  }
}

void sparkle(int num){// Takes the color value from scoreUpdate and lights the LEDs accordingly. RGB are the main RGB values, and rgb are the secondary values. Takes an integer input for length of each individual sparkle.
  switch(color){
    case 0:
    break;
    case 1:// Gold
    R=r=155;
    G=g=115;
    B=b=0;
    break;
    case 2:// Green
    R=r=0;
    G=g=255;
    B=b=0;
    break;
    case 3:// Green/Yellow
    R=0;
    G=g=135;
    B=b=0;
    r=255;
    break;
    case 4://Yellow
    R=r=255;
    G=100;
    B=b=0;
    g=100;
    break;
    case 5:// Orange/Yellow
    R=r=255;
    G=165;
    B=b=0;
    g=0;
    break;
    case 6:// Red
    R=r=255;
    G=g=0;
    B=b=0;
    break;
    default:
    break;
  }
  CircuitPlayground.clearPixels();
  for (int i=0;i<5;i++){// Lights up 3 random LEDs every 10 milliseconds and repeats 5 times.
    CircuitPlayground.setPixelColor(random(10), R, G, B);
    CircuitPlayground.setPixelColor(random(10), R, G, B);
    CircuitPlayground.setPixelColor(random(10), r, g, b);
    delay(num/2);
    CircuitPlayground.clearPixels();
    delay(num/2);
  }
}

void selectSound(){//Plays sequence of all 8 pitches from low to high.
  if(switchState){// Only plays if game isn't muted.
  for (int i=0;i<8;i++){
    CircuitPlayground.playTone(midi[(pitches[i])], 30);
  }
  sparkleTwo(20);//Lights up LEDs quickly for when game is muted.
}}

void menuSound(){// Plays a melody using pitches 1-8 when the game starts.
  if (switchState){
  //for (int i=0;i=3;i++){
    CircuitPlayground.playTone(midi[pitches[0]],50);
    CircuitPlayground.playTone(midi[pitches[1]],50);
    CircuitPlayground.playTone(midi[pitches[7]],150);
    CircuitPlayground.playTone(midi[pitches[4]],20);
    delay(30);
    CircuitPlayground.playTone(midi[pitches[5]],20);
    delay(30);
    CircuitPlayground.playTone(midi[pitches[6]],50);
    delay(30);
    CircuitPlayground.playTone(midi[pitches[5]],50);
    CircuitPlayground.playTone(midi[pitches[6]],150);
    delay(50);
    CircuitPlayground.playTone(midi[pitches[7]],250);


 // }
}}

void sparkleTwo(int num){// A shorter version of the sparkle function that uses random colors instead of the Gold, Green, Yellow, etc.
  CircuitPlayground.clearPixels();
  for (int i=0;i<5;i++){
    CircuitPlayground.setPixelColor(random(10), random(255), random(255), random(255));
    CircuitPlayground.setPixelColor(random(10), random(255), random(255), random(255));
    CircuitPlayground.setPixelColor(random(10), random(255), random(255), random(255));
    delay(num/2);
    CircuitPlayground.clearPixels();
    delay(num/2);
  }
}

void off(){// activates the flag for the switchState to be updated.
  soundFlag=1; // allows the program to check the new state of the switch
}
