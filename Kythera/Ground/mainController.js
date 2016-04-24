'use strict';

var app = angular.module('kytheraApp', ['ngRoute',
    'ngMaterial',
    'ngResource',
    'btford.socket-io',
    'angular-flot'
  ]).
  config(function ($routeProvider) {
        $routeProvider.
            when('/launchFlow', {
                templateUrl: 'components/launchFlow/launchFlow.html',
                controller: 'launchFlowController'
            }).
            when('/login-register', {
                templateUrl: 'components/login-register/login-registerTemplate.html',
                controller: 'LoginRegisterController'
            }).
            when('/rawDownlink', {
                templateUrl: 'components/rawDownlink/rawDownlink.html',
                controller: 'rawDownlinkController'
            }).
            otherwise({
                redirectTo: '/login-register'
            });

    });

// SET UP THE SOCKETS
app.factory('socket', function ($rootScope) {
  var socket = io.connect();
  return {
    on: function (eventName, callback) {
      socket.on(eventName, function () {
        var args = arguments;
        $rootScope.$apply(function () {
          callback.apply(socket, args);
        });
      });
    },
    emit: function (eventName, data, callback) {
      socket.emit(eventName, data, function () {
        var args = arguments;
        $rootScope.$apply(function () {
          if (callback) {
            callback.apply(socket, args);
          }
        });
      })
    }
  };
});

app.controller('MainController', ['$scope', '$rootScope', '$location', '$resource', '$http','socket',
  function ($scope, $rootScope, $location, $resource, $http, socket) {

        $scope.main = {};

        // create some prarams that will be shared by all controllers
        $scope.main.permission = false;
        $scope.main.project = "Kythera Ground Station";
        $scope.main.message = "Please Login"
        $scope.main.session = {};

        $scope.main.here = false;
        $scope.main.num_online = 0;
        $scope.main.stages = 24;
        $scope.main.currentStage = 0;
        $scope.main.errorCount = 0;
        $scope.main.rocketeers = "Rocketers";
        $scope.main.downlinkStream = [];

        $scope.main.accelY = [ [[0, 1], [1, 5], [2, 2], [3, 2]] ];
        $scope.main.myChartOptions = {};

        /* Directives contain
         * - profile photo
         * - profile name
         * - time
         * - message
         */
        $scope.main.messages = [];

        var adding = {};
        adding["source"] = "/images/kythera.png";
        adding["name"] = "Kythera";
        adding["message"] = "Hey, I'm Kythera! I'm pretty chatty so I'll be asking you some questions before flight :D";
        adding["time"] = new Date();

        $scope.main.messages.push(adding);

        $scope.main.progress = $scope.main.currentStage + "/" + $scope.main.stages;

        /*socket.on('send:message', function (message) {
          $scope.main.messages.push(message);
        });*/

        socket.on('connect', function(data) {
            //socket.emit('join', 'Hello World from client');
            console.log("client connected");
        });

        var most_recent = new Date();
        socket.on('from:kythera', function(message){
            message = JSON.parse(message);
            console.log("FROM KYTHERA: " + message);
            most_recent = new Date(message.time);

            $scope.main.num_online = message.num_online;

            // add the message to our model locally
            $scope.main.messages.push({
              source:"/images/kythera.png",
              name: "Kythera",
              message: message.data,
              time: most_recent
            });
        });

        $scope.main.healthy = false;

        setInterval(function(){
            $scope.$apply( function(){
                var right_now = new Date();
                var seconds = (right_now.getTime() - most_recent.getTime())/1000;
                if(seconds > 10){
                  $scope.main.healthy = false;
                } else{
                  $scope.main.healthy = true;
                }
            });
        }, 3000);

        $scope.main.send = function(toSend){
            socket.emit('from:controller', {
              message: toSend
            });
            console.log("FROM USER: " + toSend);
        };

        /*
         * FetchModel - Fetch a model from the web server.
         *   url - string - The URL to issue the GET request.
         *   doneCallback - function - called with argument (model) when the
         *                  the GET request is done. The argument model is the object
         *                  containing the model. model is undefined in the error case.
         */
        $scope.main.FetchModel = function(url, doneCallback) {
          var model;
          var error = false;
          /* create the XMLHttpRequest object */
          var xhttp;
          xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            //Don’t do anything if not final state
           if (this.readyState !== 4){
             return;
           }
           //Final State but status not OK
           if (this.status !== 200) {
             error = true;
             var empty;
             doneCallback(empty);
             return;
           }
           //Final State
           if (this.readyState === 4 && this.status === 200) {
             doneCallback(this.responseText);
             return;
           }
         };
          xhttp.open("GET", url);
          xhttp.send();
        };


        //RESTRICT TO LOGIN SCREEN
        $rootScope.$on( "$routeChangeStart", function(event, next, current) {
          if (!$scope.main.here) {
               // no logged user, redirect to /login-register unless already there
            if (next.templateUrl !== "components/login-register/login-registerTemplate.html") {
                  $location.path("/login-register");
            }
          }
        });



       /////
       //
       //  MISC DATE FORMATING FUNCTION
       //
       /////

        //Make formatted time available to all sub controllers
        /* Create arrays containing names od days and names of months */
        var weekday = new Array(7);
        weekday[0]=  "Sunday";
        weekday[1] = "Monday";
        weekday[2] = "Tuesday";
        weekday[3] = "Wednesday";
        weekday[4] = "Thursday";
        weekday[5] = "Friday";
        weekday[6] = "Saturday";

        var months = new Array(12);
        months[0]=  "January";
        months[1] = "February";
        months[2] = "March";
        months[3] = "April";
        months[4] = "May";
        months[5] = "June";
        months[6] = "July";
        months[7] = "August";
        months[8] = "September";
        months[9] = "October";
        months[10] = "November";
        months[11] = "December";

        /* constructs the date in a more readable form returns it as a string */
        $scope.main.formatTime = function(passed){
          var d = passed;
          var day = weekday[d.getDay()];
          var month = months[d.getMonth()];
          var date = d.getDate().toString();
          var year = d.getFullYear().toString();
          var time;

          /* include leading 0 to minutes below 10 */
          var minute = (d.getMinutes() < 10) ? "0" + d.getMinutes().toString() : d.getMinutes().toString();
          var seconds = (d.getSeconds() < 10) ? "0"+ d.getSeconds().toString() : d.getSeconds().toString();

          /* distinguish between AM and PM */
          var hour = d.getHours();
          if(hour < 13){
            /* 12 am is a special case here since it is returned as a 0 but 00 is not an acceptable format */
            if (hour === 0){
              time = "12" + ":" + minute + " AM";
            }
            /* all other times under 10am require a leading 0 */
            else{
              time = (hour < 10)? "0" + hour.toString() + ":" + minute + ":" + seconds +" am" : hour.toString() + ":" + minute + ":" + seconds + " am";
            }
          } else{
            /* pm times must be in 12 hour format and times under 10 must have a leading 0*/
            hour -= 12;
            time = (hour < 10)? "0" + hour.toString() + ":" + minute + ":" + seconds +" pm" : hour.toString() + ":" + minute + ":" + seconds + " pm";
          }

          /* return the formatted stirng */
          return day + ", " + month + " " + date + ", " + year + " " + "at "  + time;
        };

    }]);
