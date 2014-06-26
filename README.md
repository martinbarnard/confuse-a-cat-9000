# The Confuse-A-Cat 9000

I started working on this project after I was very disappointed by a
commercially available "laser cat toy" (as was my cat).  The
commercial toy semi-randomly moved the laser pointer in a circle while
pointed at a mirror.  I knew I could do _much_ better than this,
especially since I had an Arduino Uno lying around and knew where to
get/build everything else I might need to give the laser pointer more
freedom of movement.

## How to build one

### Materials:

  - Arduino
  - Hobby servo x2
  - Momentary push-button x2
  - Potentiometer
  - LED x4
  - Laser emitter
  - Pan-tilt mount

### Hooking it all up

I'm going to assume that you know how to wire the various parts up
appropriately.  I would love for more detailed instructions to go here
(I just followed the examples out of the
[SparkFun Inventor's Kit][sparkfun-inventor-kit]).

Download and install [Fritzing][fritzing]
[fritzing]: http://fritzing.org/download "Fritzing circuit designer"
[sparkfun-inventor-kit]: http://www.sparkfun.com/products/10173 "See the Web or Print quality guide"

#### Digital pins

- 3 - X-axis center LED
- 4 - X-axis range LED
- 5 - Y-axis center LED
- 6 - Y-axis range LED
- 7 - Select which setting to adjust (Momentary push-button)
- 8 - Confirm setting (Momentary push-button)
- 9 - X-axis servo
- 10 - Y-axis servo

#### Analog pins

- 0 - Setting value selection (Potentiometer)

## How to use one

There are a few compile-time, and four run-time options that can be
set.

### Compile-time options

#### Which pins to use

```c++
const int sX_pin = 9;
const int sY_pin = 10;
const int sX_center_led_pin = 3;
const int sX_range_led_pin  = 4;
const int sY_center_led_pin = 5;
const int sY_range_led_pin  = 6;
const int settingButtonPin  = 7;
const int setButtonPin      = 8;
const int potPin            = 0;
```

`sX_pin` is which digital pin the X-axis servo is connected to.

`sY_pin` is which digital pin the Y-axis servo is connected to.

`sX_center_led_pin` is the digital pin for the LED to indicate that
the center for the X-axis will be adjusted.

`sX_range_led_pin` is the digital pin for the LED to indicate that the
range of movement for the X-axis will be adjusted.

`sY_center_led_pin` is the digital pin for the LED to indicate that
the center for the Y-axis will be adjusted.

`sY_range_led_pin` is the digital pin for the LED to indicate that the
range of movement for the Y-axis will be adjusted.

`settingButtonPin` is the digital pin for the momentary-button that
will cycle through the available settings.

`setButtonPin` is the digital pin for the momentary-button that will
set the value for the currently selected setting.

`potPin` is the analog pin for the potentiometer to select the value
for the settings.

#### Sleeping

```c++
int sleep_amt      = 250;
int min_sleep      = 100;
int max_sleep      = 1000;
int max_sleep_step = 500;
```

`sleep_amt` is the inital number of milliseconds to sleep between
moving the servos.

`min_sleep` is the minimum number of milliseconds to sleep between
moving the servos.

`max_sleep` is the maximum number of milliseconds to sleep between
moving the servos.

`max_sleep_step` is the maximum amount to add to or subtract from the
`sleep_amt` when adjusting it for determining how long to sleep before
moving the servos again.

#### Servo movement

```c++
int sX_center = 110;
int sX_range  = 30;
int sY_center = 45;
int sY_range  = 45;
int max_servo_step = 20;
int min_servo_step = 0;
```

`sX_center` determines the initial degree setting the X-axis's
movements will be centered around.

`sX_range` determines the initial degrees of motion the X-axis is
allowed (centered around `sX_center`).

`sY_center` determines the initial degree setting the Y-axis's
movements will be centered around.

`sY_range` determines the initial degrees of motion the Y-axis is
allowed (centered around `sY_center`).

`max_servo_step` is the maximum number of degrees the servos are
allowed to move from their previous position per update.

`min_servo_step` is the minimum number of degrees the servos are
allowed to move from their previous position per update.

### Run-time options

The run-time options are set using the two momentary push-buttons, and
the potentiometer.  By pressing the button hooked up to
`settingButtonPin` the `setButtonPin` momentary push-button and
potentiometer will cycle adjusting the following in order: Nothing
(default; no LED), X-axis centering (`sX_center_led_pin` LED), X-axis
range of motion (`sX_range_led_pin` LED), Y-axis centering
(`sY_center_led_pin` LED), Y-axis range of motion (`sY_range_led_pin`
LED).

Once the desired setting has been chosen using `settingButtonPin`'s
momentary push-button, the desired value can be selected using the
potentiometer (attached to analog pin `potPin`).  The new setting is
then confirmed by pressing `setButtonPin`'s momentary push-button.
The new value is only used once `setButtonPin` is pressed, to prevent
accidentally changing settings when cycling through the available
options.
