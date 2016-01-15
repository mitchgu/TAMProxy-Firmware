TAMProxy Firmware
=================

Purpose
-------

TAMProxy (Totally A Microcontroller Proxy) is a microcontroller project offloads configuration, sampling, or setting of microcontroller devices/peripherals to a host computer through exchange USB packets.

For example, running the classic Arduino blink sketch using the python host software is as simple as:

	class Blink(Sketch):
	
	    def setup(self):
	        self.led = DigitalOutput(self.tamp, 13)
	        self.led_timer = Timer()
	        self.led_state = False
	
	    def loop(self):
	        if self.led_timer.millis() > 1000:
	            self.led_timer.reset()
	            self.led_state = not self.led_state
	            self.led.write(self.led_state)
	
    sketch = Blink()
    sketch.run()
    
With most similar libraries that control Arduino pins over USB, the communication is of a stop and wait nature. When the user sends a request, their program blocks while waiting for a response from the Arduino. With USB serial latency often in the millisecond range, this puts a big bottleneck on the user's code.

TAMProxy does things differently by having a formalized variable-length packet structure and implementing a sliding window protocol on the host side to send them. TAMProxy can release several packets and simultaneously listen for the responses of packets sent earlier, which significantly increases throughput. All the communcations code runs in another process, so the user's sketch never blocks. In preliminary testing, throughput reaches around 17,000 packets per second at maximum when running PyPy 4.0.1 on a 2013 MBP.

TAMProxy was designed for use in MIT's MASLAB 2016 competition (autonomous robotics). It currently only supports the Teensy 3.x boards, but support for Arduinos is probably possible if development continues.

### Limitations
Right now this firmware only works with the Teensy 3.x, with the support of the Teensyduino libraries available from PJRC

Communicating with certain peripherals that need high-speed or advanced features such as interrupts, SPI, or I2C will be difficult to use if support for that peripheral isn't built into TAMProxy yet.

Adding support for these peripherals is best done by extending this firmware by adding a new peripheral class, as described below. Despite the relatively high throughput of the USB packet protocol, trying to handle these high speed features oh the host side is probably infeasible.

Supported Devices
-----------------
- [x] Digital input
- [x] Digital output
- [x] Analog input
- [x] Analog output (PWM, or DAC on A14/40)
- [x] Quadrature encoder
- [x] Standard Motor (Cytron/Dago with PWM & dir)
- [ ] Feedback motor (Encoder feedback with PID for settable speed)
- [x] Servo Motor
- [ ] Stepper Motor
- [x] Analog Devices Gyroscope (SPI)
- [x] Short-range IR Distance Sensor (just an analog input)
- [x] Ultra-short range IR Distance Sensor (just a digital input)
- [x] Color Sensor (I2C)

Dependencies
------------

- GNU Make (Probably already have if you're running OSX/Linux)
- [Arduino IDE 1.6.6](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous) - needed to install Teensyduino and probably good to have anyway
- [Teensyduino 1.2.6](https://www.pjrc.com/teensy/td_download.html) - Contains the necessary ARM C compilers, Arduino-compatible libraries, and programmers for the Teensy

### Windows Support
Unfortunately, getting the Makefile to work with an existing Teensyduino install is difficult on Windows, so the Makefile currently only works with OSX or Linux.
However, an easy alternative is using the Arduino IDE to compile and upload. Simply open `src.ino` in the Arduino IDE and compile and upload.

Quick Start
-----------

- Ensure that you have the dependencies. If you're on Linux you may have to setup a [udev rule](https://www.pjrc.com/teensy/loader_linux.html) for the Teensy.
- Open the Makefile and confirm that the Arduino application path is correct for your system.
- Run `make` to build the firmware and output an Intel `.hex` file.
- Plug your Teensy in.
- Run `make upload` to open the Teensy Loader programmer with the `.hex` file and program the Teensy. You may have to press the programming button on the Teensy to enter programming mode.

**Tip**: Once you've run `make upload` once, if you keep the Teensy Loader open you can skip the `make upload` step and just press the Teensy's programming button to reprogram it with a newer `.hex` file.

**Further tip**: If you're running Sublime Text or another editor with build settings, you can have Sublime open the entire folder and set the build system to `Makefile`, then compile the project with the build shortcut (command B or ctrl B).

Extending the Firmware
----------------------
The most natural way to extend the firmware is to write a new device class that isn't included yet. To integrate it into the device manager, the class must extend the purely virtual (abstract) class `Device` and implement the `handleRequest` function. It should probably also have a custom constructor that takes initialization settings (pins, etc). Finally, the device needs to be given a device code and added to the `DeviceList` class's `add` function.

Make Targets
------------

- `make` aliases for `make hex`
- `make build` compiles everything and produces a .elf
- `make hex` converts the elf to an intel hex file
- `make post_compile` opens the launcher with the correct file
- `make upload` uploads the hex file to a teensy board
- `make reboot` reboots the teensy

Credits
-------

- The `Makefile` is derived from the wonderful makefile at [apmorton/teensy-template](https://github.com/apmorton/teensy-template) as well as the makefile in Teensyduino: [PaulStoffregen/cores](PaulStoffregen/cores)
- The firmware design is based on the Maple firmware used in MASLAB 2014: [jwbowler/maslab-2014](https://github.com/jwbowler/maslab-2014)
