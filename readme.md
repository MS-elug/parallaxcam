# Prerequisites

* Install [Visual Studio Code](https://code.visualstudio.com/)
* Install cpptools plugin for Visual Studio code
* Install [Visual C++ 2015 Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)
    -> select personalized installation and make sure to install  Windows SDK - Windows 10 10.0.15063.137
* Install PCWH 4.104

Library version used: 
* [LibUsb 1.0.21](http://libusb.info/) (Open source)
* [OpenCV 2.4.13.3](http://opencv.org/releases.html) (Open source)

# Project description

This project contains 3 folders:
* app: contains source code for the windows parallax camera viewer
* pic: contains source code project (PCWH 4.104) for the PIC 18F4550 board mounted with 2 Parallax camera.
* WinUSBDriver: contains windows installation (unsigned) driver to register the usb device (PIC 18F4550 board connected by USB) using the generic WinUSB windows driver.

## Folder app
### How to build the application

Open folder app with vscode.

From the integrated command line, execute ```build.bat```

Once built, execute ```.\bin\parallax.exe```

Be careful to plug the device prior to launch the app.

## Folder pic

Open main.pjt with PCWH.

Do build, and write the .hex file on the PIC 18F4550.

## Folder WinUSBDriver
### Device Installation guide

Connect the board by USB on your computer.

In the control panel system device manager, right click on the device 'PARALLAX CAM' and select to install manually the driver from the WinUSBDriver folder.

## Generic documentation

http://www.rennes.supelec.fr/ren/fi/elec/docs/usb/usb.html

http://www.abcelectronique.com/acquier/usb5_fr.htm
