# Abstraction Minified

A minified version of the [`.abstraction()`](https://github.com/tmhglnd/abstraction) installation that can be run on a RPi2+ with a small LCD screen and controlled with a self-build wireless controller using an ESP32.

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

Open the application and disable the screensaver.

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

# Acknowledgements

This installation is a derivative of the `.abstraction()` installation that was originally commissioned by the [CODA Museum](https://www.coda-apeldoorn.nl/) in Apeldoorn, The Netherlands.

# This all was possible thanks to the following great platforms and libraries

- Raspberry Pi
	- [https://www.raspberrypi.com/](https://www.raspberrypi.com/)
- Raspberry Pi OS (Raspbian)
	- [https://www.raspberrypi.com/software/](https://www.raspberrypi.com/software/)
- Chromium Browser
	- [https://www.chromium.org/Home/](https://www.chromium.org/Home/)
- Arduino Uno
	- [https://www.arduino.cc/](https://www.arduino.cc/)
- HD44780 LCD by Bill Perry
	- [https://github.com/duinoWitchery/hd44780](https://github.com/duinoWitchery/hd44780)
- OSC For Arduino by Yotam Mann and Adrien Freed
	- [https://github.com/CNMAT/OSC](https://github.com/CNMAT/OSC)
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

