#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
float X, Y, Z;
int R,G,B,r,g,b;
int iGlobal;
int pitches[8]={60,62,64,66,68,70,72,74};
int correctNotes=0;
int totalNoteSequence[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int goodNoteSequence[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
float midi[127];
double score;
int color;
int A_four = 440; // a is 440 hz...
int level;
int switchPin=7;
// Timing: 500ms is one beat.
const int beat = 500;
bool correctNote,wrongNote;
int correctNoteCount=0;
int tiltVar;
float x,y;
bool negX,negY;
volatile bool buttonAFlag, buttonBFlag;
int noteNum;
int loopNum;
bool switchState;
volatile bool soundFlag;
int speedNum;

AsyncDelay delay_1s;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  generateMIDI(); // Custom function for calculating and storing the frequency value associated for each MIDI pitch number
  randomSeed(analogRead(0)); // Seed random with garbage unknown analog reading
  attachInterrupt(digitalPinToInterrupt(4),AFlag,FALLING);
  attachInterrupt(digitalPinToInterrupt(5),BFlag,FALLING);
  attachInterrupt(digitalPinToInterrupt(switchPin), off, CHANGE);
}

void loop() {
  if (soundFlag){
    delay(5);
    switchState=CircuitPlayground.slideSwitch();// updates the switchState when the flag is activated.
    soundFlag=0;
  }
  level=1;
  score=0;
  correctNotes=0;
  while(!Serial);
  Serial.println("C I R C U I T   B O A R D");
  Serial.println("         H E R O         ");
  Serial.println("_________________________");
  Serial.println("_________________________");
  Serial.println("Press Any Button to Start");
  menuSound();
  delay(1000);
  while(!digitalRead(4)&& !digitalRead(5)){
    sparkleTwo(20);
  }
  selectSound();
  delay(1000);
  Serial.println("Tilt board in direction of arrows");
  Serial.println("Press left or right buttons to match left or right columns. Press as soon as they appear at bottom screen.");
  Serial.println("LEDS Reflect Score: Gold: 100%, Green: >90%, Green/Yellow: >80%, Orange: >70%, Orange/Red: >60%, Red: 60 and lower");
  Serial.println("Your Score = (correct notes/10)*100");
  Serial.println("Press any button to begin.");
  while(!digitalRead(4)&& !digitalRead(5));
  selectSound();
  delay(1000);
  Serial.println("------------------------------------------");
  Serial.println("Turn switch left to mute, right for sound.");
  Serial.println("Your Score = (correct notes/number of notes in round.");
  Serial.println("");
  Serial.println("Press left button for level 1, right button for level 2, or hold both at the same time for level 3.");
  while(!digitalRead(4)&& !digitalRead(5));
  if (digitalRead(4)){
    delay(200);
    if(digitalRead(5)){
    level=3;
    loopNum=20;
    speedNum=1000;
    }else{
     level=1;
    loopNum=10;
    speedNum=2000;
    }
  }else if(digitalRead(5)){
    delay(200);
      if(digitalRead(4)){
       level=3;
       loopNum=20;
       speedNum=1000;
      }else{
       level=2;
      loopNum=10;
      speedNum=1000;
    }
  }
  selectSound();
  delay(1000);
  switch(level){
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
  Serial.println("Will start in 3");
  if (switchState){
  CircuitPlayground.playTone(midi[pitches[0]],50);
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
  Serial.println("            |            ");
  Serial.println("            |            ");
  Serial.println("            |            ");
  Serial.println("            |            ");
  Serial.println("            |            ");
  for(int i=0;i<loopNum;i++){
  totalNoteSequence[i]=pitches[random(8)];
} 
  goodNoteSeqGenerator();
  for (int i = 0; i<loopNum; i++){
  
  iGlobal=i;
  CircuitPlayground.clearPixels();
  //Serial.println(goodNoteSequence[i]);
  symbols(goodNoteSequence[i]);   
  
  //Serial.println(totalNoteSequence[i]);
  delay_1s.start(speedNum, AsyncDelay::MILLIS);
  while(!delay_1s.isExpired()){
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
  tiltCheck();
  if(buttonAFlag || buttonBFlag){
  buttonCheck();
  }
  
  }
  if(correctNote){
    correctNotes+=1;
    lightUp(1);
    scoreUpdate();
    correctNote=0;
    Serial.println(correctNotes);

  }else{
    lightUp(0);
    Serial.println(correctNotes);
  // Serial.print("X: ");
  // Serial.print(X);
  // Serial.print("  Y: ");
  // Serial.print(Y);
  // Serial.print("  Z: ");
  // Serial.println(Z);
  // Serial.print("  ");
  // Serial.print(tiltVar);
  // Serial.print(" ");
  // Serial.print(noteNum);

  //delay(1000);
}
}
 scoreUpdate();
 Serial.print("Your Score: ");
 Serial.print(correctNotes);
 Serial.print("  out of  ");
 Serial.println(loopNum);
 //Serial.println(double(correctNotes/2));
Serial.println("Press any button to play again.");
 while(!digitalRead(4)&& !digitalRead(5)){
   sparkle(20);
 }
 selectSound();
}


void tiltCheck(){
  if (X<0){
    x=-X;
    negX=1;
  }else{
    x=X;
    negX=0;
  }
  if(Y<0){
    y=-Y;
    negY=1;
  }else{
    y=Y;
    negY=0;
  }
  if(x<2 && y<2){
    tiltVar=0;
  }else if (x>y){
    if(negX){
      tiltVar=2;
    }else{
      tiltVar=4;
    }
  }else if(y>x){
    if(negY){
      tiltVar=1;
    }else{
      tiltVar=3;
    }
  }else{
    tiltVar=0;
  }
}

void buttonCheck(){
  if (buttonAFlag){
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
  if (buttonBFlag){
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
  playPitch();
  correctNote=noteCheck(goodNoteSequence[iGlobal]);
  noteNum=0;
  buttonAFlag=0;
  buttonBFlag=0;
}
void AFlag(){
  delay(5);
  buttonAFlag=1;
}
void BFlag(){
  delay(5);
  buttonBFlag=1;
}

void generateMIDI(){
  for (int x = 0; x < 127; ++x){
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    //Serial.println(midi[x]);
  }
}

void playPitch(){
  if (noteNum!=0){
  if(switchState){
  CircuitPlayground.playTone(midi[(pitches[noteNum-1])], 50);
  }
}
}

bool noteCheck(int goodNote){
  if (noteNum==goodNote){
    return 1;
    //break;
  }else{
    return 0;
  }
}

int noteReturn(int num){
  for (int i=0;i<8;i++){
  if (pitches[i]==totalNoteSequence[num]){
    return (i+1);
  }
  }

  }


void goodNoteSeqGenerator(){
  for (int i=0;i<loopNum;i++){
    goodNoteSequence[i]=noteReturn(i);
  }
}

void lightUp(int num){
  switch(num){
    case 0:
    for (int i=0;i<10;i++){
    CircuitPlayground.setPixelColor(i,255,0,0);
    }
    delay(100);
    for (int i=0;i<10;i++){
    CircuitPlayground.setPixelColor(i,0,0,0);
    }
    delay(100);
    break;
    case 1:
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

void symbols(int goodNote){
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

  }
}

void scoreUpdate(){
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

void sparkle(int num){
  switch(color){
    case 0:
    break;
    case 1:// Gold
    R=r=255;
    G=g=215;
    B=b=0;
    break;
    case 2:// Green
    R=r=0;
    G=g=255;
    B=b=0;
    break;
    case 3:// Green/Yellow
    R=0;
    G=g=235;
    B=b=0;
    r=255;
    break;
    case 4://Yellow
    R=r=255;
    G=100;// used to be 255
    B=b=0;
    g=100;// used to be 165
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
  for (int i=0;i<5;i++){
    CircuitPlayground.setPixelColor(random(10), R, G, B);
    CircuitPlayground.setPixelColor(random(10), R, G, B);
    CircuitPlayground.setPixelColor(random(10), r, g, b);
    delay(num/2);
    CircuitPlayground.clearPixels();
    delay(num/2);
  }
}

void selectSound(){
  if(switchState){
  for (int i=0;i<8;i++){
    CircuitPlayground.playTone(midi[(pitches[i])], 30);
  }
  sparkleTwo(20);
}}

void menuSound(){
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

void sparkleTwo(int num){
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
