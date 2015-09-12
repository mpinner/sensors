
# Installation




## Processing

OSC from Ableton (or anywhere) can be sent to our USB Gem Controller via a small application. This application provides a GUI for configuring PC USB Port and the Osc Connection Parameters. I've preconfigured our standard profile, so these settings shouldn't have to be messed with unless there are new configurations or devices introduced.

###Components

 * **FORKER** : a performance optimization to rate limit  excessive ableton osc messaging
 * **GemSender** : route osc to SLIP serial usb Teensy
 * **GemSenderManual** : a test sketch to use a midi device with ableton
 
###Dependencies

* Java : https://www.java.com/en/download/ - this will require an internet connection* Serial Drivers : https://www.pjrc.com/teensy/serial_install.exe* GemSender :  * unzip and find the application for your operating system  * it would be best to right click the application and create a shortcut to place where you choose. - when launched this application will start with our preconfigured settings  - when incoming data is received the UDP box will flash white.

* Firewall Rules (windows only) :  - if the white box is not flashing and you've double checked your connection parameters, you might need to alter your firewall.  - Within the Control Panel -> Firewall -> Disable Firewall for Local/Private Networks  - Explanation : The GemSender is attempting to open a local port to listen for incoming OSC. the default windows firewall rules will prevent any application from acting in a server this way.

 
## Arduino


 
  * **Gems_HitSync** : latest Usb Gem Controller code to controll all 8 gems
  * **lightingTEsts** : used to test gem leds, power, and connections
  * **PurityRingSystemDataFlows.jpg** : new multi-machine diagram
  
###Dependencies

* latest Arduino
* Teensduino
* Teensy 3.1
* OctoWs2811Adapter and OctoWS2811.h
* Open Sound Control : OSCBundle /SLIPEncodedUSBSerial.h
* LucidTronix HSB RGB Color Library : Color.h

  
## TouchOsc
 * layouts for simulating osc gem hits and control messages
 
###Dependencies

http://hexler.net/software/touchosc

  * android or ios device running TouchOSC app
  * their desktop companion app for pushing layouts



## LightBeamer 
 * the dark star, another drum triggered instrument
 
###Dependencies

 * see BOM

 
## Contribute
 * be kind
 * code review
 * build shaders
 * test
 * build new osc -> socks serial usb app
 * document
 
 
 
 
 