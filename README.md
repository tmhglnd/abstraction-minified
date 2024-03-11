# Abstraction Minified

A minified version of the [`.abstraction()`](https://github.com/tmhglnd/abstraction) installation that can be run on a RPi2+ with a small LCD screen and controlled with a self-build wireless controller using for example an ESP32.

## Support my work

[![](https://img.shields.io/static/v1?label=Support%20on%20Ko-Fi&message=%E2%9D%A4&logo=Kofi)](https://ko-fi.com/I2I3SV7FX)
[![](https://img.shields.io/static/v1?label=Support%20on%20Patreon&message=%E2%9D%A4&logo=Patreon)](https://www.patreon.com/bePatron?u=9649817)
[![](https://img.shields.io/static/v1?label=Support%20on%20Gumroad&message=%E2%9D%A4&logo=Gumroad)](https://tmhglnd.gumroad.com/)

<!-- ![](./images/img1.JPG) -->

# Install

This installation runs in the browser via a localhost NodeJS server. First install node modules:

`npm install`

Then start the server:

`npm start`

Then navigate in the browser to:

`http://localhost:3000`

# Make this into an installation running on a Raspberry Pi

It is possible to run this installation on a Raspberry Pi 4+ (3 or 2 are also possible but on a low visual resolution, like 160x90 pixels or less, this however is aesthetically quite pleasing!).

### Table of Content

- [Install NodeJS](#install-nodejs-and-node-version-manager-on-rpi)
- [Run Process on Boot](#run-process-on-boot-on-rpi)
- [Run Browser fullscreen](#run-browser-in-fullscreen-on-rpi)
- [Remove screensaver and mouse](#remove-screensaver-and-mouse)
- [Extend with RPi screen](#extend-with-an-rpi-screen)
- [Built hardware controller](#built-the-hardware-controller)
- [Uploading code to ESP32](#uploading-code-to-the-esp32)
- [Making RPi into WiFi Accesspoint](#setting-up-rpi-as-wifi-access-point)
- [Acknowledgements](#acknowledgements)
- [License](#license)

## Materials

- Raspberry Pi 2+ 
	- screen, keyboard and mouse while setting up
	- Wi-Pi wifi dongle for RPi2
	- TFT LCD Screen RPi compatible (eg. 3.5" waveshare)
- Wemos Lolin32 with soldered header pins
	- 1 Micro-usb cable for data transfer
	- 1 LiPo battery 3.7V 500mAh (optional)
- Breadboard and jumperwires
	- 2 Potentiometers
	- Protoboard and wires (optional for soldering everything)

## Install NodeJS and Node Version Manager on RPi

First install NodeJS `node` via the terminal.

For nvm (Node Version Manager) use the following installing script:

`curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash`

And run the following scripts:

`export NVM_DIR="$HOME/.nvm"`

Run this to load nvm:

`[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"`

Check if nvm is installed with:

`command -v nvm` (*This should return: `nvm`*)

Now install the latest stable NodeJS version with:

`nvm install stable` or `nvm install vX.Y.Z` (eg. `v18.11.0`)

Now check if correctly installed with:

`node -v` (*This should return the version number, eg: `v18.11.0`*)

You can switch versions with:

`nvm use X` (eg. `nvm use 18`)

Now download/clone this repository and navigate inside the folder with `cd`.

Now follow the steps from [Install](#install). 

Optionally run `node server.js --debug` to see logs of received osc-messages to check connection with controller.

## Run process on boot on rPi

To make sure the installation starts up directly with the boot of the RPi we install *Process Manager 2* with `npm`:

`npm install pm2 -g` (*this is a global installation, sometimes `sudo npm install` is needed depending on admin rights*)

Navigate to the `abstraction-minified` project folder and start project:

`pm2 start server.js --name my-process-name` (choose your own process name)

Create the startup files:

`pm2 startup` (`pm2 unstartup` to remove)

Copy/paste/execute the command printed in the terminal and afterwards save the startup process

`pm2 save`

Some useful commands to monitor processes while the installation runs:

`pm2 list`, `pm2 monit`, `pm2 logs [process name]`

## Run browser in fullscreen on RPi

If you like to show the browser in fullscreen you can use `F11` or you can start the browser in fullscreen mode via the terminal with:

`chromium-browser --start-fullscreen http://localhost:3000` (*this is already included in the server.js startup script*)

## Remove screensaver and mouse

To avoid letting the screen go to black when not using the rPi install xscreensaver with:

`sudo apt-get install xscreensaver`

Open the application and disable the screensaver. Maybe a reboot of the RPi is necessary before the application runs correctly.

To hide the mouse when not moving the curser install unclutter:

`sudo apt-get install unclutter`

Then run the following command to hide the cursor after 1 second

`unclutter -idle 1`

## Extend with an RPi Screen

You can combine the installation with a little RPi LCD screen to create a version that can run standalone in the living room as a kind of generative picture frame. For example use the [`3.5 inch RPi LCD Screen`](https://www.waveshare.com/3.5inch-rpi-lcd-c.htm)

`git clone https://github.com/waveshare/LCD-show.git`

Then navigate to the folder

`cd LCD-show`

Modify the file `LCD35-show` to add executable permission and execute

`chmod +x LCD35-show`

`./LCD35-show` (of if you have `C` model for example: `./LCD35C-show`)

Set the correct resolution via the Preferences > Configuration of the RPi. If you do an `apt-get upgrade` on the RPi make sure to add this line to the `config.txt`

`dtoverlay=ads7846`

## Built the hardware controller

The [ESP32 Lolin](https://www.wemos.cc/en/latest/d32/d32.html) is a microcontroller that has many of the same functionalities as an Arduino allowing us to read the voltage from a potmeter (variable resistor) or some other sensor through an `analogRead()`. The extra benefit of this microcontroller is that it can run wireless, accessing a WiFi network or creating its own access point. It also can be powered through a LiPo battery of 500mAh.

Connect the potentiometers of your choice to the board. For example a standard potentiometer of 10kOhm can be connected easily. The potmeter has 3 pins, leftmost pin can go to the ground (GND), rightmost pin goes to the power (3V). The middle pin can be read from a GPIO pin on the ESP32 board.

**Note: Choose a GPIO pin from ADC1**. [See the schematic here](https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmischianti.org%2Fwp-content%2Fuploads%2F2020%2F11%2FESP32-WeMos-LOLIN32-pinout-mischianti-1024x562.png&f=1&nofb=1&ipt=88e6d3051ddbbf0c0ed73a787bd206160372b5ab19359411a17a5456f0aa1b12&ipo=images) Pins with ADC2 are disabled when the ESP32 is connected via WiFi.

## Uploading code to the ESP32

Install the [Arduino IDE](https://www.arduino.cc/en/software) for your operating system.

Start the Arduino IDE and install the *esp32* by *Espressif Systems* via the Boards Manager.

Also install the *OSC* library by *Adrien Freed and Yotam Mann* via the Library Manager.

Connect the board via a micro-usb cable. Note: Make sure the cable allows for data transfer. Open the `controller.ino` arduino sketch from the folder arduino.

Click on *Select Board* and select the USB port the ESP32 is connected to. In the BOARDS menu that pops up select WEMOS LOLIN32.

Upload the sketch to the board by clicking the **->** arrow (or `CMD + U`)

The LED starts to blink rapidly every 50 ms. This means it is searching for a WiFi network to connect to. Follow the instructions below to setup this WiFi Access Point on the RPi. After choosing an `ssid` and `wpa_passphrase` you can adjust this in your arduino code and upload again:

```c
// WiFi network name and password replace with your own:
const char * networkName = "RPiAccessPoint";
const char * networkPswd = "RPiPassWord";
```

If the connection works the LED blinks slowly, every 500 ms.

## Setting up RPi as WiFi Access Point

By setting up the Raspberry Pi as a WiFi Access Point you allow the ESP32 to automatically connect to it and send the potmeter information as osc-messages over the wireless network. This gives the installation a wireless controller that you can play with.

When using an Raspberry Pi 2 make sure you connect a WiFi dongle such as the Wi-Pi wlan module.

The following steps are adapted from the [Adafruit tutorial](https://learn.adafruit.com/setting-up-a-raspberry-pi-as-a-wifi-access-point/install-software).

Boot the RPi and check if it can ping out to the internet with `ping 8.8.8.8`. Use `ctrl + c` to quit. Expected result in the terminal:

```
PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
64 bytes from 8.8.8.8: ...
--- 8.8.8.8 ping statistics ---
x packets transmitted, x received, 0% packet loss, time xxxxms
```

Check if the WiFi hardware is connected properly to allowing connections with `ifconfig -a`. You should see `wlan0` listed.

Update the RPi with `sudo apt update` and then `sudo apt -y upgrade`.

When done run the following commands:

`sudo apt install -y hostapd dnsmasq`

`sudo systemctl unmask hostapd`

`sudo systemctl enable hostapd`

`sudo DEBIAN_FRONTEND=noninteractive apt install -y netfilter-persistent iptables-persistent`

`sudo reboot`

After rebooting go back in the terminal to edit the following files using `nano` or another editor of your choice. 

`sudo nano /etc/dhcpcd.conf`

In `dhcpcd.conf` add the following to the bottom. Note: the 4 spaces for indentation are important!

```
interface wlan0
    static ip_address=192.168.4.1/24
    nohook wpa_supplicant
```

Run `ctrl + x` to save and quit. Move to the next file:

`sudo nano /etc/sysctl.d/routed-ap.conf` and add:

```
# Enable IPv4 routing
net.ipv4.ip_forward=1
```

Save and quit. Go to the next file: `sudo nano /etc/dnsmasq.conf`. Add the following to the top of the file:

```
# Listening interface
interface=wlan0
# Pool of IP addresses served via DHCP
dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h
# Local wireless DNS domain
domain=wlan
# Alias for this router
address=/gw.wlan/192.168.4.1
```

Save and quit. Got to the next file: `sudo nano /etc/hostapd/hostapd.conf`. Add the following:

```
country_code=US
interface=wlan0
ssid=RPiAccessPoint
hw_mode=a
channel=48
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=RPiPassWord
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
# driver=nl80211
```

Adjust the settings based for `country_code`, `ssid`, `hw_mode`, `channel` and `wpa_passphrase`. Find your [country_code here](https://en.wikipedia.org/wiki/ISO_3166-1#Current_codes). For hw_mode, use `a` for 5GHz (RPi3B+), `b` for 2.4GHz and `g` for usb WiFi dongles. The channel depends on the hw_mode and the country_code. For b-g use `1-11` as a channel, for a the range is larger. The `driver=nl80211` can be uncommented for usage of WiFi dongles with an 802.11n chip.

Finally run the following lines:

`sudo raspi-config nonint do_wifi_country US` (replace US with your country_code)

`sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE`

`sudo netfilter-persistent save`

`sudo reboot`

For troubleshooting see [this page](https://learn.adafruit.com/setting-up-a-raspberry-pi-as-a-wifi-access-point/test-and-troubleshoot)

# Acknowledgements

This installation is a derivative of the `.abstraction()` installation that was originally commissioned by the [CODA Museum](https://www.coda-apeldoorn.nl/) in Apeldoorn, The Netherlands.

This project was made possible thanks to the usage following amazing platforms and libraries:

- Raspberry Pi
	- [https://www.raspberrypi.com/](https://www.raspberrypi.com/)
- Raspberry Pi OS (Raspbian)
	- [https://www.raspberrypi.com/software/](https://www.raspberrypi.com/software/)
- Chromium Browser
	- [https://www.chromium.org/Home/](https://www.chromium.org/Home/)
- Arduino
	- [https://www.arduino.cc/](https://www.arduino.cc/)
- Wemos Lolin32
	- [https://www.wemos.cc/en/latest/](https://www.wemos.cc/en/latest/)
- OSC For Arduino by Yotam Mann and Adrien Freed
	- [https://github.com/CNMAT/OSC](https://github.com/CNMAT/OSC)
- HD44780 LCD by Bill Perry
	- [https://github.com/duinoWitchery/hd44780](https://github.com/duinoWitchery/hd44780)
- NodeJS
	- [https://nodejs.org/en/](https://nodejs.org/en/)
- Node Package Manager (npm)
	- [https://www.npmjs.com/](https://www.npmjs.com/)
- ExpressJS
	- [https://expressjs.com/](https://expressjs.com/)
- Socket.io by Automattic
	- [https://socket.io/](https://socket.io/)
- Process Manager 2
	- [https://www.npmjs.com/package/pm2](https://www.npmjs.com/package/pm2)
- p5.JS by Lauren Lee McCarthy
	- [https://editor.p5js.org/](https://editor.p5js.org/)
- Hydra Synth by Olivia Jack
	- [https://hydra.ojack.xyz/](https://hydra.ojack.xyz/)

# License

The software in this repository is licensed under the [**GNU GPLv3** license](https://choosealicense.com/licenses/gpl-3.0/)

The creative output of this work is licensed under the [**CC BY-SA 4.0** license](https://creativecommons.org/licenses/by-sa/4.0/legalcode)

This is a [**Free Culture License**](https://creativecommons.org/share-your-work/public-domain/freeworks)!

You are free to:

- `Share` — copy and redistribute the material in any medium or format

- `Adapt` — remix, transform, and build upon the material for any purpose, even commercially.

Under the following terms:

- `Attribution` — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

- `ShareAlike` — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

