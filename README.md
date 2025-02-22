# Attiny85-4-leds-blink-with-push-button

This code is an evolution of my previous code. I have added a button that allows changing the LED blinking program.

With each press of the button, we add +1 to the value 's0'. It should be noted that pressing the button generates 2 interrupts: one on the falling edge and another on the rising edge.

In the 'loop' function, we use the 'case' instruction to read the value of 's0' and apply different programs with different LED sequences and sleep times.
