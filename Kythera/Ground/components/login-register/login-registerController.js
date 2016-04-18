'use strict';

app.controller('LoginRegisterController', ['$scope', '$rootScope', '$location', '$resource', '$http','socket',
  function ($scope, $rootScope, $location, $resource, $http, socket) {
    /*
     * Controls the login view whcih allows users to sign up
     * or login to existing accounts. Currently just lets users
     * select their permission level with no check.
     */

     if($scope.main.here){
       $scope.main.message = "Please login";
       $scope.main.here = false;
     }

    // Let the user logon
    $scope.logon = function(num) {
      //ensure required fields are provided
      if(num === 1){
        $scope.main.permission = true;
        $scope.main.message = "Logout of Kythera Control Session ";
      }else{
        $scope.main.message = "Logout of View-Only Session ";
      }

      $scope.main.welcome = "Welcome to Hawthorne 😎";
      window.location = "#/launchFlow";
      $scope.main.here = true;
    };
  }]);
