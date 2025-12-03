# Mobile-control-car-recever
This project is an ESP32-based Wi-Fi Motor Controller that allows a user to remotely control a device (likely a robot car or an omnidirectional platform) using a web browser interface.
The system acts as a Wi-Fi Access Point (AP), hosting a web server that provides a user-friendly control panel. Commands sent from the web interface trigger changes in the state of four GPIO pins on the ESP32, which control the device's movement and operating mode.
🤖 ESP32 Wi-Fi Robot Car Controller
Description
This code turns an ESP32 into a dedicated Wi-Fi Access Point (AP) and a Web Server, providing a remote control interface for a motor-driven device (like a small robot).

The control scheme uses four digital output pins (D0, D1, Start, Mode) on the ESP32 to send signals, likely to a motor driver shield (e.g., L298N or similar H-bridge circuit) to command movement and select between manual and automatic operation modes.

The web page is a sleek, responsive HTML interface with dedicated buttons for Forward, Backward, Left, Right, and Stop, along with a toggle button to switch between Automatic and Manual modes. The interface also supports control via keyboard shortcuts (W, A, S, D, and Arrow keys) for desktop users, and touch events for mobile devices, ensuring smooth, real-time control.
🏗️ Technical Details
1. Pin Assignments
Four digital I/O pins are used to communicate commands to the motor control hardware
Constant          GPIO Pin,       Function
D0,               22,             Data Pin 0 (Movement Control Signal)
D1,               21,             Data Pin 1 (Movement Control Signal)
Start,            19,             "Enable/Start Signal (HIGH for movement, LOW for stop)"
Mode,             2,              "Mode Select (e.g., HIGH for Manual, LOW for Auto)"

2. Control Logic
The system uses a specific combination of HIGH/LOW states on the D0 and D1 pins, in conjunction with the Start pin being HIGH, to define the movement.
Command,Start (GPIO 19),D0 (GPIO 22),D1 (GPIO 21)
Stop,LOW,(Ignored),(Ignored)
Forward,HIGH,LOW,LOW
Backward,HIGH,HIGH,HIGH
Left,HIGH,HIGH,LOW
Right,HIGH,LOW,HIGH
Manual Mode,(N/A),(N/A),(N/A)
Auto Mode,(N/A),(N/A),(N/A)

3. Wi-Fi & Web Server
Mode: The ESP32 is configured to act as a Soft Access Point (AP) with the SSID "ESP32" and password "A1b2c3d4#".

Access: Users connect their computer or phone directly to the ESP32's Wi-Fi network and navigate to the assigned IP address (printed to the Serial Monitor during setup).

Routing: Each control action is mapped to a specific HTTP endpoint:
URL Endpoint,Function Handler,Action on Robot
/,handleRoot(),Serves the HTML control page.
/automatic,handleAuto(),Sets Mode pin HIGH (Manual Mode in HTML logic).
/manual,handleManual(),Sets Mode pin LOW (Auto Mode in HTML logic).
"/forward, /backward, etc.",handleX(),Sets D0/D1/Start pins to move the robot.
/stop,handleStop(),Sets Start pin LOW to halt all movement.
4. HTML & JavaScript Interface

The embedded HTML provides a dynamic controller with the following client-side features:

Responsive Design (CSS Grid): The control buttons are arranged in a classic 5-button diamond layout (Forward, Backward, Left, Right, Center Stop) using a grid system, optimized for mobile viewing.

Touch Control: Uses ontouchstart and ontouchend events on the direction buttons to send the movement command immediately, followed by the /stop command when the finger is lifted, providing momentary control.

Keyboard Control: Supports W/A/S/D and Arrow Keys for movement control and M to toggle the Mode button. The keyup event automatically sends a /stop command, mimicking the momentary behavior of the touch controls.

Mode Toggle: The "Mode" button dynamically changes its text, style, and sends either the /automatic or /manual command to the ESP32.

🚀 Setup Instructions
Hardware Connection: Connect the specified ESP32 GPIO pins (22, 21, 19, 2) to your motor driver circuit's control inputs (e.g., IN1, IN2, ENA for a two-motor system).

Code Upload: Upload the provided code to your ESP32 board.

Wi-Fi Connection:

On your computer or mobile device, search for the Wi-Fi network with the name "ESP32".

Connect using the password "A1b2c3d4#".

Control: Open a web browser and navigate to the IP address displayed on the ESP32's Serial Monitor (e.g., 192.168.4.1). You can now control the motors using the on-screen buttons or the keyboard shortcuts.

Would you like me to suggest the typical wiring schematic for the ESP32 and an L298N motor driver based on these pins?
