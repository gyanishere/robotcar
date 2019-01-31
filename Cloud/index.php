<?php

/* Route script

1. Receives GPS coordinates as Lat/Lng
2. Connects to Google Maps API to do lookup
3. Gets step-by-step walking directions
4. Spits out walking directions as JSON array

*/

// Functions
function cleanUp($html) {
  $text = strip_tags($html);
  $text = str_replace("Restricted usage road", "", $text);
  $text = str_replace("Destination will be on the left", "", $text);
  $text = str_replace("Destination will be on the right ", "", $text);
  return $text;
}


// allow external api access
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");

// ini_set('display_errors', 1);

$curl = curl_init();

// origin and destination
$origin = rawurlencode($_GET['origin']);
$destination = rawurlencode($_GET['destination']);

// api key
$api_key = 'AIzaSyD1kCe_eq_Cz_BAC0Jnamv1Y-ihpRPgDRQ';



curl_setopt_array($curl, array(
  CURLOPT_URL => "https://maps.googleapis.com/maps/api/directions/json?origin=$origin&destination=$destination&key=$api_key&mode=walking",
  CURLOPT_RETURNTRANSFER => true,
  CURLOPT_ENCODING => "",
  CURLOPT_MAXREDIRS => 10,
  CURLOPT_TIMEOUT => 30,
  CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
  CURLOPT_CUSTOMREQUEST => "GET",
  CURLOPT_HTTPHEADER => array(
    "cache-control: no-cache",
    "postman-token: a538f973-90e1-6e1c-540a-0388f0528543"
  ),
));


if (!$origin or !$destination) {
  // dont run curl

  $response = [
    'type' => 'error',
    'body' => 'missing origin or destination coordinates'
  ];
}
else {
  $curl_response = curl_exec($curl);
  $err = curl_error($curl);

  curl_close($curl);

  if ($err) {
    $response = [
      'type' => 'error',
      'body' => 'curl error'
    ];
  } else {

    $formatted_response = json_decode($curl_response, true);

    // this assumes there is only 1 leg in the trip, and the first route is best
    $steps = $formatted_response['routes'][0]['legs'][0]['steps'];

    $form_steps = [];
    foreach($steps as &$step) {
      $kaduko_step = [
        'maneuver' => 'go-forward',
        'meters' => $step['distance']['value'], // in meters
        'seconds' => $step['duration']['value'], // in seconds (Google Maps thinks we walk approximately 1 meter per second)
        'for_humans' => $step['html_instructions'],
        'textonly_description' => cleanUp($step['html_instructions']).' and continue for '.$step['distance']['value'].' meters'
      ];

      // what if not driving forward, but rather turning in a direction? Google Maps combines turning with the succeeding drive forward, but we will separate those steps here
      if ($step['maneuver'] == 'turn-right' || $step['maneuver'] == 'turn-left') {
        $turn_step = [
          'maneuver' => $step['maneuver'],
          'for_humans' => $step['html_instructions'],
          'textonly_description' => cleanUp($step['html_instructions'])
        ];

        // push this turn step before the forward motion step
        array_push($form_steps, $turn_step);
      }

      array_push($form_steps, $kaduko_step);
    }

    $response = [
      'type' => 'success',
      'body' => $form_steps
    ];
  }
}

header('Content-type: application/json');
echo json_encode( $response );

?>
