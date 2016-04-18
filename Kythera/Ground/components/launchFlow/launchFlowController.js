'use strict';

app.controller('launchFlowController', ['$scope', '$rootScope', '$location', '$resource', '$http',
  function ($scope, $rootScope, $location, $resource, $http) {/* callback to process the FetchModel for the current visiting user */

      $scope.onlineCallback = function(model){
         $scope.$apply(function () {
           $scope.main.num_online = model;
           console.log(JSON.parse(model));
         });
      };

      //make available to all sub controllers
      $scope.main.num_online = $scope.main.FetchModel("/online", $scope.onlineCallback);
}]);
