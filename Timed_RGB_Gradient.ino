/*
  Timed RGB LED Gradient
  Change the color of an RGB LED along fixed gradient steps over a provided timeframe.
*/

int redPin = 0;
int greenPin = 1;
int bluePin = 2;

int gradient[11][3] = {
  {0, 255, 0},
  {50, 255, 0},
  {100, 255, 0},
  {150, 255, 0},
  {200, 255, 0},
  {255, 255, 0},
  {255, 200, 0},
  {255, 150, 0},
  {255, 100, 0},
  {255, 50, 0},
  {255, 0, 0},
};
int gradientLength = 11 - 1;
int activeColorIndex = 0;

volatile uint8_t counter = 0;
unsigned int milliseconds = 0;
unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int hours = 0;
unsigned int days = 0;
unsigned int durationInSeconds = 0;

unsigned int gradientDurationMilliSeconds = 0;
unsigned int gradientDurationSeconds = 0;
unsigned int gradientDurationMinutes = 0;
unsigned int gradientDurationHours = 0;
unsigned int gradientDurationDays = 2;
unsigned int gradientDurationInSeconds = gradientDurationSeconds + (gradientDurationMinutes * 60) + (gradientDurationHours * 60 * 60) + (gradientDurationDays * 60 * 60 * 24);

void setup()
{
  TIMSK |= _BV(OCIE0A); // Turn on the compare interrupt (below!)

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()
{
  // Get current duration in seconds
  durationInSeconds = seconds + (minutes * 60) + (hours * 60 * 60) + (days * 60 * 60 * 24);
  
  // Get completion percentage
  double completionPercentage = (double) durationInSeconds / gradientDurationInSeconds;

  // Get active color gradient index
  activeColorIndex = (int) floor(completionPercentage * gradientLength);

  // Clamp value to highest color index
  if (activeColorIndex > gradientLength) {
    activeColorIndex = gradientLength;

    // Flash on completion
    if (seconds % 2 == 0) {
      int empty[] = {0, 0, 0};
      setColor(empty);
  
      return;
    }
  }

  setColor(gradient[activeColorIndex]);
}

void setColor(int color[])
{
  int red = 255 - color[0];
  int green = 255 - color[1];
  int blue = 255 - color[2];

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;

  //count milliseconds every 20 milliseconds
  if (counter >= 20) {
    counter = 0;
    milliseconds = milliseconds + 20;
  }

  if(milliseconds >= 1000) {
    milliseconds = 0;
    seconds = seconds + 1;
  }

  if (seconds >= 60) {
    seconds = 0;
    minutes = minutes + 1;
  }

  if (minutes >= 60) {
    minutes = 0;
    hours = hours + 1;
  }

  if (hours >= 24) {
    hours = 0;
    days = days + 1;
  }
}
