**Cloud-Based Autonomous Robot Vehicle**

Gyan Prayaga
10 May 2017

# Project Overview

This academic study project comprised the design and development of a small robotic vehicle complemented by an onboard microcontroller and ultrasonic sensor, which was connected via a Bluetooth smartphone to the navigational cloud. As such, the project entailed three distinct stages: (1) construction of the vehicle, (2) programming of the vehicle&#39;s microcontroller, (2) programming of the Android middleware app, and (3) programming of the navigational cloud. The vehicle&#39;s form factor was that of a kit-assembled DFRobot Devastator robot tank with suspension. A Romeo BLE (Arduino derivative with built-in Bluetooth) was placed on top of the tank to control motor movement. In addition, an ultrasonic sensor was connected to the BLE for basic obstacle avoidance. The BLE was programmed using Arduino C code, and a navigational logic system was developed for obstacle avoidance. The Android app was programmed using the cross-platform language Angular JS and deployed through Ionic framework. Finally, the navigational cloud was deployed on a DigitalOcean server, utilizing an HTTP GET interface programmed in PHP. During testing, the robot tank successfully drove forward, performed a left turn, and then continued forward. In addition, the Android app was able to parse navigational steps from the cloud and provide motor directives on-screen in real time using its onboard GPS (which can be upgraded to create a Bluetooth bridge between the smartphone and Arduino microcontroller). This project represents an innovative synthesis of onboard electronics, autonomous software, connectivity systems, and web technologies.

# Enclosed Code

This folder contains _three subfolders_ with code from the three components of the project.

## Arduino
The Arduino app requires no dependencies to run. It is composed of a single _robot.ino_ file (called a Sketch) which runs on the Arduion microcontroller. The microcontroller used in this project was a Bluetooth-enabled Arduino derivative called a Romeo BLE, produced by the Chinese robotics company DFRobot.
- robot.ino

## Android
The app was programmed in the cross-platform scripting language Angular JS and compiled on the Android smartphone using the Ionic framework (which integrates with Cordova to provide access to the phone&#39;s GPS and Bluetooth functionality). Here we see the breakdown of the Android app. Not all folders will be of interest, so specific folders and files have been expanded and described.

- **node\_modules**: Javascript plugins which enable front-end functionality
- **platforms**: universal Ionic files enabling the app to run on the device
- **plugins**: native Cordova plugins which enable access to Bluetooth and geolocation
- **scss**: styling of the app
- **server**: please disregard; was not used in application
- **www**
  - **css**: app-specific styling
  - **img**: image resources
  - **js**: JavaScript provides logic and functionality for the app
    - **controllers** : JavaScript files for the app&#39;s functionality
      - **DeviceController.js** : performs pings to the navigational cloud and returns text instructions and creates a motor directive
      - **HomeController.js** : functionality for the device search screen
    - **App.js** : the main runtime; this loads the initial controller
  - **Lib**: various libraries used as dependencies for the app
  - **Templates**: HTML templates used for rendering the app
    - **Device.html** : screen shown when smartphone connects to device over Bluetooth
    - **Home.html** : the initial app screen with Bluetooth search
  - **Index.html**: the home page before controllers are instantiated
- **package.json**: shows which packages were installed/enabled

## Cloud
The cloud navigation app runs on an Apache Ubuntu server. The app itself is a simple HTTP API interface programmed with the web scripting language PHP.

- **index.php**
