(function(){
  angular.module('starter').controller('DeviceController', ['$scope', '$state', '$http', '$stateParams', 'DeviceFactory', DeviceController]);

  function DeviceController($scope, $state, $http, $stateParams, DeviceFactory){

    var me = this;

    var service_id = '12ab';
    var characteristic_id = '34cd';

    $scope.init = function(){
      // $scope.device = DeviceFactory.getDevice($stateParams.id);

      // also start

      $scope.steps = [];

      $scope.steps.push({
        html: 'Beginning route...'
      })

      function getDirectives() {

        console.info('Directive generation beginning! Starting by grabbing GPS coords...');

        var routeStep = function(position) {
            var lat = position.coords.latitude;
            var lng = position.coords.longitude;

            var origin = lat+','+lng;

            // var origin = '34.125871,-118.210910';
            var destination = '34.128124, -118.211335';

          $http.get('http://138.197.205.168?origin='+origin+'&destination='+destination)
          .then(function (result) {
            console.log('Made request to navigational cloud')

            var steps = result.data.body;;
            /*

            Converts navigational steps into explicit motor directives

            --> Directives
            1: Forward
            2: Left turn
            3: Right turn
            4: Reverse
            5: Stop

            --> Syntax
            var step = {Directive}.{Microseconds}

            */

            var step = steps[0];
            step.microseconds = step.seconds * 1000; // need to calculate this according to robot speed
            switch(step.maneuver) {
              case 'go-forward':
                directive = '1.'+step.microseconds;
                break;
              case 'turn-left':
                step.microseconds = '?'; // for turns, the nav script will determine
                directive = '2.'+step.microseconds;
                break;
              case 'turn-right':
              step.microseconds = '?'; // for turns, the nav script will determine
                directive = '3.'+step.microseconds;
                break;
              default:
                console.error('Step could not be converted into directive');
                break;
            }

            // alert(step.textonly_description+'\n'+'Directive:'+directive)

            $scope.steps.push({
              html: step.textonly_description+' -- Directive:'+directive
            })

            console.info(directive);

            // for mobile
            // alert(directive);

            /* Send this to BLE

            We will send the X.X directive (type.microseconds) to the BLE, but the BLE might not listen.

            Why? Well, what if the robot is driving forward along X Road and has 15 s remaining. If we tell robot to drive forward for 15 seconds, it will disregard this command and continue on its path since the motor directive types match

            However, if we suddenly suggest a turn, then the robot will halt its current task and perform the turn

            */
            ble.write(
              $stateParams.id,
              service_id,
              characteristic_id,
              btoa(directive),
              function(response) {
                console.log(response)
              },
              function(err) {
                console.error(err)
              }
            );

          }, function (result) {
            console.group('Could not make request to navigational cloud')
            console.error(result);
            console.groupEnd();
          })

            // console.info('Latitude: '          + position.coords.latitude          + '\n' +
            //       'Longitude: '         + position.coords.longitude         + '\n' +
            //       'Altitude: '          + position.coords.altitude          + '\n' +
            //       'Accuracy: '          + position.coords.accuracy          + '\n' +
            //       'Altitude Accuracy: ' + position.coords.altitudeAccuracy  + '\n' +
            //       'Heading: '           + position.coords.heading           + '\n' +
            //       'Speed: '             + position.coords.speed             + '\n' +
            //       'Timestamp: '         + position.timestamp                + '\n');
        };

        // onError Callback receives a PositionError object
        //
        function onError(error) {
            console.warn('code: '    + error.code    + '\n' +
                  'message: ' + error.message + '\n');
        }

        navigator.geolocation.getCurrentPosition(routeStep, onError);

        // $cordovaGeolocation
        //   .getCurrentPosition(posOptions)
        //   .then(function (position) {
        //     var lat  = position.coords.latitude
        //     var lng = position.coords.longitude
        //   }, function(err) {
        //     console.error('Could not get location')
        //   });


      }

      // set timer for every 5 seconds, starting now!
      getDirectives();
      setInterval(function(){
        getDirectives();
      }, 5000);

    }

    $scope.send = function(){
      ble.write(
        $stateParams.id,
        service_id,
        characteristic_id,
        btoa(me.input),
        function(response){
          alert(response)
        },
        function(err){
          alert(err)
        }
      );
    }

    $scope.backToHome = function(){
      $state.go('home');
      ble.disconnect($stateParams.id);
    }

  }

})();
