# Enzyme-labeled-instrument ( 2006-2007)

An Enzyme-labeled instrument, developed with c++ running on a Samsung 44B0X (ARM7) MCU.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

This is a Arm ADS project. There is also a virtual project for simulating the instrument, which is a Microsoft Visual studio project (VC6.0).

  * ARM ADS IDE
  * Visual studio


### Installing

step 1. Donwload the code
step 2. Compile the project with ADS
step 3. Donwload it to the board and run it.

\* As it is hardware-specified, so it would not work on any board, so the installing is just a reference.

## Built With

* ARM ADS
* Visual Studio 6.0 for the simulator

## System architecture
This is a event-drive system proving near real-time functionality. it is a bit close to microsoft window OS with UI, and file system (similar to FAT). The structure is like,

![](https://github.com/Borrk/Enzyme-labeled-instrument/raw/master/doc/System-Architecture.png)

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Brook Huang**

See also the list of [contributors](https://github.com/Borrk/Enzyme-labeled-instrument.git/contributors) who participated in this project.

## License
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**

