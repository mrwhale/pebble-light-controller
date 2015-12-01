
//  file: config.js

// change this token for your project
//var settings = require('settings');

var initialized = false;
var options = {};
//var Settings = require('settings');
//var options = Settings.option();


/*
Pebble.addEventListener('appmessage', function(e) {
  var key = e.payload.action; 
  if (typeof(key) != 'undefined') {
    var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
        var ip = '10.1.1.15';
        var port = '8899';
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
    }

  } 
}); */

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



Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
  console.log("Hrere is ip :" + localStorage.getItem('ip'));
});

Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");
  //Pebble.openURL('http://assets.getpebble.com.s3-website-us-east-1.amazonaws.com/pebble-js/configurable.html?'+encodeURIComponent(JSON.stringify(options)));
  Pebble.openURL('https://rss.mrwhal3.com:5090/configuration.html?'+encodeURIComponent(JSON.stringify(options)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  // webview closed
  //Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
    localStorage.setItem('ip', options.ip);
    localStorage.setItem('port',options.port);
    var ip = '10.1.1.15';
    var port = '8899';
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


