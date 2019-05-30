#include <ESP8266WiFi.h>

// include the SD library:
#include <SPI.h>
#include <SD.h>
#define BUTTON_BUILTIN 0 

const int chipSelect = D2;
unsigned long times;
String cabecera = "TIME;SSDI;RSSI;TOMA";
File dataFile;
unsigned long contador = 0;
int flag = 0;
unsigned long current_time=0;

void setup()
{ 
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_BUILTIN, INPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");
  dataFile = SD.open("log_file.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(cabecera);
    dataFile.close();
    // print to the serial port too:
    Serial.println(cabecera);
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening log_file.txt");
  }
}

void loop()
{
  int estado = digitalRead(BUTTON_BUILTIN);

  if( estado == LOW){
    flag = 1;
    Serial.print("Entro!!");
    current_time = millis();
  }
  if( flag == 1){
  
  Serial.print("Time: ");
  times = (millis()- current_time) / 1000;
  Serial.print(times);

  int n = WiFi.scanNetworks();
  String csv = "";

  if (n == 0)
  {
    Serial.println("No networks found");
    csv += "No networks found";
  }
  else
  {

    for (int i = 0; i < n; ++i)
    {
      csv += String(times) + ";";
      csv += WiFi.SSID(i) + ";" + String(WiFi.RSSI(i)) + ";" + String(contador) +"\n";
    }
    contador++;
  }

  String dataString = csv;
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("log_file.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.print(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);

  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening log_file.txt");
  }
        digitalWrite(LED_BUILTIN, LOW); 
      delay(500);
      digitalWrite(LED_BUILTIN, HIGH);
  delay(9500);
  }
}
