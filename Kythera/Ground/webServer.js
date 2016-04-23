"use strict";

/* jshint node: true */

// THIS CODE JUST LETS US FORMAT DATES NICE CAN BE IGNORED

var w = new Array(7); w[0]=  "Sunday"; w[1] = "Monday"; w[2] = "Tuesday";
w[3] = "Wednesday"; w[4] = "Thursday"; w[5] = "Friday"; w[6] = "Saturday";
var m = new Array(12); m[0]=  "January"; m[1] = "February"; m[2] = "March";
m[3] = "April"; m[4] = "May"; m[5] = "June"; m[6] = "July"; m[7] = "August";
m[8] = "September"; m[9] = "October"; m[10] = "November"; m[11] = "December";
var time = function(){
  var d = new Date(); var day = w[d.getDay()]; var month = m[d.getMonth()];
  var date = d.getDate().toString(); var year = d.getFullYear().toString();
  var time; var minute = (d.getMinutes()<10)?"0"+d.getMinutes().toString():d.getMinutes().toString();
  var hour = d.getHours();
  if(hour < 13){
    if (hour === 0){ time = "12" + ":" + minute + " AM";}
    else{ time = (hour < 10)? "0" + hour.toString() + ":" + minute + " am" : hour.toString() + ":" + minute + " am";}
  } else{ hour -= 12;
    time = (hour < 10)? "0" + hour.toString() + ":" + minute + " pm" : hour.toString() + ":" + minute + " pm";
  } return day + ", " + month + " " + date + ", " + year + " " + "at "  + time;
};

// OK BACK TO ACTUAL STUFF

/*
 * This builds on the webServer of previous projects in that it exports the current
 * directory via webserver listing on a hard code (see portno below) port. It also
 * establishes a connection to the MongoDB named 'cs142project6'.
 *
 * To start the webserver run the command:
 *    node webServer.js
 *
 * Note that anyone able to connect to localhost:portNo will be able to fetch any file accessible
 * to the current user in the current directory or any of its children.
 *
 *
 */

var mongoose = require('mongoose');
var async = require('async');

// Load the Mongoose schema for User, Photo, and SchemaInfo
var User = require('./schema/user.js');
var Photo = require('./schema/photo.js');
var SchemaInfo = require('./schema/schemaInfo.js');
var Online = require('./schema/online.js');

// some extra libraries we'll be using
var session = require('express-session');
var bodyParser = require('body-parser');
var multer = require('multer');
var fs = require("fs");
var express = require('express');
var app = express();

// configure body parsing and session libraries
app.use(session({secret: 'secretKey', resave: false, saveUninitialized: false}));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies
var processFormBody = multer({storage: multer.memoryStorage()}).single('uploadedphoto');

//connect to our DB
mongoose.connect('mongodb://localhost/cs142project6');

// We have the express static module (http://expressjs.com/en/starter/static-files.html) do all
// the work for us.
app.use(express.static(__dirname));

// Set up the Logging File and the Raw Downlink File
var log = fs.createWriteStream('./files/log.txt');
log.write("\n\n\nSTARTING NEW LOGGING SESSION \n " + time() + "\n");
var raw = fs.createWriteStream('./files/rawDownlink.txt');
raw.write("\n\n\nSTARTING NEW LOGGING SESSION \n " + time()  + "\n");


app.get('/', function (request, response) {
     response.sendfile(__dirname + '/index.html');
});

// Server Initialization...we should probably use the under the hood html object
// which express wraps over  but since the raw object is returned by app.listen,
// we can probably get away  with calling io.on() before io can totally be created.
var io = require('socket.io');
var server = app.listen(3000, '0.0.0.0', function () {
    var port = server.address().port;
    console.log('Listening at http://localhost:' + port + ' exporting the directory ' + __dirname);
    log.write('Server is online ... \n');
    log.write('Listening at http://localhost:' + port + ' exporting the directory ' + __dirname + ' ...\n')
});
var socketServer = io(server);
var socket_ids = [];

// Let's set up the serial port now
var serialport = require('serialport');// include the library
var SerialPort = serialport.SerialPort; // make a local instance of it
   // get port name from the command line:
var portName = '/dev/tty-usbserial1';

var myPort = new SerialPort(portName, {
   baudRate: 19200,
   // look for return and newline at the end of each data packet.
   // '\n' must be sent by the XBEE to generate a new event
   parser: serialport.parsers.readline("\n")
 });

function showPortOpen() {
    console.log('port open. Data rate: ' + myPort.options.baudRate);
    log.write("Listening for Kythera at data rate: " + myPort.options.baudRate + " ...");
}

// Define behaviors for when we recieve data
myPort.on('open', showPortOpen);

// What to do when our connection opens
function openSocket(socket){
	// this function runs if there's input from the client:
	socket.on('from:controller', function(data) {
    console.log("GOT SOMETHING");
		myPort.write(data);							// send the data to the serial device
    log.write("SENDING at " + time() + "\n")
    log.write(data + "\n\n");
	});

	// this function runs if there's input from the serialport:
	myPort.on('data', function(data) {
    var toSend = {};
    toSend["num_online"] = socket_ids.length;
    toSend["data"] = data;
    toSend["time"] = new Date();
		socket.emit('from:kythera', toSend);		// send the data to the client
    log.write("RECIEVED at " + toSend["time"] + "\n");
    log.write(data + "\n\n");
	});
}

socketServer.on('connection', function(socket){

  console.log('A user connected with '+socket.id);
  if(socket_ids.indexOf(socket.id) < 0){
    socket_ids.push(socket.id);
    log.write('\nA user connected ... \n');
  }

  socket.on('disconnect', function(){
     console.log('user disconnected with ' + socket.id );
     socket_ids.splice(socket_ids.indexOf(socket.id), 1);
     console.log(socket_ids);
     log.write('\nA user disconnected ... \n');
  });
  openSocket(socket);

});

myPort.on('error', showError);

function showPortClose() {
   console.log('port closed.');
   log.write("\n\nSESSION COMPLETE \n\n")
}

function showError(error) {
   console.log('Serial port error: ' + error);
   log.write('Serial port error: ' + error);
}


/*
 **********
 *
 *   Respond to certain URL GET's
 *
 **********
 */

 /*
  * Use GET to let client side access the session status
  */
  app.get('/online', function (request, response) {
      console.log("SERVING up: " + socket_ids.length);
      response.status(200).send(JSON.stringify(socket_ids.length));
  });
