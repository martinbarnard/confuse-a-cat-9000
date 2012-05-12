/***********************************************************************/
/* Copyright (c) 2012 Jacob Helwig <jacob@technosorcery.net>           */
/*                                                                     */
/* Permission is hereby granted, free of charge, to any person         */
/* obtaining a copy of this software and associated documentation      */
/* files (the "Software"), to deal in the Software without             */
/* restriction, including without limitation the rights to use, copy,  */
/* modify, merge, publish, distribute, sublicense, and/or sell copies  */
/* of the Software, and to permit persons to whom the Software is      */
/* furnished to do so, subject to the following conditions:            */
/*                                                                     */
/* The above copyright notice and this permission notice shall be      */
/* included in all copies or substantial portions of the Software.     */
/*                                                                     */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,     */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF  */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND               */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN  */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN   */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    */
/* SOFTWARE.                                                           */
/***********************************************************************/

#include <Servo.h>

long now;

Servo sX;
Servo sY;

const int sX_pin = 9;
const int sY_pin = 10;

int x_pos = 90;
int y_pos = 90;

long sleep_time    = 0;
int sleep_amt      = 250;
int min_sleep      = 100;
int max_sleep      = 1000;
int max_sleep_step = 500;
int sleep_step;
int new_sleep;

// Tuning the servo stuff
const int sX_center_led_pin = 3;
const int sX_range_led_pin  = 4;
const int sY_center_led_pin = 5;
const int sY_range_led_pin  = 6;
const int settingButtonPin  = 7;
const int setButtonPin      = 8;
const int potPin            = 0;
// Variables will change:
int settingButtonState;             // the current reading from the input pin
int lastSettingButtonState = LOW;   // the previous reading from the input pin
int setButtonState;             // the current reading from the input pin
int lastSetButtonState = LOW;   // the previous reading from the input pin
int potValue;
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastSettingDebounceTime = 0;  // the last time the output pin was toggled
long lastSetDebounceTime = 0;      // the last time the output pin was toggled
long debounceDelay = 50;           // the debounce time; increase if the output flickers
// Track which parameter we're updating
#define UPDATE_NONE      0
#define UPDATE_SX_CENTER 1
#define UPDATE_SX_RANGE  2
#define UPDATE_SY_CENTER 3
#define UPDATE_SY_RANGE  4
int current_setting = UPDATE_NONE;
int sX_center = 110;
int sX_range  = 30;
int sY_center = 45;
int sY_range  = 45;
int max_servo_step = 20;
int min_servo_step = 0;

void setup() {
  Serial.begin(9600);

  sX.attach(sX_pin);
  sY.attach(sY_pin);

  pinMode(sX_center_led_pin, OUTPUT);
  pinMode(sX_range_led_pin,  OUTPUT);
  pinMode(sY_center_led_pin, OUTPUT);
  pinMode(sY_range_led_pin,  OUTPUT);
  pinMode(13, OUTPUT);
}

void update_current_setting() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(settingButtonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastSettingButtonState) {
    // reset the debouncing timer
    lastSettingDebounceTime = now;
    if (reading == LOW) {
      current_setting++;
      if (current_setting > UPDATE_SY_RANGE)
        current_setting = UPDATE_NONE;
      update_led();
    }
  }

  if ((now - lastSettingDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    settingButtonState = reading;
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastSettingButtonState = reading;
}

void update_led() {
  digitalWrite(sX_center_led_pin, LOW);
  digitalWrite(sX_range_led_pin,  LOW);
  digitalWrite(sY_center_led_pin, LOW);
  digitalWrite(sY_range_led_pin,  LOW);

  switch (current_setting) {
  case UPDATE_SX_CENTER: digitalWrite(sX_center_led_pin, HIGH); break;
  case UPDATE_SX_RANGE:  digitalWrite(sX_range_led_pin,  HIGH); break;
  case UPDATE_SY_CENTER: digitalWrite(sY_center_led_pin, HIGH); break;
  case UPDATE_SY_RANGE:  digitalWrite(sY_range_led_pin,  HIGH); break;
  }
}

void set_setting(int degrees) {
  if (current_setting == UPDATE_NONE)
    return;

  Serial.print("Set ");
  switch (current_setting) {
  case UPDATE_SX_CENTER:
    sX_center = degrees;
    Serial.print("sX_center to ");
    Serial.print(sX_center);
    break;
  case UPDATE_SX_RANGE:
    sX_range = degrees;
    Serial.print("sX_range to ");
    Serial.print(sX_range);
    break;
  case UPDATE_SY_CENTER:
    sY_center = degrees;
    Serial.print("sY_center to ");
    Serial.print(sY_center);
    break;
  case UPDATE_SY_RANGE:
    sY_range = degrees;
    Serial.print("sY_range to ");
    Serial.print(sY_range);
    break;
  }
  Serial.println(" degrees");
}

void update_setting() {
  int pot_reading = analogRead(potPin);
  // read the state of the switch into a local variable:
  int reading = digitalRead(setButtonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastSetButtonState) {
    // reset the debouncing timer
    lastSetDebounceTime = now;
    if (reading == LOW) {
      int mapped_reading = constrain(map(pot_reading, 0, 1024, 0, 180), 0, 180);
      set_setting(mapped_reading);
    }
  }

  if ((now - lastSetDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    setButtonState = reading;
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastSetButtonState = reading;
}

void loop() {
  now = millis();
  if (now > sleep_time) {
    sleep_step = rand() % (max_sleep_step * 2);
    if (sleep_step > max_sleep_step)
      new_sleep = sleep_amt + (sleep_step - max_sleep_step);
    else
      new_sleep = sleep_amt - (max_sleep_step - sleep_step);

    sleep_amt = constrain(new_sleep, min_sleep, max_sleep);
    sleep_time = now + sleep_amt;

    //constrain(rand() % sX_range + (sX_center - sX_range/2), 0, 180);
    //constrain(rand() % sY_range + (sY_center - sY_range/2), 0, 180);
    int x_delta = (rand() % ((max_servo_step + 1 - min_servo_step) * 2) + min_servo_step) - (max_servo_step + 1);
    int y_delta = (rand() % ((max_servo_step + 1 - min_servo_step) * 2) + min_servo_step) - (max_servo_step + 1);

    int x_min = constrain((sX_center - (sX_range/2)), 0, 180);
    int x_max = constrain((sX_center + (sX_range/2)), 0, 180);
    int y_min = constrain((sY_center - (sY_range/2)), 0, 180);
    int y_max = constrain((sY_center + (sY_range/2)), 0, 180);

    x_pos = constrain((x_pos + x_delta), x_min, x_max);
    y_pos = constrain((y_pos + y_delta), y_min, y_max);

    Serial.print(" x_delta ");
    Serial.print(x_delta);
    Serial.print(" y_delta ");
    Serial.print(y_delta);
    Serial.print(" x_min ");
    Serial.print(x_min);
    Serial.print(" x_max ");
    Serial.print(x_max);
    Serial.print(" y_min ");
    Serial.print(y_min);
    Serial.print(" y_max ");
    Serial.print(y_max);

    Serial.print(" x_pos ");
    Serial.print(x_pos);
    Serial.print(" y_pos ");
    Serial.print(y_pos);

    sX.write(x_pos);
    sY.write(y_pos);

    Serial.println();
  }

  update_current_setting();
  update_setting();
}
