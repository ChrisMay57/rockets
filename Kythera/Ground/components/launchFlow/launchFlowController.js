'use strict';

app.controller('launchFlowController', ['$scope', '$rootScope', '$location', '$resource', '$http',
  function ($scope, $rootScope, $location, $resource, $http) {/* callback to process the FetchModel for the current visiting user */

      $scope.onlineCallback = function(model){
         $scope.$apply(function () {
           $scope.main.num_online = model;
           console.log(JSON.parse(model));
           if($scope.main.num_online == 1){
             $scope.main.rocketeers = "Rocketeer";
           }else{
             $scope.main.rocketeers = "Rocketers";
           }
         });
      };

      $scope.sendMessage = function(){
        console.log(this.text);

        $scope.main.send(this.text);

        // add the message to our model locally
        $scope.main.messages.push({
          source:"/images/human.png",
          name: "Controller",
          message: this.text,
          time: new Date()
        });

        document.getElementById("chatting").reset();
      }

      //make available to all sub controllers
      $scope.main.num_online = $scope.main.FetchModel("/online", $scope.onlineCallback);

      console.log($scope.main.messages);

}]);
