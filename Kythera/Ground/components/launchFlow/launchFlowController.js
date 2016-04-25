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
      };

      $scope.main.num_online = $scope.main.FetchModel("/online", $scope.onlineCallback);

      //make available to all sub controllers
      $scope.updateOnline = function(){
        $scope.main.num_online = $scope.main.FetchModel("/online", $scope.onlineCallback);
      };

      function checkTime(i) {
          if (i < 10) {i = "0" + i};  // add zero in front of numbers < 10
          return i;
      }

      $scope.setTimer = function() {
          var now = new Date();
          var timer = (now.getTime() - $scope.main.lastStateChange.getTime())/1000;
          var h = Math.floor(timer / 3600);
          var m = Math.floor(timer / 60);
          var s = Math.floor(timer % 100);
          m = checkTime(m);
          s = checkTime(s);
          document.getElementById('sinceTime').innerHTML = "T+"+ m + ":" + s;
          var t = setTimeout($scope.setTimer, 500);
      };

      $scope.setTimer();


      // LETS DO SOME GRAPHS
      
      /*$scope.onClick = function (points, evt) {
        console.log(points, evt);
      };*/

}]);

/*app.directive('chart', function(){
    return{
        restrict: 'A',
        link: function(scope, elem, attrs){

            var chart = null,
                opts  = { };

            var data = scope[attrs.ngModel];

            scope.$watch(attrs.chart, function(v){
                if(!chart){
                    chart = $.plot(elem, v , opts);
                    elem.show();
                }else{
                    chart.setData(v);
                    chart.setupGrid();
                    chart.draw();
                }
            });
        }
    };
});*/
