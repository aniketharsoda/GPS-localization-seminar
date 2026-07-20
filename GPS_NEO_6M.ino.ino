#include <TinyGPS++.h>

// Create a GPS parser object from the TinyGPS++ library to decode raw satellite sentences.
TinyGPSPlus gps;

void setup() {
  // Initialize the main serial port for communication with the PC (Serial Monitor/Plotter).
  Serial.begin(9600);

  // Initialize Hardware Serial 1 (Pins 18 and 19) to receive incoming data from the NEO-6M module.
  Serial1.begin(9600);
}

void loop() {
  // Continuous check: Keep reading data as long as incoming bytes exist in the hardware buffer.
  while (Serial1.available() > 0) {
    
    // Feed raw data characters into the encoder. Returns true when a complete NMEA sentence is parsed.
    if (gps.encode(Serial1.read())) {
      displayParsedData(); // Trigger our custom function to display the processed data
    }
  }
}

void displayParsedData() {
  // Safety Gate: Only print data if the module has resolved clock bias and established a stable 3D fix.
  // This prevents the system from cluttering your screen with blank or "zero" data during startup.
  if (gps.location.isValid()) {
    
    // Note: The F() macro keeps static text strings inside Flash memory to save limited dynamic RAM (SRAM).
    // Commas are carefully printed between values to allow the Serial Plotter to split and graph multiple lines cleanly.
    
    Serial.print(F("Latitude:"));
    Serial.print(gps.location.lat(), 6); // Print latitude to 6 decimal places for precise high-resolution positioning.
    Serial.print(F(","));
    
    Serial.print(F(" Longitude:"));
    Serial.print(gps.location.lng(), 6); // Print longitude to 6 decimal places for precise high-resolution positioning.
    Serial.print(F(","));
    
    Serial.print(F(" Satellites:"));
    Serial.print(gps.satellites.value()); // Print active satellite count; a minimum of 4 is required for a 3D fix.
    Serial.print(F(","));
    
    Serial.print(F(" Altitude:"));
    Serial.println(gps.altitude.meters()); // Print altitude in meters; println terminates the data row packet cleanly.
  }
}