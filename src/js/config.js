
//  file: pebble-js-app.js
//  auth: Matthew Clark, SetPebble

// change this token for your project
//var settings = require('settings');

var setPebbleToken = '3T74';


Pebble.addEventListener('appmessage', function(e) {
  var key = e.payload.action;
  if (typeof(key) != 'undefined') {
    var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
      try {
        var ip = '11';
        var port = '12';
        var options = {
          'KEY_IP' : ip,
          'KEY_PORT' : port
        };
        Pebble.sendAppMessage(options,
  function(e) {
    console.log('Weather info sent to Pebble successfully!');
  },
  function(e) {
    console.log('Error sending weather info to Pebble!');
  }
);
      } catch (e) {
        console.log ("didnt send :(");
      }
    }
    /*
    var request = new XMLHttpRequest();
    request.open('GET', 'http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken(), true);
    request.onload = function(e) {
      if (request.readyState == 4)
        if (request.status == 200)
          try {
            Pebble.sendAppMessage(JSON.parse(request.responseText));
          } catch (e) {
          }
    }
    request.send(null); */
  }
});
/*

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken());
});
Pebble.addEventListener('webviewclosed', function(e) {
  if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
    try {
      Pebble.sendAppMessage(JSON.parse(e.response));
      localStorage.setItem(setPebbleToken, e.response);
    } catch(e) {
    }
  }
}); 
*/

var initialized = false;
var options = {};

Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");
  //Pebble.openURL('http://assets.getpebble.com.s3-website-us-east-1.amazonaws.com/pebble-js/configurable.html?'+encodeURIComponent(JSON.stringify(options)));
  Pebble.openURL('http://rssreader.no-ip.org:5091/configuration.html?'+encodeURIComponent(JSON.stringify(options)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  // webview closed
  //Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
    localStorage.setItem(setPebbleToken, e.response);
    var ip = '15';
    var port = '16';
    var cheese = {
          'KEY_IP': ip,
          'KEY_PORT': port
        };
        console.log("ip is:" + ip);
    console.log("port is:" + port);
        Pebble.sendAppMessage(cheese,
  function(e) {
    console.log('Weather info sent to Pebble successfully!');
  },
  function(e) {
    console.log('Error sending weather info to Pebble!');
  }
);
    
  } else {
    console.log("Cancelled");
  }
});


