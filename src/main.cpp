/* Create a WiFi access point and provide a web server on it.
** For more details see http://42bots.com.
*/
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WebServer.h>

#include "effects.cpp"

const char *ssid = "Long LED"; // The name of the Wi-Fi network that will be created
const char *password = "";   // The password required to connect to it, leave blank for an open network
int strip_mode = 0;
short int red = 80;
short int green = 40;
short int blue = 225;
short int brightness = 50;
int cooling = 55, sparkling = 120, fire_delay = 15;
int rainbow_speeddelay = 20;
int run_lights_wavedelay = 50;
int sparkle_speeddelay = 0;
int colorwipe_speed = 50;

ESP8266WebServer server(80);

String html_base();
String html_main();
void redirect_to_main();
void mode_0();
void mode_1();
void mode_2();
void mode_3();
void mode_4();
void mode_5();
void mode_6();
void mode_7();
void mode_8();
void mode_9();
void mode_10();
void main_page();
void leds_on();

String html_base() {
    String page = String("<html>") + 
        "<head> <title>Led Controll</title>" +
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" +
        "<style> *{font-size: 1.15em; font-family: sans-serif;}"+
        "a { display: block; margin: 10px auto; padding: 10px 15px; text-decoration: none;"+
        "background-color: #2c6dd6; color: #eaeaea; text-align: center; border-radius: 4px;}" + 
        "p { padding-left: 10px; border-left: 6px solid blue; border-radius: 5px;"+
        "color: #222;}" +
        "input[type=\"submit\"] {display: block; margin: 10px auto; background-color: 2c6dd6;" +
        "color: #eaeaea; text-align: center; width: 100%;  height: 2em; border-radius: 5px; border-width: 0;}" +  
        "input[type=\"text\"] {border: 2px solid #2c6dd6; border-radius: 4px; margin-left: 10px}" +
        "input[type=range] {height: 21px; -webkit-appearance: none; margin: 10px 0; width: 100%;} input[type=range]:focus {outline: none;} input[type=range]::-webkit-slider-runnable-track { width: 100%; height: 8px cursor: pointer; animate: 0.2s; box-shadow: 0px 0px 0px #000000;background: #2C6DD6; border-radius: 15px; border: 1px solid #000000;} input[type=range]::-webkit-slider-thumb {box-shadow: 0px 0px 10px #000000;border: 0px solid #000000; height: 15px; width: 15px; border-radius: 10px;background: #FFFFFF; cursor: pointer; -webkit-appearance: none; margin-top: -4px;} input[type=range]:focus::-webkit-slider-runnable-track {background: #2C6DD6;} input[type=range]::-moz-range-track {width: 100%;height: 8px; cursor: pointer; animate: 0.2s; box-shadow: 0px 0px 0px #000000; background: #2C6DD6; border-radius: 15px; border: 1px solid #000000;} input[type=range]::-moz-range-thumb {box-shadow: 0px 0px 10px #000000; border: 0px solid #000000; height: 15px; width: 15px; border-radius: 10px; background: #FFFFFF; cursor: pointer;} input[type=range]::-ms-track {width: 100%; height: 8px; cursor: pointer;animate: 0.2s; background: transparent;border-color: transparent;color: transparent;} input[type=range]::-ms-fill-lower {background: #2C6DD6;border: 1px solid #000000;border-radius: 30px;box-shadow: 0px 0px 0px #000000;} input[type=range]::-ms-fill-upper {background: #2C6DD6;border: 1px solid #000000;border-radius: 30px;box-shadow: 0px 0px 0px #000000;}input[type=range]::-ms-thumb {margin-top: 1px;box-shadow: 0px 0px 10px;#000000;border: 0px solid #000000;height: 15px;width: 15px;border-radius: 10px;background: #FFFFFF;cursor: pointer;}input[type=range]:focus::-ms-fill-lower {background: #2C6DD6;}input[type=range]:focus::-ms-fill-upper {background: #2C6DD6;}" +
        "</style>" + 
        "</head>";
    return page;
}

String html_main() {
    String page = String("<body>") + 
        "<a href=\"/turnoff\">Turn off</a>" + 
        "<a href=\"/whole\">Whole color</a>" + 
        "<a href=\"/meteor\">Meteor to Earth</a>" +
		"<a href=\"/cyclon\">Cyclon</a>" +
		// "<a href=\"/balls\">Balls, My Balls</a>" +
		"<a href=\"/fire\">Fire or IGNITE ME</a>" +
		"<a href=\"/rainbow\">Rainbowshit</a>" +
		"<a href=\"/twinkle\">Twinkle</a>" +
		"<a href=\"/runninglights\">Run The Light</a>" +
		"<a href=\"/sparkle\">Sparkles MAAAN</a>" +
		"<a href=\"/wipe\">Wipe your life</a>" +
        "<form action=\"/\" method=\"GET\">" +
        "<p><label>Brightness<input type=\"text\" name=\"brightness\" value=\"" + String(brightness) + "\" style=\"max-width: 300px;\"></label></p>" +
        "<p><label>R<input type=\"range\" name=\"red\"  min=\"0\" max=\"255\"  value=\"" + String(red) + "\"></label></p>" +
        "<p><label>G<input type=\"range\" name=\"green\"  min=\"0\" max=\"255\"  value=\"" + String(green) + "\"></label></p>" +
        "<p><label>B<input type=\"range\" name=\"blue\"  min=\"0\" max=\"255\"  value=\"" + String(blue) + "\"></label></p>" +
        "<input type=\"submit\" value=\"Send\">"
        "</body>" ;
    return page;
}

void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  server.begin();
  server.on("/", main_page);
  server.on("/turnoff", mode_0);
  server.on("/whole", mode_1);
  server.on("/meteor", mode_2);
  server.on("/cyclon", mode_3);
  // server.on("/balls", mode_4);
  server.on("/fire", mode_5);
  server.on("/rainbow", mode_6);
  server.on("/twinkle", mode_7);
  server.on("/runninglights", mode_8);
  server.on("/sparkle", mode_9);
  server.on("/wipe", mode_10);
  Serial.println("Server started");
}



void redirect_to_main() {
    String page = String("<head>") +
    "<meta http-equiv=\"refresh\" content=\"1;URL=http://192.168.4.1\" />\""+ 
    "</head>";
    server.send(200, "text/html", page);
}

void mode_0() {
    strip_mode = 0;
    String page = html_base() + "<p>Turned off!</p><br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() Turned off");
}
void mode_1() {
    strip_mode = 1;
    String page = html_base() + "<p>Light ON!!!</p><br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);\
    Serial.println("() Whole mode");
}
void mode_2() {
    strip_mode = 2;
    String page = html_base() + "<p>Meteor reaches the Earth!!!</p><br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() Meteor mode");
}
void mode_3() {
    strip_mode = 3;
    String page = html_base() + "<p>HIDE ALL!!! THE CYCLON IS MOVING TO!!!</p><br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() Cyclon mode");
}
void mode_4() {
    strip_mode = 4;
    String page = html_base() + "<p>I am just bouncing my balls)))</p><br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() BouncingBalls mode");
}
void mode_5() {
    if (server.args() > 0)
    {
        for (int i = 0; i < server.args(); i++) {
            String name = server.argName(i);
            String value = server.arg(i);
            if (name == "cooling") {
                cooling = value.toInt();
            }
            if (name == "sparkling") {
                sparkling = value.toInt();
            }
            if (name == "delay") {
                fire_delay = value.toInt();
            }
        } 
    }
    strip_mode = 5;
    String page = html_base() + "<p>IGNITE ME!!!</p>" +
    "<form method=\"GET\" >" +
    "<p><label>Cooling<input type=\"number\" name=\"cooling\" min=\"0\" value=\"" + cooling + "\"" + "</label></p>" +
    "<p><label>Sparkling<input type=\"number\" name=\"sparkling\" min=\"0\" value=\"" + sparkling + "\"" + "</label></p>" +
    "<p><label>Fire Delay<input type=\"number\" name=\"delay\" min=\"0\" value=\"" + fire_delay + "\"" + "</label></p>" +
    "<input type=\"submit\" value=\"Send\">" +
    "</form>" +
    "<br><a href=\"/\">Main</a> ";;
    server.send(200, "text/html", page);
    Serial.println("() Fire mode");
}
void mode_6() {
    if (server.args() > 0)
    {
        for (int i = 0; i < server.args(); i++) {
            String name = server.argName(i);
            String value = server.arg(i);
            if (name == "delay") {
                rainbow_speeddelay = value.toInt();
            }
        } 
    }
    strip_mode = 6;
    String page = html_base() + "<p>Call in the Rainbow Dash!</p>" +
    "<form method=\"GET\" >" +
    "<p><label>Rainbow speed<input type=\"number\" name=\"delay\" min=\"0\" value=\"" + rainbow_speeddelay + "\"" + "</label></p>" +
    "<input type=\"submit\" value=\"Send\">" +
    "</form>" +
    "<br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() Rainbow mode");
}
void mode_7() {
    strip_mode = 7;
    String page = html_base() + "<p>just some twinks, man...</p><br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() TwinkleRandom mode");
}
void mode_8() {
    if (server.args() > 0)
    {
        for (int i = 0; i < server.args(); i++) {
            String name = server.argName(i);
            String value = server.arg(i);
            if (name == "delay") {
                run_lights_wavedelay = value.toInt();
            }
        } 
    }
    strip_mode = 8;
    String page = html_base() + "<p>DISCO!!</p>" +
    "<form method=\"GET\" >" +
    "<p><label>Wave delay<input type=\"number\" name=\"delay\" min=\"0\" value=\"" + run_lights_wavedelay + "\"" + "</label></p>" +
    "<input type=\"submit\" value=\"Send\">" +
    "</form>" +
    "<br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() RunningLights mode");
}
void mode_9() {
    if (server.args() > 0)
    {
        for (int i = 0; i < server.args(); i++) {
            String name = server.argName(i);
            String value = server.arg(i);
            if (name == "delay") {
                sparkle_speeddelay = value.toInt();
            }
        } 
    }
    strip_mode = 9;
    String page = html_base() + "<p>Such a nice sparkles <3</p>" +
    "<form method=\"GET\" >" +
    "<p><label>Sparkle delay<input type=\"number\" name=\"delay\" min=\"0\" value=\"" + sparkle_speeddelay + "\"" + "</label></p>" +
    "<input type=\"submit\" value=\"Send\">" +
    "</form>" +
    "<br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() Sparkle mode");
}
void mode_10() {
    if (server.args() > 0)
    {
        for (int i = 0; i < server.args(); i++) {
            String name = server.argName(i);
            String value = server.arg(i);
            if (name == "speed") {
                colorwipe_speed = value.toInt();
            }
        } 
    }
    strip_mode = 10;
    String page = html_base() + "<p>I will wipe you!</p>" + 
    "<form method=\"GET\" >" +
    "<p><label>Speed<input type=\"number\" name=\"speed\" min=\"0\" value=\"" + colorwipe_speed + "\"" + "</label></p>" +
    "<input type=\"submit\" value=\"Send\">" +
    "</form>" +
    "<br><a href=\"/\">Main</a> ";
    server.send(200, "text/html", page);
    Serial.println("() Wipe mode"); 
}

void main_page() {
    if (server.args() > 0) {
        String message = String("Number of args received:");
        message += server.args();
        message += "\n";

        for (int i = 0; i < server.args(); i++) {
            String name = server.argName(i);
            String value = server.arg(i);
            message += "Arg nº" + (String)i + " –> ";
            message += name + ": ";
            message += value + "\n";
            if (name == "brightness") {
                brightness = value.toInt();
                FastLED.setBrightness(brightness);
            }
            if (name == "red") {
                red = value.toInt();
            } 
            if (name == "green") {
                green = value.toInt();
            }
            if (name == "blue") {
                blue = value.toInt();
            }
        } 
        

        // server.send(200, "text/plain", message);
        redirect_to_main();
    } else {
        String page = html_base() + html_main();
        server.send(200, "text/html", page);
    }
}


void loop() { 
    server.handleClient();
    leds_on();
}

// LEDS NOW

void leds_on() {
    switch (strip_mode){
        case 0: {
            setAll(0x00, 0x00, 0x00);
            break;
        }
        case 1: {
            setAll(red, green, blue);
            break;
        }
        case 2: {
            meteorRain(red, green, blue, 5, 48, true, 45);
            break;
        }
        case 3: {
            CylonBounce(red, green, blue, 4, 10, 50);
            break;
        }
        case 4: {
            BouncingBalls(red, green, blue, 5);
            break;
        }
        case 5: {
            Fire(cooling, sparkling, fire_delay);
            break;
        }
        case 6: {
            rainbowCycle(rainbow_speeddelay);
            break;
        }
        case 7: {
            TwinkleRandom(200, 70, false);
            break;
        }
        case 8: {
            RunningLights(red,green,blue, run_lights_wavedelay);
            break;
        }
        case 9: {
            Sparkle(red, green, blue, sparkle_speeddelay);
            break;
        }
        case 10: {
            colorWipe(red, green, blue, colorwipe_speed);
            colorWipe(0x00,0x00,0x00, colorwipe_speed);
            break;
        }
        
    }
}
