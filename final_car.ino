
#include <SoftwareSerial.h>
#include <Servo.h>

// Create a software serial object for the bluetooth module
SoftwareSerial bluetooth(3, 4); // RX, TX
// Define the pins for the motors, LEDs, buzzer, and ultrasonic sensor
const int MLa = 8;     // left motor 1st pin
const int MLb = 9;     // left motor 2nd pin
const int MRa = 10;    // right motor 1st pin
const int MRb = 11;    // right motor 2nd pin
const int servoPin = 5; // The pin for the servo monter
const int LEDR = 6;    // The pin the REAR LED is connected to
const int BUZ = 2;     // The pin the BUZZER is connected to
const int LEDF = 7;    // The pin the FRONT LED is connected to
const int echopin = 13;
const int trigpin = 12;

// Create a servo object for the servo motor
Servo myservo; 

void setup() {
  // Attach the servo to the servo pin
  myservo.attach(servoPin);

  // Set the baud rate for the serial communication 
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Set the motor pins as outputs
  pinMode(MLa, OUTPUT);   // left motors forward
  pinMode(MLb, OUTPUT);   // left motors reverse
  pinMode(MRa, OUTPUT);   // right motors forward
  pinMode(MRb, OUTPUT);   // right motors reverse

  // Set the LED and buzzer pins as outputs
  pinMode(LEDF, OUTPUT); // Declare the FRONT LED as an output
  pinMode(LEDR, OUTPUT); // Declare the REAR LED as an output
  pinMode(BUZ, OUTPUT);  // Declare the BUZZER as an output

  // Set the ultrasonic sensor pins as input and output
  pinMode(echopin, INPUT);
  pinMode(trigpin, OUTPUT);
}

void loop() {
  // Check if there is any data available on the bluetooth module
  if (bluetooth.available() > 0) {
    // Read the data from the bluetooth module
    char t = bluetooth.read();

    // Print the data to the serial monitor
    Serial.println(t);

    // Perform the appropriate action based on the received data
    switch (t) {
      case 'F':
        // Move forward (all motors rotate in forward direction)
        digitalWrite(LEDF, HIGH); // Turn on the LED
        digitalWrite(MLa, HIGH);
        digitalWrite(MLb, LOW);
        digitalWrite(MRa, HIGH);
        digitalWrite(MRb, LOW);
        long duration = measureDistance();
        int distance = duration * 0.034 / 2;

        // Print the distance on the Serial Monitor
        Serial.print("Distance: ");
        Serial.println(distance);

        // Wait for 1 second
      
        break;
      case 'B':
        // Move reverse (all motors rotate in reverse direction)
        digitalWrite(LEDR, HIGH);
        digitalWrite(MLa, LOW);
        digitalWrite(MLb, HIGH);
        digitalWrite(MRa, LOW);
        digitalWrite(MRb, HIGH);
        duration = measureDistance();
        distance = duration * 0.034 / 2;

        // Print the distance on the Serial Monitor
        Serial.print("Distance: ");
        Serial.println(distance);

        // Wait for 1 second
        
        break;
      case 'R':
        // Turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
        digitalWrite(MLa, LOW);
        digitalWrite(MLb, LOW);
        digitalWrite(MRa, HIGH);
        digitalWrite(MRb, LOW);
        break;
      case 'L':
        // Turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
        digitalWrite(MLa, HIGH);
        digitalWrite(MLb, LOW);
        digitalWrite(MRa, LOW);
        digitalWrite(MRb, LOW);
        break;
      case 'V':
        // LED ON
        digitalWrite(BUZ, HIGH);
        break;
      case 'v':
        // LED OFF
        digitalWrite(BUZ, LOW);
        break;
      case 'W':
        // LED ON
        digitalWrite(LEDF, HIGH);
        break;
      case 'w':
        // LED OFF
        digitalWrite(LEDF, LOW);
        break;
      case 'U':
        // LED ON
        digitalWrite(LEDR, HIGH);
        break;
      case 'u':
        // LED OFF
        digitalWrite(LEDR, LOW);
        break;
      case 'S':
        // STOP (all motors stop)
        digitalWrite(MLa, LOW);
        digitalWrite(MLb, LOW);
        digitalWrite(MRa, LOW);
        digitalWrite(MRb, LOW);

        // Move the servo motor to the middle position
        myservo.write(90);

        // Wait for 1 second
        

        // Measure the distance in front of the car
        duration = measureDistance();
        distance = duration * 0.034 / 2;

        // Print the distance on the Serial Monitor
        Serial.print("Distance: ");
        Serial.println(distance);

        // If the distance is less than 5 cm, make a sound
        if (distance < 5) {
          tone(BUZ, 1047, 100);
          delay(100);
        } else if (distance < 10) {
          tone(BUZ, 523, 100);
          delay(300);
        } else if (distance < 15) {
          tone(BUZ, 262, 100);
          delay(500);
        }
        break;
    }
  }
}

long measureDistance() {
  // Clear the trigPin
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);

  // Set the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echopin, HIGH);

  return duration;
}

