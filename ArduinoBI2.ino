#include <DHT.h>
#include <Adafruit_MAX31856.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Constants
#define DHTPIN 2 
#define DHTTYPE DHT22 
#define DRDY_PIN 5
#define xPin A3
#define yPin A4
#define zPin A5
#define MAX31856_CS_PIN 53
#define MAX31856_SCK_PIN 52
#define MAX31856_MISO_PIN 50
#define MAX31856_MOSI_PIN 51
#define DETECT 3

const int currentZ = A10;
const int currentFilament = A11;
const int currentX = A12;
const int currentY = A13;       
const float VCC = 5.0;     
const float sensitivity = 0.185;  // ACS712-5A: 0.185V/A, ACS712-20A: 0.100V/A, ACS712-30A: 0.066V/A
const float offset = VCC / 2;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(53);

int chk;
float hum; 
float temp; 
int i;
int xMin = 269;
int xMax = 406;
int yMin = 268;
int yMax = 404;
int zMin = 274;
int zMax = 406;
const int samples = 5;

// --- Print Tracking Variables & Thresholds ---
bool isPrinting = false;
unsigned long printStartTime = 0;

// Adjust these thresholds based on your machine's baseline noise
const float TEMP_THRESHOLD_NOZZLE = 30.0; // °C
const float TEMP_THRESHOLD_BED = 30.0;    // °C
const float CURRENT_THRESHOLD = 0.4;      // Amps (Absolute value)
const float ACCEL_THRESHOLD = 0.7;        // g (Deviation from baseline)

void setup()
{
  Serial.begin(9600);
  while (!Serial) delay(10);
  pinMode(DRDY_PIN, INPUT);
  dht.begin();
  i=1;
  if (!maxthermo.begin()) {
    Serial.println("MAX31856 not found. Check wiring!");
    while (1) delay(10);
  }
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);
  maxthermo.setConversionMode(MAX31856_CONTINUOUS);
  mlx.begin();
  pinMode(DETECT, INPUT); 
}

void loop()
{
  int xRaw=0,yRaw=0,zRaw=0;
  for(int i=0;i<samples;i++)
  {
    xRaw+=analogRead(xPin);
    yRaw+=analogRead(yPin);
    zRaw+=analogRead(zPin);
  }
  xRaw/=samples;
  yRaw/=samples;
  zRaw/=samples;
  
  long xMilliG = map(xRaw, xMin, xMax, -1024, 1024);
  long yMilliG = map(yRaw, yMin, yMax, -1024, 1024);
  long zMilliG = map(zRaw, zMin, zMax, -1024, 1024);
  float x_g_value = xMilliG / 1000.0;
  float y_g_value = yMilliG / 1000.0;
  float z_g_value = zMilliG / 1000.0;

  hum = dht.readHumidity();
  float ambtemp= dht.readTemperature();

  float bedtemp = mlx.readObjectTempC();

  int intcurrentZ = analogRead(currentZ);
  float voltageZ = (intcurrentZ/1023.0)*(5000);  // Convert to voltage
  float ampZ = (voltageZ - 2550) / 185;

  int intcurrentFilament = analogRead(currentFilament);
  float voltageFilament = (intcurrentFilament/1023.0)*(5000);  // Convert to voltage
  float ampFilament = (voltageFilament - 2550) / 185;

  int intcurrentX = analogRead(currentX);
  float voltageX = (intcurrentX/1023.0)*(5000);  // Convert to voltage
  float ampX = (voltageX - 2550) / 185;

  int intcurrentY = analogRead(currentY);
  float voltageY = (intcurrentY/1023.0)*(5000);  // Convert to voltage
  float ampY = (voltageY - 2550) / 185;

  double rawThermocoupleTemp = maxthermo.readThermocoupleTemperature();
  float adjustedNozzleTemp = rawThermocoupleTemp * 1.111 - 5.06;
  bool filament = !digitalRead(DETECT);

  // --- Calculate Baseline Deviations for Acceleration ---
  float z_delta = abs((z_g_value - 1.13));
  float x_delta = abs((x_g_value - 1.08));
  float y_delta = abs((y_g_value - 0.19));

  // --- Print Detection Logic ---
  bool heatingDetected = (adjustedNozzleTemp > TEMP_THRESHOLD_NOZZLE) || (bedtemp > TEMP_THRESHOLD_BED);
  bool motionDetected = (x_delta > ACCEL_THRESHOLD) || (y_delta > ACCEL_THRESHOLD) || (z_delta > ACCEL_THRESHOLD);

  if (!isPrinting) {
    if (heatingDetected ) {
      isPrinting = true;
      printStartTime = millis();
    }
  } else {
    // Optional Reset Condition: If the machine cools down completely, reset the timer.
    // This allows sequential prints without resetting the Arduino board.
    if (!heatingDetected && adjustedNozzleTemp < 35.0) {
      isPrinting = false;
    }
  }

  // Calculate elapsed time
  float printTimeMinutes = 0.0;
  if (isPrinting) {
    printTimeMinutes = (millis() - printStartTime) / 60000.0;
  }

  // --- Serial Output ---
  Serial.print(printTimeMinutes, 2); // Time in minutes (0.00 if idle)
  Serial.print(",");
  Serial.print(adjustedNozzleTemp, 2);
  Serial.print(",");
  Serial.print(bedtemp, 2); // Corrected to print, not println
  Serial.print(",");
  Serial.print(z_delta);
  Serial.print(",");  
  Serial.print(x_delta);
  Serial.print(",");
  Serial.print(y_delta);
  Serial.print(",");
  Serial.print(ambtemp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(abs(ampX-1), 2);
  Serial.print(",");
  Serial.print(abs(ampY+0.4), 2);
  Serial.print(",");
  Serial.print(abs(ampZ-1), 2);
  Serial.print(",");
  Serial.print(abs(ampFilament-1.3), 2);
  Serial.print(",");
  Serial.println(filament);
  
  delay(100);
}