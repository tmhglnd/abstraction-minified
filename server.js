// require dependencies for server
const express = require('express');
const max = require('max-api-or-not');
const socket = require('socket.io');
const shell = require('child_process');
const rpi = require('detect-rpi');

// add --debug option to not start in full screen and log
const verbose = process.argv[2] === '--debug';

// init the app server and port listen
const app = express();
app.use(express.static('.'));

const port = 3000;
const server = app.listen(port, () => {
	console.log(`Server is running at http://localhost:${port}`);
	console.log(`Run 'node server.js --debug' for logs`);
	
	if (rpi() && !verbose){
		// hide mouse when not moving
		shell.exec(`unclutter -idle 1`);
		// open browser in fullscreen incognito when on rpi
		shell.exec(`chromium-browser --start-fullscreen --start-maximized --incognito http://localhost:${port}`);
	}
});

// connect via socket io
const io = socket(server);

// initialize all visuals
let init = {};

// post socket id to max console
io.sockets.on('connection', function(socket){
	console.log(`Connected ${socket.id}`);

	// randomize initial values
	init = {
		'/control1/function' : Math.floor(Math.random()*5),
		'/control2/function' : Math.floor(Math.random()*5),
		'/control1/value' : Math.random()*4096,
		'/control2/value' : Math.random()*4096,
		'/control1/switch' : 1,
		'/control2/switch' : 1,
	}

	for (i in init){
		console.log('send', i, init[i]);
		io.emit('message', i, init[i]);
	}
});

// require dependency for receiving controller values
const { Server } = require('node-osc');
const oscPort = 9999;

// setup a server to receive OSC messages from controllers
let osc = new Server(oscPort, '0.0.0.0', () => {
	console.log(`OSC server listening at port ${oscPort}`);
	
	// receive messages and forward
	osc.on('message', (msg) => {
		// print received messages if --debug
		if (verbose){ console.log('received:', ...msg) }

		// store the new values as the initials 
		// for when page gets refreshed
		if (init[msg[0]]){
			init[msg[0]] = msg[1];
		}

		// forward to the browser
		io.emit('message', ...msg);
	});
});

// send all parameters from Max (for testing purposes)
max.addHandler('message', (...v) => {
	io.emit('message', ...v);
});