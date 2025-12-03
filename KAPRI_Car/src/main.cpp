#include <WiFi.h>
#include <WebServer.h>

#define D0 22
#define D1 21
#define Start 19
#define Mode 2

const char *ssid = "ESP32";
const char *password = "A1b2c3d4#"; // Replace with your credentials

WebServer server(80);

void handleRoot();
void handleAuto();
void handleManual();
void handleForward();
void handleBackward();
void handleLeft();
void handleRight();
void handleStop();  // Separate stop handler

void setup() {
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(Start, OUTPUT);
    pinMode(Mode, OUTPUT);

    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");

    IPAddress IP = WiFi.localIP();
    Serial.print("ESP IP address: ");
    Serial.println(IP);

    server.on("/", handleRoot);
    server.on("/automatic", handleAuto);
    server.on("/manual", handleManual);
    server.on("/forward", handleForward);
    server.on("/backward", handleBackward);
    server.on("/left", handleLeft);
    server.on("/right", handleRight);
    server.on("/stop", handleStop); // Use the dedicated stop handler
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    String html = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Motor Control</title>
    <style>
        * {
            margin: 0;
            padding: 0;
        }

        body {
            min-height: 100vh;
            font-family: sans-serif;
            background-image: radial-gradient(circle farthest-corner at 10% 20%, rgba(234, 249, 249, 0.67) 0.1%, rgba(239, 249, 251, 0.63) 90.1%);
        }

        .container {
            display: flex;
            flex-direction: column;
            justify-content: center;
            width: 400px;
            margin: 100px auto 0 auto;
            background-image: linear-gradient(106.1deg, rgba(69, 242, 143, 0.52) 10.2%, rgba(14, 228, 175, 0.61) 83.6%);
            border-radius: 5px;
            padding: 30px;
        }

        /* ... (rest of your CSS styles) ... */
         .container .heading {
            width: 100%;
            text-align: center;
            margin-bottom: 30px;
        }

        .container .heading span {
            font-size: 30px;
            font-weight: bold;
        }

        .container .controllers {
            display: grid;
            grid-template-columns: repeat(5, 1fr);
            grid-template-rows: repeat(3, 1fr);
            grid-column-gap: 15px;
            grid-row-gap: 15px;
        }

        .container .controllers .common {
            text-align: center;
            justify-content: center;
            display: flex;
        }

        .container .controllers .common button {
            background-image: radial-gradient(circle 439px at 47.1% 47.8%, rgba(8, 208, 115, 1) 0%, rgba(0, 0, 0, 1) 91.3%);
            border: none;
            width: 70px;
            height: 70px;
            border-radius: 5px;
            cursor: pointer;
            transition: 0.30s ease-in-out;
        }

        .container .controllers .common button:hover {
            transform: scale(0.90);
        }

        .container .controllers .common button:active {
            background-image: red;
        }

        .container .controllers .common button span {
            font-size: 20px;
        }

        .container .controllers .controller_forward {
            grid-area: 1 / 3 / 2 / 4;
        }

        .container .controllers .controller_left {
            grid-area: 2 / 2 / 3 / 3;
            justify-content: right;
        }

        .container .controllers .controller_right {
            grid-area: 2 / 4 / 3 / 5;
            justify-content: left;
        }

        .container .controllers .controller_backward {
            grid-area: 3 / 3 / 4 / 4;
        }

        .container .controllers .controller_forward button:hover,
        .container .controllers .controller_left button:hover,
        .container .controllers .controller_right button:hover,
        .container .controllers .controller_backward button:hover {
            box-shadow: 0 0px 10px rgb(29, 107, 71);
        }

        .container .controllers .controller_stop {
            grid-area: 2 / 3 / 3 / 4;
        }

        .container .controllers .controller_stop button {
            background-image: radial-gradient(circle farthest-corner at 17.1% 22.8%, rgba(226, 24, 24, 1) 0%, rgba(160, 6, 6, 1) 90%);
        }

        .container .controllers .controller_stop button:hover {
            box-shadow: 0 0px 10px rgb(110, 21, 21);
        }

        .container .controllers .controller_stop button span {
            color: white;
        }

        .container .mode {
            display: flex;
            justify-content: center;
            margin-top: 50px;
        }

        .container .mode button {
            background-image: radial-gradient(circle farthest-corner at 10% 20%, rgba(97, 186, 255, 1) 0%, rgba(166, 239, 253, 1) 90.1%);
            border: none;
            width: 150px;
            height: 40px;
            border-radius: 5px;
            cursor: pointer;
            transition: 0.30s ease-in-out;
        }

        .container .mode button:hover {
            box-shadow: 0 5px 10px rgb(211, 211, 211);
            transform: scale(0.95);
        }

        .container .mode button:active {
            background-image: red;
        }

        .container .mode button span {
            font-size: 20px;
            text-transform: uppercase;
            font-weight: bold;
        }

        #autoManualButton.automatic {
            background-image: radial-gradient(circle farthest-corner at 10% 20%, rgba(37, 145, 251, 0.98) 0.1%, rgba(0, 7, 128, 1) 99.8%);
            color: white;
        }

        #autoManualButton.manual {
            background-image: radial-gradient(circle 489px at 49.3% 46.6%, rgba(255, 214, 126, 1) 0%, rgba(253, 200, 0, 1) 100.2%);
            color: black;
        }
    </style>
</head>

<body>
    <div class="container">
        <div class="heading">
            <span>Motor Control</span>
        </div>
        <div class="controllers">
            <div class="common controller_forward">
                <button class="arrow-button" ontouchstart="eventChecker('/forward', event)"
                    ontouchend="eventChecker('/stop', event)"> <span>↑</span>
                </button>
            </div>
            <div class="common controller_left">
                <button class="arrow-button" ontouchstart="eventChecker('/left', event)"
                    ontouchend="eventChecker('/stop', event)"><span>↶</span>
                </button>
            </div>
            <div class="common controller_stop">
                <button class="stop-button" onclick="sendCommand('/stop')"><span>Stop</span></button>
            </div>
            <div class="common controller_right">
                <button class="arrow-button" ontouchstart="eventChecker('/right', event)"
                    ontouchend="eventChecker('/stop', event)"><span>↷</span>
                </button>
            </div>
            <div class="common controller_backward">
                <button class="arrow-button" ontouchstart="eventChecker('/backward', event)"
                    ontouchend="eventChecker('/stop', event)"><span>↓</span>
                </button>
            </div>
        </div>
        <div class="mode">
            <button class="start-button" id="autoManualButton" onclick="handleAutoManual()">
                <span>Mode</span>
            </button>
        </div>
    </div>

    <script>
        let autoMode = true;
        let movementCommands = {
            '/forward': false,
            '/left': false,
            '/right': false,
            '/backward': false
        };

        async function sendCommand(command) {
            console.log(`Sending command: ${command}`);
            try {
                const response = await fetch(command);
                if (!response.ok) {
                    const errorText = await response.text();
                    console.error(`Server returned an error: ${response.status} - ${errorText}`);
                    const errorDiv = document.getElementById("error-message"); // Make sure you have this div
                    if (errorDiv) {
                        errorDiv.textContent = `Command failed: ${response.status}`;
                        setTimeout(() => errorDiv.textContent = "", 3000);
                    }
                    return;
                }
                console.log(`${command} command sent successfully`);
            } catch (error) {
                console.error(`Fetch request failed:`, error);
            }
        }

        function eventChecker(command, event) {
            if (event) {
                if (event.type === 'touchstart') {
                    sendCommand(command);
                    movementCommands[command] = true;
                }
                if (event.type === 'touchend') {
                    sendCommand('/stop');
                    for (let cmd in movementCommands) {
                        movementCommands[cmd] = false;
                    }
                }
            }
        }

        function handleAutoManual() {
            autoMode = !autoMode;
            const button = document.getElementById("autoManualButton");
            button.innerHTML = autoMode ? "<span>Manual</span>" : "<span>Auto</span>";
            button.classList.toggle("automatic", autoMode);
            button.classList.toggle("manual", !autoMode);
            sendCommand(autoMode ? '/automatic' : '/manual'); 
        }

        document.addEventListener('keydown', (event) => {
            let command = null;
            switch (event.key) {
                case 'w':
                case 'W':
                case 'ArrowUp':
                    command = '/forward';
                    break;
                case 'a':
                case 'A':
                case 'ArrowLeft':
                    command = '/left';
                    break;
                case 'd':
                case 'D':
                case 'ArrowRight':
                    command = '/right';
                    break;
                case 's':
                case 'S':
                case 'ArrowDown':
                    command = '/backward';
                    break;
            }
            
            if (event.key === 'm' || event.key === 'M') {
                handleAutoManual();
            }

            if (command && !movementCommands[command]) {
                movementCommands[command] = true;
                sendCommand(command);
            }
        });

        document.addEventListener('keyup', (event) => {
            let command = null;
            switch (event.key) {
                case 'w':
                case 'W':
                case 'ArrowUp':
                    command = '/forward';
                    break;
                case 'a':
                case 'A':
                case 'ArrowLeft':
                    command = '/left';
                    break;
                case 'd':
                case 'D':
                case 'ArrowRight':
                    command = '/right';
                    break;
                case 's':
                case 'S':
                case 'ArrowDown':
                    command = '/backward';
                    break;
            }
            if (command && movementCommands[command]) {
                movementCommands[command] = false;
                sendCommand('/stop');
            }
        });
    </script>
    <div id="error-message" style="color: red; margin-top: 10px;"></div>
    <div align="center"> If you ar in PC use <br> W,uparrow = foroward<br>S,Downarrow = backoward<br>A,leftparrow = Left Turn,<br>D,rightarrow = Right Turn<br>M = Mode Change</div>
    </body>

</html>
)=====";
    server.send(200, "text/html", html);
}

void handleAuto() {
    digitalWrite(Mode, HIGH);
    Serial.println("Manual Mode Enabled");
}

void handleManual() {
    digitalWrite(Mode, LOW);
    Serial.println("Auto Mode Enabled");
}

void handleForward() {
    digitalWrite(Start, HIGH); // Start signal
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    Serial.println("Forward");
}

void handleBackward() {
    digitalWrite(Start, HIGH); // Start signal
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    Serial.println("Backward");
}

void handleLeft() {
    digitalWrite(Start, HIGH); // Start signal
    digitalWrite(D0, HIGH);
    digitalWrite(D1, LOW);
    Serial.println("Left");
}

void handleRight() {
    digitalWrite(Start, HIGH);  // Start signal
    digitalWrite(D0, LOW);
    digitalWrite(D1, HIGH);
    Serial.println("Right");
}

void handleStop() {
    digitalWrite(Start, LOW); // Stop signal
    Serial.println("Stop");
}