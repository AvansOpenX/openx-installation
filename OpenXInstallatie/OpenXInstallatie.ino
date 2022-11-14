#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

#define EEPROM_SIZE 1024

#define BTN_PIN_1 18
#define BTN_PIN_2 2
#define BTN_PIN_3 15
#define BTN_PIN_4 4
#define BTN_PIN_5 19
#define BTN_PIN_6 17
#define BTN_PIN_7 0
#define BTN_PIN_8 16

#define LED_PIN_1 33
#define LED_PIN_2 14
#define LED_PIN_3 26
#define LED_PIN_4 32
#define LED_PIN_5 27
#define LED_PIN_6 13
#define LED_PIN_7 25
#define LED_PIN_8 12

#define PLAYCOUNT_ADDRESS 0
#define HIGHSCORE_ADDRESS 1

byte zero[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte one[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte two[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte three[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte four[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

byte five[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// set the LCD number of columns and rows and the address
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

long gameLength = 30000; // 30 seconds
long idleLength = 500;
int currentScore = 0;

class Led {
  private:
    byte pin;
  
  public:
    Led(byte pin) {
      this->pin = pin;
      init();
    }
  
    void init() {
      pinMode(pin, OUTPUT);
      off();
    }
  
    void on() {
      digitalWrite(pin, HIGH);
    }
  
    void off() {
      digitalWrite(pin, LOW);
    }
};

class Button {
  private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
  
  public:
  	Led *led;
  
    Button(byte pin, byte ledPin) {
      this->pin = pin;
      this->led = new Led(ledPin);
      lastReading = LOW;
      init();
    }
  
    void init() {
      pinMode(pin, INPUT_PULLUP);
      update();
    }
  
    void update() {
      byte newReading = digitalRead(pin);
      if (newReading != lastReading) {
        lastDebounceTime = millis();
      }
      if (millis() - lastDebounceTime > debounceDelay) {
        state = newReading;
      }
      lastReading = newReading;
    }
  
    byte getState() {
      update();
      return state;
    }
  
    bool isPressed() {
      return (getState() != HIGH);
    }
};

Button buttons[] = {
  Button(BTN_PIN_1, LED_PIN_1),
  Button(BTN_PIN_2, LED_PIN_2),
  Button(BTN_PIN_3, LED_PIN_3),
  Button(BTN_PIN_4, LED_PIN_4),
  Button(BTN_PIN_5, LED_PIN_5),
  Button(BTN_PIN_6, LED_PIN_6),
  Button(BTN_PIN_7, LED_PIN_7),
  Button(BTN_PIN_8, LED_PIN_8),
};
  
void setup() {
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE);

  lcd.init();
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Plays: ");
  lcd.print(EEPROM.read(PLAYCOUNT_ADDRESS));
  lcd.setCursor(0, 1);
  lcd.print("Highscore: ");
  lcd.print(EEPROM.read(HIGHSCORE_ADDRESS));
  lcd.setCursor(0, 2);
  lcd.print("Score: 0");
}

void loop() {
  idleAnimation();
  if (buttons[0].isPressed()) runGame();
}

void idleAnimation() {
  static bool ledsOn;
  unsigned static long previousBlink = millis();
  if (millis() - previousBlink >= idleLength) {
    previousBlink = millis();
  	for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
      if (ledsOn) buttons[i].led->off();
      else buttons[i].led->on();
  	}
    ledsOn = !ledsOn;
  }
}

void runGame() {
  for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
    buttons[i].led->off();
  }

  // Increment playcount
  int playCount = EEPROM.read(PLAYCOUNT_ADDRESS);
  playCount++;
  EEPROM.write(PLAYCOUNT_ADDRESS, playCount);
  EEPROM.commit();
  lcd.setCursor(7, 0);
  lcd.print(playCount);

  // Clear old score
  lcd.setCursor(7, 2);
  lcd.print("0      ");

  delay(1000);

  gameLoop();

  for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
    buttons[i].led->on();
  }

  // Clear progress bar
  lcd.setCursor(0, 3);
  lcd.write("                    ");

  // Save highscore
  if (EEPROM.read(HIGHSCORE_ADDRESS) < currentScore) {
    EEPROM.write(HIGHSCORE_ADDRESS, currentScore);
    EEPROM.commit();
    lcd.setCursor(11, 1);
    lcd.print(currentScore);
  }

  currentScore = 0;
  delay(2000);
}

void gameLoop() {
  int activeButton = 0;
  buttons[activeButton].led->on();
  unsigned long startTime = millis();
  // Start looping for as long as a game should last
  while(millis() - startTime < gameLength) {
    // Detect whether the correct button has been pressed
    if (buttons[activeButton].isPressed()) {
      currentScore++;
      lcd.setCursor(7, 2);
      lcd.print(currentScore);
      buttons[activeButton].led->off();
      // Select a new button to press
      while(true) {
      	int nextButton = random(sizeof buttons/sizeof buttons[0] - 1);
        if (nextButton != activeButton) {
          activeButton = nextButton;
          break;
        }
      }
      buttons[activeButton].led->on();
      delay(50);
    }
    updateProgressBar(millis() - startTime, gameLength, 3);
  	delay(15);
  }
}

void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn) {
  double factor = totalCount / 100.0;          
  int percent = (count + 1) / factor;
  int number = percent / 5;
  int remainder = percent % 5;
  if(number > 0) {
    lcd.setCursor(number - 1, lineToPrintOn);
    lcd.write(5);
  }  
  lcd.setCursor(number, lineToPrintOn);
  lcd.write(remainder);   
}
