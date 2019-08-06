# Enzyme-labeled-instrument ( 2006-2007)

An Enzyme-labeled instrument, developed with c++ running on a C8051 MCU.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

This is a Silicon IDE project, Silicon Labs or Keil uVision is recommended to develop and debug the code. There is also a virtual project for simulating the instrument, which is a Microsoft Visual studio project (VC6.0).

  * Silicon Labs Integrated Development Environment (IDE) or Keil uVision
  * Visual studio
  * And a USB Debug Adaptor ![](https://www.silabs.com/content/usergenerated/asi/cloud/attachments/siliconlabs/en/community/mcu/8-bit/knowledge-base/jcr%3acontent/content/primary/blog/using_the_usb_debug-XeTq/images/8832b5dc-56ac-4f0e-a1d6-d2facba7f010.png)

### Installing

step 1. Donwload the code
step 2. Compile the project with Silicon Labs/uVision
step 3. Donwload it to the board and run it.

\* As it is hardware-specified, so it would not work on any board, so the installing is just a reference.

## Built With

* Silocon Labs or Keil uVision
* Visual Studio 6.0 for the simulator

## System architecture
This is a event-drive system proving near real-time functionality. it is a bit close to microsoft window OS with UI, and file system (similar to FAT). The structure is like,

![](https://github.com/Borrk/Enzyme-labeled-instrument/raw/master/doc/System-Architecture.png)

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Brook Huang** - *All the work*

See also the list of [contributors](https://github.com/Borrk/Enzyme-labeled-instrument.git/contributors) who participated in this project.

## License
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**

