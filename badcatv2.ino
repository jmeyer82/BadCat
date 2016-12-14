/*****************************************************************************
BadCat Motion Sensor 

This program uses a Photon, a PIR motion sensor & peizo buzzer from SparkFun
Kit to determine whether a cat has been detected on a surface such as a table
or counter and publishes events to IFFF to send SMS texts & notifications.

The code was created from a particle conference room tutorial & SPARKFUN SIK
Guide buzzer example. 
http://docs.particle.io/tutorials/topics/maker-kit.
******************************************************************************/

int buzzerPin = D1;                 // choose the pin for the Buzzer
int inputPin = D6;               // choose the PIR sensor pin
int motionCounter = 0;           // variable to count motion events
String status = "test";          // string to hold "available" or "in use",
                                 // "test" is for when it first turns on
const int songLength = 18;

char notes[] = "cdfda ag cdfdg gf "; // a space represents a rest

int beats[] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};

int tempo = 150; // determines how fast to play the beats

Timer timer(30000, determineMotion); // software timer to check every 30s

void setup() {
  pinMode(buzzerPin, OUTPUT);       // set buzzer as output
  pinMode(inputPin, INPUT);      // set sensor as input

  Particle.publish("cat_alert", status, PRIVATE); // send test publish

  timer.start(); // start the determineMotion timer
}

void determineMotion() {  // this function determines if there's motion.
    // if little or no motion detected and if status has changed:
    if(motionCounter < 2 && status != "Good Cat") {
        status = "Good Cat";                     // set status to "Good Cat",
        Particle.publish("cat_alert", status, PRIVATE);   // then publish 
    } // if motion was detected and status has changed:
    else if (motionCounter >= 2 && status != "Bad Cat") {
        status = "Bad Cat";                       // set status to "Bad Cat",
        Particle.publish("cat_alert", status, PRIVATE); // then publish
    }
    motionCounter = 0; // reset motion counter
}

void loop() {
  if (digitalRead(inputPin) == LOW) {  // if the sensor detected motion,
    digitalWrite(buzzerPin, HIGH);         // turn the Buzzer on
    motionCounter++; // increment motion counter
    playSong();
  } 
  else {
    digitalWrite(buzzerPin, LOW);          // turn Buzzer OFF if no motion
  }
 delay(1000);                          // wait 0.5s
}

void playSong() 
{
  int i, duration;
  
  for (i = 0; i < songLength; i++) // step through the song arrays
  {
    duration = beats[i] * tempo;  // length of note/rest in ms
    
    if (notes[i] == ' ')          // is this a rest? 
    {
      delay(duration);            // then pause for a moment
    }
    else                          // otherwise, play the note
    {
      tone(buzzerPin, frequency(notes[i]), duration);
      delay(duration);            // wait for tone to finish
    }
    delay(tempo/10);              // brief pause between notes
  }
  
  // We only want to play the song once, so we'll pause forever:
  //while(true){}
  // If you'd like your song to play over and over,
  // remove the above statement
}

int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.
  
  int i;
  const int numNotes = 8;  // number of notes we're storing
  
  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};
  
  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.
  
  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0
 delay(1000);
}
