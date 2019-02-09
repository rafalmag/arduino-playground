# Wavegen

## About

I created a PCB design - to be at home - soldered on 8x12 cm protoboard (without etching or ordering a PCB).

The schematic based on original work by [rgco](https://www.instructables.com/id/Arduino-Waveform-Generator-1). 

My design principles (modifications to the original design):
- smd resistors (but there is some space left, could be replaced with tht in stand-up position)
- no wire bridges
- 90 degree track curves should be avoided, but here during soldering I try to smooth them out
- potentiometers available for LCD brightness and contrast adjustments
- screw terminals
- LCD secured to PCB
- PCB with legs
- LED attached to buffered output to visualize the output (works best for low frequencies)
- powered from USB 5V

## Future improvement ideas:
- DC-DC boost converter output voltage potentiometer could be desoldered and positioned on the main PCB between Arduino and the DC-DC board (as the ergonomic of adjustment one in horizontal position is not ideal)
- hardware de-bouncer for encoder (in my case issues are quite rare, but it depends on luck - from the same batch of encoders some are less, some are more noisy)
- amplitude potentiometer could be replaced with multi turn potentiometer (ideally hand-operated without a screwdriver)
- I actually used 5% tolerance resistors, 1% or better would be ideal. However I did not notice any shape distortions on the oscillograph. 

## License

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.

## Credits
Idea, code and initial schematic by rgco:
https://www.instructables.com/id/Arduino-Waveform-Generator-1
