#include "DHT.h" 
#include <SoftwareSerial.h>
#define DHTPIN 8    
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);


String str;
int analogInPin = A0;        
int sensorValue = 0; 
int VoltPin = A1; 
float Vmodul = 0.0; 
float hasil = 0.0;
float R1 = 30000.0; //30k
float R2 = 7500.0; //7500 ohm resistor, 
int value = 0;



void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(analogInPin, INPUT);
  pinMode(VoltPin, INPUT);
 
}

void loop() {
 
  sensorValue = analogRead(analogInPin);
  
  //Pembacaan untuk data kelembaban
  float humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  float celcius_1 = dht.readTemperature();
  //pembacaan dalam format Fahrenheit
  float fahrenheit_1 = dht.readTemperature(true);


  value = analogRead(VoltPin);
  Vmodul = (value * 5.0) / 1024.0;
  hasil = Vmodul / (R2/(R1+R2));
 
/*
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print(" ");
  //pembacaan nilai pembacaan data suhu
  Serial.print("Suhu : ");
  Serial.print(celcius_1 ); //format derajat celcius
  Serial.print("°"); //simbol derajat
  Serial.print("C / ");
  Serial.print(fahrenheit_1); //format derajat fahrenheit
  Serial.print("°");
  Serial.println("F\t");
  Serial.print("Tegangan keluaran modul = ");
  Serial.print(Vmodul,2);
  Serial.print("volt");
  Serial.print(", Hasil pengukuran = ");
  Serial.print(hasil,2);
  Serial.println("volt");
 */
  str =String("*")+String("se")+String(",")+String(sensorValue/10)+String(",")+String(celcius_1)+String(",")+String(hasil)+String("#");
  Serial.println(str);

 delay(1000);
  
 
}
