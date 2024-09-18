# Altimeter / Elevation Display for Car

This will display your current elevation/altitude based off of a GPS fix. It also dims the display after sunset. 

For elevations above 9,999 feet, the display reads out using a shortened syntax. For example 10,500 feet will read out as "10.5k".

![installed_altimeter](https://github.com/user-attachments/assets/f73971af-cc9b-4cc4-bcb3-9e36a53d3b15)

## Requirements

### Parts:
* [Adafruit Arduino Metro Mini w/ STEMMA QT](https://www.adafruit.com/product/2590)
* [Adafruit Mini GPS w/ STEMMA QT](https://www.adafruit.com/product/4415)
* [Adafruit 7 Segment Display w/ STEMMA QT](https://www.adafruit.com/product/878)
* [1x short (50mm) STEMMA QT cable](https://www.adafruit.com/product/4399)
* [1x long (400mm) STEMMA QT cable](https://www.adafruit.com/product/5385)
* [12v to USB univeral power adapter tap](https://www.amazon.com/dp/B0D4LGW3JM) (for powering the Arduino off of my rear-view mirror wiring harness)

### Tools:
* Soldering iron (for final assembly of the 7-segment display)
* Multimeter (for finding positive and negative wires in the rear-view mirror wiring harness)
    * Note: this may not be needed if you can find a rear-view mirror adapter specific to your car rather than using the universal adapter I listed above.

### Software:
* [Arduino IDE](https://docs.arduino.cc/software/ide/#ide-v2) (for compiling and uploading the code to the Arduino)

## Instructions

### Hardware assembly
The 7-segment display I used required some simple soldering to get the final assembled part. Instructions can be found on the Adafruit website.

Thanks to the STEMMA QT ports, connecting the components is a breeze! I simply wired it like:

```
Arduino <--short cable--> GPS <------long cable------> Display
```

Having a long cable go to the display made it so I could stuff the rest of the components out of sight while keeping flexibility in where I placed the display.

### Software
* Install Arduino IDE and the appropriate libraries
* Upload the `.ino` sketch included in this repository to the Arduino using the Arduino IDE & a USB cable

**NOTE:** I needed to make a minor modification to the `Sunrise` v2.0.4 library to get compiler to work (I added `#define rint(x) (double)lrint(x)` as suggested [here](https://github.com/signetica/SunRise/blob/93c2db79c580f0758d3f497aed1d599d7d815036/SunRise.cpp#L263)). 
This required editing the library file locally. 

### 3D Printing the Case
<.stl file to come>

#### Printer settings
- Printer make: Prusa
- Printer: i3 MK2S
- Rafts: No
- Supports: No
- Resolution: 0.2mm
- Infill: 20%

### Car installation
The Arduino runs off of USB power, which I got via a 12V-to-USB adapter tapped into the 12V source that powers my rear-view mirror. 
These adapters are usually used to power dashcams. 
You may be able to find a rear-view mirror adapter specific to your car which would allow you to skip the multimeter work.

If you do not have a powered rear-view mirror, you will have to power a USB adapter another way.

#### Access the rear-view mirror power harness 
You may need to remove some trim to do this.

#### Identify the positive and negative wires using a multimeter
First make sure your car is in a state that is actually providing power to the rear-view mirror (e.g. ignition turned on). Then, with your multimeter in voltage mode, probe the outputs until you see a signal around 12V. Protip: if you see black and red wires start by testing those. 

You may need to insert some wires into harness to be able to probe the outputs like I did below:

![testing_leads](https://github.com/user-attachments/assets/1bfcbf1b-6476-44bb-94f8-dab262eb5149)


#### Tap the 12V power
The universal adapter I used requires you to insert its leads *into the back* of the wiring harness like so:

![installed_tap](https://github.com/user-attachments/assets/8e88bfdc-2a13-4f1e-8def-8ad9cea59d59)

#### Install the hardware
On my car there was a small compartment above the rear-view mirror where I could stuff the Arduino and GPS components without having to take apart any trim. I simply put all the components in there and fed a single wire out to the display which was mounted via velcro.

That's it!

## Acknowledgements
The Arduino code and car installation instructions were adapted from Kris Linquist's [Arduino-Elevation-Display](https://github.com/klinquist/Arduino-Elevation-Display) repo.

With the help of Devin J., the 3D model for the 7-segment display case was adapted from [Pyrce007's Adafruit Segmented Display Case](https://www.thingiverse.com/thing:2669204). We added the STEMMA QT ports and adjusted the dimensions of the face opening. 

## Other Learning Resources
I highly recommend checking out the guides in the [Adafruit Learning System](https://learn.adafruit.com/) if you want to learn more about any of the parts or their Arduino libraries. 

Each Arduino library also comes with loads of useful examples you can find in the Arduino IDE via: `File > Examples`

## Licenses
The Arduino code license is TBD.

The display case .stl file is licensed under the [Creative Commons - Attribution - Share Alike](https://creativecommons.org/licenses/by-sa/3.0/) license to be compatible with the license on the [original source](https://www.thingiverse.com/thing:2669204).
