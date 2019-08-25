#include <Arduino.h>

String base() {
    String page = String("<html>") + 
        "<head>" +
        "<title>Led Controll</title>" +
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" +
        "<style>" +
            "* { font-size: 1.15em;" +
                "font-family: sans-serif; }" +
            "a { display: block;" +
                "margin: 10px auto;" +
                "padding: 10px 15px;" + 
                "text-decoration: none;"+
                "background-color: #2c6dd6;" +
                "color: #eaeaea;" +
                "text-align: center;" +
                "border-radius: 4px; }" + 
            "p { padding-left: 10px;" +
                "border-left: 6px solid blue;" +
                "border-radius: 5px;"+
                "color: #222; }" +
            "input[type=\"submit\"] { " +
                "display: block;" +
                "margin: 10px auto;" +
                "background-color: 2c6dd6;" +
                "color: #eaeaea;" +
                "text-align: center;" +
                "width: 100%;" +
                "height: 2em;" +
                "border-radius: 5px;" +
                "border-width: 0; }" +  
            "input[type=\"text\"] { " +
                "border: 2px solid #2c6dd6;" +
                "border-radius: 4px;" +
                "margin-left: 10px }" +
            "input[type=range] { " +
                "height: 21px;" +
                "-webkit-appearance: none;" +
                "margin: 10px 0;" +
                "width: 100%;}" +
            "input[type=range]:focus { " +
                "outline: none;}" +
            "input[type=range]::-webkit-slider-runnable-track { " +
                "width: 100%;" +
                "height: 8px;" +
                "cursor: pointer;" +
                "animate: 0.2s;" +
                "box-shadow: 0px 0px 0px #000000;" +
                "background: #2C6DD6;" +
                "border-radius: 15px;" +
                "border: 1px solid #000000;}" +
            "input[type=range]::-webkit-slider-thumb { " +
                "box-shadow: 0px 0px 10px #000000;" +
                "border: 0px solid #000000;" +
                "height: 15px;" +
                "width: 15px;" +
                "border-radius: 10px;" +
                "background: #FFFFFF;" +
                "cursor: pointer;" +
                "-webkit-appearance: none;" +
                "margin-top: -4px;}" +
            "input[type=range]:focus::-webkit-slider-runnable-track { " +
                "background: #2C6DD6;}" +
            "input[type=range]::-moz-range-track { " +
                "width: 100%;" +
                "height: 8px;" +
                "cursor: pointer;" +
                "animate: 0.2s;" +
                "box-shadow: 0px 0px 0px #000000;" +
                "background: #2C6DD6;" +
                "border-radius: 15px;" +
                "border: 1px solid #000000;}" +
            "input[type=range]::-moz-range-thumb { " +
                "box-shadow: 0px 0px 10px #000000;" +
                "border: 0px solid #000000;" +
                "height: 15px;" +
                "width: 15px;" +
                "border-radius: 10px;" +
                "background: #FFFFFF;" +
                "cursor: pointer;}" +
            "input[type=range]::-ms-track { " +
                "width: 100%;" +
                "height: 8px;" +
                "cursor: pointer;" +
                "animate: 0.2s;" +
                "background: transparent;" +
                "border-color: transparent;" +
                "color: transparent;}" +
            "input[type=range]::-ms-fill-lower { " +
                "background: #2C6DD6;" +
                "border: 1px solid #000000;" +
                "border-radius: 30px;" +
                "box-shadow: 0px 0px 0px #000000;}" +
            "input[type=range]::-ms-fill-upper { "
                "background: #2C6DD6;" +
                "border: 1px solid #000000;" +
                "border-radius: 30px;" +
                "box-shadow: 0px 0px 0px #000000;}" +
            "input[type=range]::-ms-thumb {" +
                "margin-top: 1px;" +
                "box-shadow: 0px 0px 10px #000000;" +
                "border: 0px solid #000000;" +
                "height: 15px;" +
                "width: 15px;" +
                "border-radius: 10px;" +
                "background: #FFFFFF;" +
                "cursor: pointer;}" +
            "input[type=range]:focus::-ms-fill-lower { " +
                "background: #2C6DD6;}" +
            "input[type=range]:focus::-ms-fill-upper { " +
                "background: #2C6DD6;}" +
        "</style>" + 
        "</head>";
    return page;
}

String main_page(byte brightness, byte red, byte green, byte blue) {
    String page = String("") + 
        "<body>" +
            "<a href=\"/turnoff/\">Turn off</a>" + 
            "<a href=\"/whole/\">Whole color</a>" + 
            "<a href=\"/comets/\">Comets</a>" + 
            // "<a href=\"/meteor\">Meteor to Earth</a>" +
            // "<a href=\"/cyclon\">Cyclon</a>" +
            // // "<a href=\"/balls\">Balls, My Balls</a>" +
            // "<a href=\"/fire\">Fire or IGNITE ME</a>" +
            // "<a href=\"/rainbow\">Rainbowshit</a>" +
            // "<a href=\"/twinkle\">Twinkle</a>" +
            // "<a href=\"/runninglights\">Run The Light</a>" +
            // "<a href=\"/sparkle\">Sparkles MAAAN</a>" +
            // "<a href=\"/wipe\">Wipe your life</a>" +
            
            "<form action=\"/\" method=\"GET\">" +
                "<p>" +
                    "<label for=\"brightness\">Brightness</label>" +
                    "<input type=\"text\"" +
                        "id=\"brightness\"" +
                        "name=\"brightness\"" +
                        "value=\"" + String(brightness) + "\" " +
                        "style=\"{max-width: 300px;}\">" +
                "</p>" +
                "<p>" +
                    "<label for=\"red\">R</label>" +
                    "<input type=\"range\"" +
                        "id=\"red\"" +
                        "name=\"red\"" +
                        "min=\"0\"" +
                        "max=\"255\"" +
                        "value=\"" + String(red) + "\">" +
                "</p>" +
                "<p>" +
                    "<label for\"green\">G</label>" +
                    "<input type=\"range\"" +
                        "id=\"green\"" +
                        "name=\"green\"" +
                        "min=\"0\"" +
                        "max=\"255\"" +
                        "value=\"" + String(green) + "\">" +
                "</p>" +
                "<p>" +
                    "<label for=\"blue\">B</label>" +
                    "<input type=\"range\"" +
                        "id=\"blue\"" +
                        "name=\"blue\"" +
                        "min=\"0\"" +
                        "max=\"255\"" +
                        "value=\"" + String(blue) + "\">" +
                "</p>" +
                "<input type=\"submit\" value=\"Send\">" +
            "</form>" +
        "</body>" ;
    return base() + page;
}