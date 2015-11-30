// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    console.log ("payload: " + e.payload);
    var requestPayloadAsJsonString = JSON.stringify (e.payload);
    console.log ("    stringified: " + requestPayloadAsJsonString);
    //getWeather();
    
  }                     
);