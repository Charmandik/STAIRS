#include <FastLED.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <SettingItem.h>
#include <PGMWrap.h>

#define LED         13
#define LED_PIN     17
#define NUM_LEDS    13
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define LIGHTSENSOR A1
#define SWITCHER    A2
#define REDPIN 9
#define BLUEPIN 11
#define GREENPIN 10

CRGB leds[NUM_LEDS];

uint8_t STEPS = 18;

#define SETTINGS 1
#define WORKING 2
#define LIGHTUP  78
#define LIGHTDOWN  79
#define LIGHTCROSS  80
#define STAIRSOFF  81
#define STAIRSON  53
#define OFFUP  99
#define  OFFDOWN 84
#define  OFFCROSS 85

const uint8_t  encPinA = 2;
const uint8_t  encPinB = 3;
const uint8_t  BUTTON = 14;

bool changed = false;

uint8_t counterA = 0;
uint8_t counterB = 0;
uint8_t counterC = 18;
uint8_t counterD = 0;
uint8_t randColor;
uint8_t stepsVal[18];
long timeoutStart;
long currentTime;
uint8_t currentState; //состояние - лестница включена, загорается вверх, вниз, выключается вверх, вниз, вся, навстречу
uint8_t BotMoveSensor;   //датчик приблоижения нижний
uint8_t TopMoveSensor;   //датчик приближения верхний
uint8_t buffBot;   //датчик приблоижения нижний
uint8_t buffTop;   //датчик приближения верхний
uint8_t light1;   //датчик освещения нижний
uint8_t light2;   //датчик освещения верхний
uint8_t light;        //уровень освещения текущий
uint8_t oldLight;
uint8_t switch1;   //проходной
uint8_t switched = 0;
uint8_t mode; //Режим - ночь вкл, день выкл, настройки
uint8_t btn_flag = 0;
uint8_t setting_id;

SettingItem BRIGHTNESS;  //Общая яркость лестниц
SettingItem SPEED;     //Скорость увеличения яркости
SettingItem ANIMATION;//Выбор типа анимации
SettingItem COLOR;    //Выбор цвета
SettingItem ADVANCED;  //Расширенные настройки
SettingItem STEPNUMBER;  //Регулировка количества ступеней
SettingItem DELAY_TIME;  //Задержка до выключения
SettingItem LIGHTSENSORS;  // Выбор работающих датчиков  освещенности
SettingItem LUMINOSITY; // уровень освещенности, при котором начинает работать контроллер в ночном режиме
SettingItem NIGHT_BRIGHTNESS;//Уровень яркости ночной подсветки
SettingItem SENSORS_DISTANCE; //Этот пункт позволяет производить калибровку расстояния, на котором срабатывают датчики
SettingItem ADAPTIVE;//Адаптивная яркость - вкл/выкл
SettingItem EXIT;//Выход
SettingItem COLOR_MODE;
SettingItem COLOR_SPEED;
SettingItem TEMPERATURE;
SettingItem RED_COLOR;
SettingItem GREEN_COLOR;
SettingItem BLUE_COLOR;
SettingItem HUE_COLOR;
SettingItem SATURATION_COLOR;

SettingItem *CurrentItem  = new SettingItem();



volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
//volatile byte val = 0;
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent


uint8_t* steps[39] = {
  &leds[0].r, &leds[0].g, &leds[0].b,
  &leds[1].r, &leds[1].g, &leds[1].b,
  &leds[2].r, &leds[2].g, &leds[2].b,
  &leds[3].r, &leds[3].g, &leds[3].b,
  &leds[4].r, &leds[4].g, &leds[4].b,
  &leds[5].r, &leds[5].g, &leds[5].b,
  &leds[6].r, &leds[6].g, &leds[6].b,
  &leds[7].r, &leds[7].g, &leds[7].b,
  &leds[8].r, &leds[8].g, &leds[8].b,
  &leds[9].r, &leds[9].g, &leds[9].b,
  &leds[10].r, &leds[10].g, &leds[10].b,
  &leds[11].r, &leds[11].g, &leds[11].b,
  &leds[12].r, &leds[12].g, &leds[12].b,
};

LiquidCrystal_I2C LCD(0x27, 16, 2);

byte wrench[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b01110,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};
byte sun[8] = {
  0b00000,
  0b10101,
  0b01110,
  0b11011,
  0b01110,
  0b10101,
  0b00000,
  0b00000
};
byte moon[8] = {

  0b00011,
  0b00110,
  0b11000,
  0b11000,
  0b11000,
  0b00110,
  0b00011,
  0b00000
};

void InitializeSetting()
{
  BRIGHTNESS.setrange(0, 32);
  SPEED.setrange(1, 16);
  ANIMATION.setrange(0, 3);
  COLOR.setrange(0, 1);
  ADVANCED.setrange(0, 1);
  STEPNUMBER.setrange(3, 18);
  DELAY_TIME.setrange(1, 60);
  NIGHT_BRIGHTNESS.setrange(0, 100);
  LIGHTSENSORS.setrange(0, 4);
  LUMINOSITY.setrange(0, 100);
  SENSORS_DISTANCE.setrange(0, 2);
  ADAPTIVE.setrange(0, 1);
  EXIT.setrange(0, 2);
  COLOR_MODE.setrange(0, 3);
  COLOR_SPEED.setrange(1, 16);
  TEMPERATURE.setrange(0, 7);
  RED_COLOR.setrange(0, 255);
  GREEN_COLOR.setrange(0, 255);
  BLUE_COLOR.setrange(0, 255);
  HUE_COLOR.setrange(0, 255);
  SATURATION_COLOR.setrange(0, 255);

  BRIGHTNESS.nextSettingItem = &SPEED;
  SPEED.nextSettingItem = &ANIMATION;
  ANIMATION.nextSettingItem = &COLOR;
  COLOR.nextSettingItem = &COLOR_MODE;
  COLOR_MODE.nextSettingItem = &RED_COLOR;
  RED_COLOR.nextSettingItem = &GREEN_COLOR;
  GREEN_COLOR.nextSettingItem = &BLUE_COLOR;
  BLUE_COLOR.nextSettingItem = &HUE_COLOR;
  HUE_COLOR.nextSettingItem = &SATURATION_COLOR;
  SATURATION_COLOR.nextSettingItem = &COLOR_SPEED;
  COLOR_SPEED.nextSettingItem = &TEMPERATURE;
  TEMPERATURE.nextSettingItem = &ADVANCED;
  ADVANCED.nextSettingItem = &STEPNUMBER;
  STEPNUMBER.nextSettingItem = &DELAY_TIME;
  DELAY_TIME.nextSettingItem = &LIGHTSENSORS;
  LIGHTSENSORS.nextSettingItem = &LUMINOSITY;
  LUMINOSITY.nextSettingItem = &NIGHT_BRIGHTNESS;
  NIGHT_BRIGHTNESS.nextSettingItem = &SENSORS_DISTANCE;
  SENSORS_DISTANCE.nextSettingItem = &ADAPTIVE;
  ADAPTIVE.nextSettingItem = &EXIT;
  EXIT.nextSettingItem = &BRIGHTNESS;

  BRIGHTNESS.setvalue(32);
  SPEED.setvalue(6);
  ANIMATION.setvalue(0);
  COLOR.setvalue(0);
  COLOR_MODE.setvalue(1);
  HUE_COLOR.setvalue(0);
  SATURATION_COLOR.setvalue(255);
  COLOR_SPEED.setvalue(1);
  TEMPERATURE.setvalue(0);
  ADVANCED.setvalue(0);
  STEPNUMBER.setvalue(18);
  DELAY_TIME.setvalue(2);
  LIGHTSENSORS.setvalue(0);
  LUMINOSITY.setvalue(0);
  NIGHT_BRIGHTNESS.setvalue(0);
  SENSORS_DISTANCE.setvalue(0);
  ADAPTIVE.setvalue(1);
  EXIT.setvalue(0);
}

void changeVal(short number, uint8_t counter)
{
  stepsVal[counter] = constrain(stepsVal[counter] + number, 0, 255);
}

bool checkON(uint8_t value)
{
  uint8_t counter = 0;
  for (uint8_t i = 0; i < STEPNUMBER.value; i++)
  {
    if (stepsVal[i] == value)
    {
      counter++;

    }
  }
  if (counter == STEPNUMBER.value)
    return true;
  else return false;
}

void updateLeds()
{
  for (uint8_t i = 0; i < sizeof(stepsVal); i++)
    *steps[i] = stepsVal[i];
}

void setSteps(int8_t value)
{
  for (uint8_t i = 0; i < 18; i++)
    stepsVal[i] = 0;
  for (uint8_t i = 0; i < STEPNUMBER.value; i++)
    stepsVal[i] = value;
}

void getSensors()
{
  uint8_t bytenum = 0;
  Wire.requestFrom(0x38, 2);
  while (Wire.available())
  {
    char c = Wire.read();
    bytenum++;
    if (bytenum == 1)
    {
      if (c == 1) buffBot++;
      if (c == 0) buffBot = 0;
      if (buffBot > 0) {
        BotMoveSensor = 1;
        Serial.println("Bot Sensor ON");
      }
      if (buffBot == 0) {
        BotMoveSensor = 0;
      }
    }
    if (bytenum == 2)
    {
      light1 = c;
    }
  }

  bytenum = 0;
  Wire.requestFrom(0x39, 2);
  while (Wire.available())
  {
    char c = Wire.read();
    bytenum++;
    if (bytenum == 1)
    {
      if (c == 1) buffTop++;
      if (c == 0) buffTop = 0;
      if (buffTop > 0) {
        TopMoveSensor = 1;
        Serial.println("TOP Sensor ON");
      }
      if (buffTop == 0) TopMoveSensor = 0;
    }
    if (bytenum == 2)
    {
      light2 = c;
    }
  }

  oldLight = light;
  switch (LIGHTSENSORS.value)
  {
    case (0):
      light = 0;
      break;
    case (1):
      light = map(light2, 0, 127, 0, 100);
      break;
    case (2):
      light =  map(light1, 0, 127, 0, 100);
      break;
    case (3):
      light = ( map(light1, 0, 127, 0, 100) + map(light2, 0, 127, 0, 100)) / 2;
      break;
    case (4):
      light = map(constrain(analogRead(LIGHTSENSOR), 0, 919), 0, 919, 0, 127);
      break;
  }
  if ((oldLight != light) && (CurrentItem == &LUMINOSITY))
    LCD_renew();
}

void readSettings()
{
  CurrentItem = &BRIGHTNESS;
  for (uint8_t i = 0; i < 8; i++) //EEPROM
  {
    CurrentItem->setvalue(EEPROM.read(i));
    CurrentItem = CurrentItem->nextSettingItem;
  }
}

void saveSettings()
{
  CurrentItem = &BRIGHTNESS;
  for (uint8_t i = 0; i < 8; i++) //EEPROM
  {
    if (EEPROM.read(i) != CurrentItem->value)
      EEPROM.write(i, CurrentItem->value);
    CurrentItem = CurrentItem->nextSettingItem;
  }
}

void updateSettings()
{
  if (ADAPTIVE.value == 0)
  {
    uint8_t coeff = map(light, 0, 127, 127, 32);
    uint16_t correctedBrightness = (BRIGHTNESS.value * 7) * 32 / coeff;
    if (((BRIGHTNESS.value * 7) > 0) & (correctedBrightness == 0)) correctedBrightness = (BRIGHTNESS.value * 7);
    FastLED.setBrightness(correctedBrightness);
  }
  else
    FastLED.setBrightness(BRIGHTNESS.value * 7);
}

void stepsWork()
{
  switch (currentState)
  {
    case (STAIRSOFF):

      setSteps(0);
      break;
    case (STAIRSON):
      setSteps(255);
      break;

    case (LIGHTUP) :
      for (uint8_t i = 0 ; i < STEPNUMBER.value; ++i)
      {
        if (stepsVal[i] < 255)
        {
          if (ANIMATION.value != 3)
            changeVal(SPEED.value, i);
          if ((ANIMATION.value == 1) && (STEPNUMBER.value - i > (STEPNUMBER.value / 4)))
          {
            for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
            {
              if (stepsVal[i] - stepsVal[k + i] >= 63 * k)
                changeVal(SPEED.value / 2, k + i);
            }
          }
          else if (ANIMATION.value == 2)
          {
            changeVal(255, i);
            delay(255 / SPEED.value);
          }
          else if (ANIMATION.value == 3)
          {
            if (counterA <= STEPNUMBER.value)
            {
              for (uint8_t w = 0; w < STEPNUMBER.value - counterB; w++)
              {
                if (w != counterA)
                  changeVal(-255, w);
              }
              changeVal(255, counterA);
              delay(500 / SPEED.value);
              counterA = constrain(counterA + 1, 0, STEPNUMBER.value);
              if (counterA == STEPNUMBER.value)
              {
                counterA = 0;
                counterB = constrain(counterB + 1, 2, STEPNUMBER.value);
                delay(500 / SPEED.value);
              }
              if (counterB == STEPNUMBER.value)
              {
                counterB = 0;
                delay(100);
              }
            }
          }
          if (checkON(255))
          {
            timeoutStart = currentTime;
            Serial.println("h");
          }
          break;
        }
      }
      break;

    case (LIGHTDOWN):
      for (uint8_t i = STEPNUMBER.value - 1 ; i >= 0; --i)
      {
        if (stepsVal[i] < 255)
        {
          if (ANIMATION.value != 3)
            changeVal(SPEED.value, i);
          if ((ANIMATION.value == 1) && (i >= (STEPNUMBER.value / 4)))
          {
            for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
            {
              if (stepsVal[i] - stepsVal[i - k] >= 63 * k)
                changeVal(SPEED.value / 2, i - k);
            }
          }
          else if (ANIMATION.value == 2)
          {
            changeVal(255, i);
            delay(255 / SPEED.value);
          }
          else if (ANIMATION.value == 3)
          {
            for (uint8_t w = STEPNUMBER.value - 1; w > counterD; w = constrain(w - 1, 0, 17))
            {
              if (w != counterC - 1)
                changeVal(-255, w);
            }
            counterC = constrain(counterC - 1, 0, 18);
            changeVal(255, counterC);
            delay(500 / SPEED.value);
            if (counterC == 0)
            {
              counterC = 18;
              counterD = constrain(counterD + 1, 0, 17);
              delay(500 / SPEED.value);
            }
          }
          if (checkON(255))
          {
            if (mode != WORKING)
              counterD = 0;
            timeoutStart = currentTime;
          }
          break;
        }
      }
      break;

    case (LIGHTCROSS): //thought about 1 loop, but it's not efficient
      for (uint8_t i = 0 ; i < STEPNUMBER.value; ++i)
      {
        if (stepsVal[i] < 255)
        {
          if (ANIMATION.value != 3)
            changeVal(SPEED.value, i);
          if ((ANIMATION.value == 1) && (STEPNUMBER.value - i > (STEPNUMBER.value / 4)))
          {
            for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
            {
              if (stepsVal[i] - stepsVal[k + i] >= 63 * k)
                changeVal(SPEED.value / 2, k + i);
            }
          }
          else if (ANIMATION.value == 2)
          {
            changeVal(255, i);
            delay(255 / SPEED.value);
          }
          else if (ANIMATION.value == 3)
          {
            for (uint8_t w =  STEPNUMBER.value / 2; w < STEPNUMBER.value - counterB; w++)
            {
              if (w != counterA + 1)
                changeVal(-255, w);
            }
            counterA = constrain(counterA + 1, STEPNUMBER.value / 2 - 1, STEPNUMBER.value - 1);
            changeVal(255, counterA);
            delay(100);

            if (counterA == STEPNUMBER.value - 1)
            {
              counterA =  STEPNUMBER.value / 2 - 1;
              counterB = constrain(counterB + 1, 2, STEPNUMBER.value / 2);
            }
          }
          if (checkON(255))
            timeoutStart = currentTime;
          break;
        }
      }

      for (uint8_t i = STEPNUMBER.value - 1 ; i > 0; --i)
      {
        if (stepsVal[i] < 255)
        {
          if (ANIMATION.value != 3)
            changeVal(SPEED.value, i);

          if ((ANIMATION.value == 1) && (i >= (STEPNUMBER.value / 4)))
          {
            for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
            {
              if (stepsVal[i] - stepsVal[i - k] >= 63 * k)
                changeVal(SPEED.value / 2, i - k);
            }
          }
          else if (ANIMATION.value == 2)
          {
            changeVal(255, i);
            delay(255 / SPEED.value);
          }
          else if (ANIMATION.value == 3)
          {
            for (uint8_t w = STEPNUMBER.value / 2; w > counterD; w = constrain(w - 1, 0, 9))
            {
              if (w != counterC - 1)
                changeVal(-255, w);
            }
            counterC = constrain(counterC - 1, 0, STEPNUMBER.value / 2);
            changeVal(255, counterC);
            delay(100);

            if (counterC == 0)
            {
              counterC = STEPNUMBER.value / 2;
              counterD = constrain(counterD + 1, 0, STEPNUMBER.value / 2);
            }
          }
          if (checkON(255))
          {
            timeoutStart = currentTime;
          }
          break;
        }
      }

      break;

    case (OFFDOWN):
      for (uint8_t i = STEPNUMBER.value - 1 ; i >= 0; --i)
      {
        if ((stepsVal[i] > 0) && ((stepsVal[0] >= NIGHT_BRIGHTNESS.value) && (stepsVal[STEPNUMBER.value - 1] >= NIGHT_BRIGHTNESS.value)))
        {
          if (ANIMATION.value != 3)
            changeVal(-(SPEED.value), i);
          switch (ANIMATION.value)
          {
            case (1):
              if (i > STEPNUMBER.value / 4)
              {
                for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
                {
                  if (stepsVal[i] - stepsVal[i - k] <= 63 * k)
                    changeVal(-SPEED.value / 2, i - k);
                }
              }
              break;
            case (2):
              changeVal(-255, i);
              delay(255 / SPEED.value);
              break;


          }
          break;
        }
      }
      break;

    case (OFFUP):

      for (uint8_t i = 0 ; i < STEPNUMBER.value; ++i)
      {
        if ((stepsVal[i] > 0) && ((stepsVal[0] >= NIGHT_BRIGHTNESS.value) && (stepsVal[STEPNUMBER.value - 1] >= NIGHT_BRIGHTNESS.value)))
        {
          if (ANIMATION.value != 3)
            changeVal(-(SPEED.value), i);
          switch (ANIMATION.value)
          {
            case (1):
              if (STEPNUMBER.value - i > STEPNUMBER.value / 4)
                for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
                {
                  if (stepsVal[k + i] - stepsVal[i]   >= 63 * k)
                    changeVal(-SPEED.value / 2, k + i);
                }
              break;
            case (2):
              changeVal(-255, i);
              delay(255 / SPEED.value);
              break;
          }
          break;
        }
      }
      break;

    case (OFFCROSS):
      for (uint8_t i = STEPNUMBER.value / 2 ; i <= STEPNUMBER.value; ++i)
      {
        if ((stepsVal[i] > 0) && ((stepsVal[0] >= NIGHT_BRIGHTNESS.value) && (stepsVal[STEPNUMBER.value - 1] >= NIGHT_BRIGHTNESS.value)))
        {
          if (ANIMATION.value != 3)
            changeVal(-(SPEED.value), i);
          switch (ANIMATION.value)
          {
            case (1):
              if (STEPNUMBER.value - i > STEPNUMBER.value / 4)
                for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
                {
                  if (stepsVal[k + i] - stepsVal[i]   >= 63 * k)
                    changeVal(-SPEED.value / 2, k + i);
                }
              break;
            case (2):
              changeVal(-255, i);
              delay(255 / SPEED.value);
              break;
          }
          break;
        }
      }

      for (uint8_t i = STEPNUMBER.value / 2 - 1 ; i >= 0; --i)
      {
        if ((stepsVal[i] > 0) && ((stepsVal[0] >= NIGHT_BRIGHTNESS.value) && (stepsVal[STEPNUMBER.value - 1] >= NIGHT_BRIGHTNESS.value)))
        {
          if (ANIMATION.value != 3)
            changeVal(-(SPEED.value), i);
          switch (ANIMATION.value)
          {
            case (1):
              if (i > STEPNUMBER.value / 4)
              {
                for (uint8_t k = 1 ; k <= (STEPNUMBER.value / 4); k++)
                {
                  if (stepsVal[i] - stepsVal[i - k] <= 63 * k)
                    changeVal(-SPEED.value / 2, i - k);
                }
              }
              break;
            case (2):
              changeVal(-255, i);
              delay(255 / SPEED.value);
              break;

          }
          break;
        }
      }
      break;

  }
}

void updateStairsState()
{
  switch1 = analogRead(SWITCHER);
  if (switch1 < 100)
  {
    currentState  = STAIRSON;
    switched = 1;
  }
  if ((switch1 >= 100) && (switched))
  {
    currentState = STAIRSOFF;
    switched = 0;
  }

  switch (currentState)
  {
    case (STAIRSON):

      if (checkON(255))
        if (currentTime - timeoutStart > (DELAY_TIME.value * 1000))
          currentState = STAIRSOFF;
      break;

    case (STAIRSOFF):

      if (TopMoveSensor) currentState = LIGHTDOWN;
      if (BotMoveSensor) currentState = LIGHTUP;
      break;

    case (LIGHTUP):

      if ((TopMoveSensor) && (!checkON(255))) currentState = LIGHTCROSS;
      if ((checkON(255)) && ((currentTime - timeoutStart > (DELAY_TIME.value * 1000)) || (TopMoveSensor)))
      {
        currentState = OFFUP;
      }
      break;

    case (LIGHTDOWN):
      if ((BotMoveSensor) && (!checkON(255))) currentState = LIGHTCROSS;
      if ((checkON(255)) && ((currentTime - timeoutStart > (DELAY_TIME.value * 1000)) || (BotMoveSensor)))
      {
        currentState = OFFDOWN;
      }
      break;

    case (LIGHTCROSS):
      if ((checkON(255)) && (currentTime - timeoutStart > (DELAY_TIME.value * 1000)))
        currentState = OFFCROSS;
      break;

    case (OFFCROSS):
      if (checkON(0))
      {
        randColor = random(255);
        currentState = STAIRSOFF;
        
      }
      break;

    case (OFFDOWN):
      if (checkON(0))
      {
         randColor = random(255);
        currentState = STAIRSOFF;
       
      }
      break;

    case (OFFUP):
      if (checkON(0))
      {
         randColor = random(255);
        currentState = STAIRSOFF;
        
      }
      break;
  }

}

void PinA() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    //*
    encoderPos++; //increment the encoder's position count
    if (CurrentItem == &NIGHT_BRIGHTNESS) CurrentItem->changevalue(5);
    else if (mode == SETTINGS) CurrentItem->changevalue(1);

    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos--; //decrement the encoder's position count
    if (CurrentItem == &NIGHT_BRIGHTNESS) CurrentItem->changevalue(-5);
    else if (mode == SETTINGS) CurrentItem->changevalue(-1);

    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void button()
{
  if (mode == WORKING)
  {
    mode = SETTINGS;
    CurrentItem = &BRIGHTNESS;
    setting_id = 0;
    LCD.backlight();
    LCD_renew();
  }
  else
  {
    changed = false;
    if (CurrentItem == &EXIT)
    {
      switch (CurrentItem->value)
      {
        case (0):
          saveSettings();
          mode = WORKING;
          LCD.noBacklight();
          break;
        case (1):
          readSettings();
          mode = WORKING;
          LCD.noBacklight();
          break;
        case (2):
          setting_id = 0;

          CurrentItem = &BRIGHTNESS;
          changed = true;
          break;
      }
    }
    else if (CurrentItem == &ADVANCED)
    {
      if (CurrentItem->value == 1)
      {
        CurrentItem = &EXIT;
        setting_id = 20;//number of exit_name
        changed = true;
      }
    }
    else if (CurrentItem == &COLOR)
    {
      if (CurrentItem->value == 1)
      {
        CurrentItem = &ADVANCED;
        setting_id = 12;//number of advanced_name
        changed = true;
      }
    }
    else if (CurrentItem == &COLOR_MODE)
    {
      switch (CurrentItem->value)
      {
        case (0):
          CurrentItem = &SATURATION_COLOR;
          setting_id = 9;//number of advanced_name
          changed = true;
          break;
        case (1):
          CurrentItem = &HUE_COLOR;
          setting_id = 8;//number of hue
          changed = true;
          break;
        case (2):
          CurrentItem = &RED_COLOR;
          setting_id = 5;//number of red color
          changed = true;
          break;
        case (3):
          CurrentItem = &TEMPERATURE;
          setting_id = 11;//number of TEMPERATURE
          changed = true;
          break;
      }
    }
    else if ((CurrentItem == &BLUE_COLOR) || ((CurrentItem == &SATURATION_COLOR) && (COLOR_MODE.value != 0)))
    {
      CurrentItem = &TEMPERATURE;
      setting_id = 11;//number of TEMPERATURE
      changed = true;
    }

    if (!changed)
    {
      setting_id = constrain(setting_id + 1, 0, 20);
      CurrentItem = CurrentItem->nextSettingItem;
    }
    LCD_renew();
  }
}

void LCD_renew()
{
  const char brightness_name[] PROGMEM = "Brightness";
  PGM_P brightness[] PROGMEM =
  {
    brightness_name
  };

  const char Speed_name[] PROGMEM = "Speed";
  PGM_P sspeed[] PROGMEM =
  {
    Speed_name
  };

  const char animation_name[] PROGMEM = "Animation";
  const char animation_answer1[] PROGMEM = "Step By Step";
  const char animation_answer2[] PROGMEM = "Smooth";
  const char animation_answer3[] PROGMEM = "Sharp";
  const char animation_answer4[] PROGMEM = "Cascade";
  PGM_P animation[] PROGMEM =
  {
    animation_name,
    animation_answer1,
    animation_answer2,
    animation_answer3,
    animation_answer4
  };

  const char color_name[] PROGMEM = "Color settings?";
  const char color_answer1[] PROGMEM = "Yes";
  const char color_answer2[] PROGMEM = "No";
  PGM_P color[] PROGMEM =
  {
    color_name,
    color_answer1,
    color_answer2
  };

  const char color_mode_name[] PROGMEM = "Color Mode?";
  const char color_mode_answer1[] PROGMEM = "Dynamic color";
  const char color_mode_answer2[] PROGMEM = "Constant HSV";
  const char color_mode_answer3[] PROGMEM = "Constant RGB";
  const char color_mode_answer4[] PROGMEM = "1 use 1 color";
  PGM_P color_mode[] PROGMEM =
  {
    color_mode_name,
    color_mode_answer1,
    color_mode_answer2,
    color_mode_answer3,
    color_mode_answer4
  };

  const char red_color_name[] PROGMEM = "Red";
  PGM_P red_color[] PROGMEM =
  {
    red_color_name
  };

  const char green_color_name[] PROGMEM = "Green";
  PGM_P green_color[] PROGMEM =
  {
    green_color_name
  };

  const char blue_color_name[] PROGMEM = "Blue";
  PGM_P blue_color[] PROGMEM =
  {
    blue_color_name
  };

  const char hue_name[] PROGMEM = "Hue";
  PGM_P hue_color[] PROGMEM =
  {
    hue_name
  };

  const char saturation_name[] PROGMEM = "Saturation";
  PGM_P saturation_color[] PROGMEM =
  {
    saturation_name
  };

  const char color_speed_name[] PROGMEM = "Color Speed";
  PGM_P color_speed[] PROGMEM =
  {
    color_speed_name
  };

  const char temperature_name[] PROGMEM = "Temperature";
  PGM_P temperature[] PROGMEM =
  {
    temperature_name
  };

  const char advanced_name[] PROGMEM = "Advanced?";
  const char advanced_answer1[] PROGMEM = "Yes";
  const char advanced_answer2[] PROGMEM = "No";
  PGM_P advanced[] PROGMEM =
  {
    advanced_name,
    advanced_answer1,
    advanced_answer2
  };

  const char stepnumber_name[] PROGMEM = "Steps count";
  PGM_P stepnumber[] PROGMEM =
  {
    stepnumber_name
  };

  const char delaytime_name[] PROGMEM = "Delay";
  PGM_P delaytime[] PROGMEM =
  {
    delaytime_name
  };

  const char lightsensors_name[] PROGMEM = "Light sensors";
  const char lightsensors_answer1[] PROGMEM = "NO (ALWAYS ON)";
  const char lightsensors_answer2[] PROGMEM = "LOWER";
  const char lightsensors_answer3[] PROGMEM = "UPPER";
  const char lightsensors_answer4[] PROGMEM = "AVERAGE";
  const char lightsensors_answer5[] PROGMEM = "External";
  PGM_P lightsensors[] PROGMEM =
  {
    lightsensors_name,
    lightsensors_answer1,
    lightsensors_answer2,
    lightsensors_answer3,
    lightsensors_answer4,
    lightsensors_answer5,
  };

  const char luminosity_name[] PROGMEM = "Luminosity";
  PGM_P luminosity[] PROGMEM =
  {
    luminosity_name
  };

  const char nightbrightness_name[] PROGMEM = "Standby light";
  PGM_P nightbrightness[] PROGMEM =
  {
    nightbrightness_name
  };

  const char sensorsdistance_name[] PROGMEM = "Sensors";
  const char sensorsdistance_answer1[] PROGMEM = "Upper";
  const char sensorsdistance_answer2[] PROGMEM = "Lower";
  const char sensorsdistance_answer3[] PROGMEM = "Skip";
  PGM_P sensorsdistance[] PROGMEM =
  {
    sensorsdistance_name,
    sensorsdistance_answer1,
    sensorsdistance_answer2,
    sensorsdistance_answer3
  };

  const char adaptive_name[] PROGMEM = "Adaptive";
  const char adaptive_answer1[] PROGMEM = "Yes";
  const char adaptive_answer2[] PROGMEM = "No";
  PGM_P adaptive[] PROGMEM =
  {
    adaptive_name,
    adaptive_answer1,
    adaptive_answer2
  };

  //  const char standbyscreen_name[] PROGMEM = "STAND BY SCREEN";
  //  const char standbyscreen_answer1[] PROGMEM = "On";
  //  const char standbyscreen_answer2[] PROGMEM = "Off";
  //  PGM_P standbyscreen[] PROGMEM =
  //  {
  //    standbyscreen_name,
  //    standbyscreen_answer1,
  //    standbyscreen_answer2
  //  };

  //  const char language_name[] PROGMEM = "LANGUAGE";
  //  const char language_answer1[] PROGMEM = "English";
  //  const char language_answer2[] PROGMEM = "Russian";
  //  PGM_P language[] PROGMEM =
  //  {
  //    language_name,
  //    language_answer1,
  //    language_answer2
  //  };


  const char exit_name[] PROGMEM = "Exit";
  const char exit_answer1[] PROGMEM = "Save&Exit";
  const char exit_answer2[] PROGMEM = "Exit (No Save)";
  const char exit_answer3[] PROGMEM = "Cancel";
  PGM_P Exit[] PROGMEM =
  {
    exit_name,
    exit_answer1,
    exit_answer2,
    exit_answer3
  };

  PGM_P* settings_name[] PROGMEM =
  {
    brightness,
    sspeed,
    animation,
    color,
    color_mode,
    red_color,
    green_color,
    blue_color,
    hue_color,
    saturation_color,
    color_speed,
    temperature,
    advanced,
    stepnumber,
    delaytime,
    lightsensors,
    luminosity,
    nightbrightness,
    sensorsdistance,
    adaptive,
    //    standbyscreen,
    //language,
    Exit
  };

  LCD.clear();
  LCD.home();

  if (mode == WORKING)
  {
    LCD.print("    WORKING");
  }
  else
  {
    LCD.write((uint8_t)0);
    LCD.print(settings_name[setting_id][0]);
    LCD.print(": ");
    if ((CurrentItem == &STEPNUMBER) || (CurrentItem == &COLOR_SPEED) || (CurrentItem == &RED_COLOR) || (CurrentItem == &BLUE_COLOR) || (CurrentItem == &GREEN_COLOR)
        || (CurrentItem == &HUE_COLOR) || (CurrentItem == &SATURATION_COLOR) || (CurrentItem == &TEMPERATURE) || (CurrentItem == &DELAY_TIME) || (CurrentItem == &LUMINOSITY)
        || (CurrentItem == &NIGHT_BRIGHTNESS)  || (CurrentItem == &BRIGHTNESS) || (CurrentItem == &SPEED) || (CurrentItem == &ADAPTIVE) ||  (CurrentItem == &LIGHTSENSORS))
    {
      if ((CurrentItem == &BRIGHTNESS) || (CurrentItem == &SPEED))
      {
        LCD.print(CurrentItem->value);
        LCD.setCursor(0, 1);
        printStars();
        if (CurrentItem == &BRIGHTNESS)
        {
          updateSettings();
          setSteps(255);
        }
        else setSteps(0);
      }
      else if (CurrentItem == &LUMINOSITY)
      {
        LCD.setCursor(0, 1);
        LCD.print("SET: ");
        LCD.print(LUMINOSITY.value);
        LCD.setCursor(8, 1);
        LCD.print("NOW: ");
        LCD.print(light);
        LCD.setCursor(14, 0);
        if (LUMINOSITY.value < light)
          LCD.write((uint8_t)3);
        else
          LCD.write((uint8_t)4);
      }
      else if ((CurrentItem == &RED_COLOR) || (CurrentItem == &BLUE_COLOR) || (CurrentItem == &GREEN_COLOR) || (CurrentItem == &HUE_COLOR) || (CurrentItem == &SATURATION_COLOR) || (CurrentItem == &TEMPERATURE))
      {
        setSteps(255);
        LCD.setCursor(0, 1);
        LCD.print(CurrentItem->value);
      }
      else if (CurrentItem == &STEPNUMBER)
      {
        setSteps(255);
        if (STEPNUMBER.value < 18)
          for (uint8_t i = STEPNUMBER.value; i < 18; i++)
            changeVal(-255, i);
        LCD.setCursor(0, 1);
        LCD.print(CurrentItem->value);
      }
      else if (CurrentItem == &LIGHTSENSORS)
      {
        LCD.setCursor(0, 1);
        LCD.print(settings_name[setting_id][CurrentItem->value + 1]);
        if (LIGHTSENSORS.value != 0)
        {
          LCD.print(" ");
          LCD.print(light);
          LCD.print("/100");
        }
        else if (CurrentItem == &ADAPTIVE)
        {
          updateSettings();
          setSteps(255);
          LCD.setCursor(0, 1);
          LCD.print(settings_name[setting_id][CurrentItem->value + 1]);
        }
      }
      else
      {
        setSteps(0);
        LCD.setCursor(0, 1);
        LCD.print(CurrentItem->value);
        if (CurrentItem == &DELAY_TIME)
          LCD.print(" sec");
        else if (CurrentItem == &STEPNUMBER)
          LCD.print(" steps");
        else if (CurrentItem == &NIGHT_BRIGHTNESS)
          LCD.print("%");
      }
    }
    else
    {
      setSteps(0);
      LCD.setCursor(0, 1);
      LCD.print(settings_name[setting_id][CurrentItem->value + 1]);

    }
  }
}

void printStars()
{
  LCD.setCursor(0, 1);
  if (CurrentItem->max_value == 16)
  {
    for (uint8_t i = 0; i < CurrentItem->value; i++)
    {
      LCD.setCursor(i, 1);
      LCD.print("*");
    }
    for (uint8_t i = CurrentItem->value; i < 16; i++)
      LCD.print("_");
  }
  else
  {
    for (uint8_t i = 0; i < CurrentItem->value / 2; i++)
    {
      LCD.setCursor(i, 1);
      LCD.print("*");
    }
    for (uint8_t i = 0; i < 16; i++)
      LCD.print("_");
  }
}

void setup()
{
  wdt_disable();
  Serial.begin(115200);
  Serial.println("W");
  readSettings();
  Wire.begin();
  pinMode(BUTTON, INPUT_PULLUP);
  LCD.init();
  LCD.backlight();
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  mode = WORKING;
  currentState = STAIRSOFF;
  pinMode(LED, OUTPUT);
  delay( 500 ); // power-up safety delay
  //Инициализация выходов WS2811
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  pinMode(encPinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(encPinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  TCCR1B = TCCR1B & 0b11111000 | 0x01;
  TCCR2B = TCCR1B & 0b11111000 | 0x01;
  LCD.print("     STAIRS");
  LCD.createChar(0, wrench);
  //  LCD.createChar(1, stairs);
  //  LCD.createChar(2, sensor);
  LCD.createChar(3, sun);
  LCD.createChar(4, moon);
  LCD.setCursor(0, 1);
  LCD.setCursor(1, 1);
  LCD.print("  CONTROLLER");
  InitializeSetting();  // Инициализация объектов класса SettingItem(меню)
  wdt_enable (WDTO_2S);
}

void loop()
{
  wdt_reset();

  CHSV hsvColor;
  switch (COLOR_MODE.value)
  {
    case (0):
      hsvColor.hue += COLOR_SPEED.value;
      hsvColor.saturation = 255;
      hsvColor.value = 255;
      hsv2rgb_raw(hsvColor, leds[13]);
      break;
    case (1):
      hsvColor.hue = HUE_COLOR.value;
      hsvColor.saturation = SATURATION_COLOR.value;
      hsvColor.value = 255;
      hsv2rgb_raw(hsvColor, leds[13]);
      break;
    case (2):
      leds[13].r = RED_COLOR.value;
      leds[13].g = GREEN_COLOR.value;
      leds[13].b = BLUE_COLOR.value;
      break;
    case (3):
      hsvColor.hue = randColor;
      hsvColor.saturation = SATURATION_COLOR.value;
      hsvColor.value = 255;
      hsv2rgb_raw(hsvColor, leds[13]);
      break;
  }
  analogWrite(REDPIN, leds[13].r);
  analogWrite(GREENPIN, leds[13].g);
  analogWrite(BLUEPIN, leds[13].b);
  digitalWrite(LED, 1);
  digitalWrite(LED, 0);
  currentTime = millis();
  getSensors();

  if (mode == WORKING)
  {
    updateStairsState();
    stepsWork();
  }
  else if ((CurrentItem == &SPEED) || (CurrentItem == &ANIMATION) || (CurrentItem == &COLOR_SPEED) || (CurrentItem == &COLOR_MODE) ) // включает выбранную анимацию подсветки вверх при настройке
  {
    if (checkON(255))
    {
      currentState = OFFUP;
      
    }
    else if (checkON(0))
    {
    randColor = random(255);
      currentState = LIGHTUP;
    }
    stepsWork();
  }

  if (digitalRead(BUTTON) == LOW && btn_flag == 0)
  {
    btn_flag = 1;
    button();
  }

  if (digitalRead(BUTTON) == HIGH && btn_flag == 1)  //now it's useless condition
  {
    btn_flag = 0; //обнуляем переменную flag
  }
  if (oldEncPos != encoderPos)
  {
    oldEncPos = encoderPos;
    LCD_renew();
  }
  updateLeds();
  FastLED.show();
  FastLED.delay(1000 / 300);
}




