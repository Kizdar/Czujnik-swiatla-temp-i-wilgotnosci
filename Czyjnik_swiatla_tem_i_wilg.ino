#include "DHT.h"
#include <OneWire.h>
#include <DS18B20.h>
#include <SPI.h>                                     
#include <SD.h>                                     
#define DHTPIN 2
#define DHTTYPE DHT11
#define prezystor A0
#define ONEWIRE_PIN 2
#define led 3
File plik;

// Adres czujnika
byte address[8] = {0x28, 0xFF, 0x15, 0x35, 0x31, 0x18, 0x2, 0x7C};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

DHT dht(DHTPIN, DHTTYPE);
  float temperature=0;
float pomiary_temp[25];
float wilgotnosc[25];
int slonce[25];
int nr_pomiaru=0;

void setup() {
Serial.begin(9600);
pinMode(led,OUTPUT);
digitalWrite(led,LOW);
Serial.println("Cekaj czy karta zostanie wykryta…");
if (!SD.begin(4))                                       //sprawdź czy nie ma karty na pinie ChipSelect 4
  {
     Serial.println("Nie wykryto karty(ERR)");            //błąd wykrycia karty
     digitalWrite(led,HIGH);
     return;                                              //przerwij program
  }  else
Serial.println("Karta została wykryta");
plik=SD.open("wykresy.txt",FILE_WRITE);
if(!plik)
Serial.println("Nie udało się otworzyć pliku");
digitalWrite(led,LOW);
pinMode(prezystor,INPUT);
dht.begin();
sensors.begin();                                           //Inicjalizacja czujnikow
sensors.request(address);

for(int i=0;i>=24;i++)                                    //zerowanie tablic
{
pomiary_temp[i]=-120;
wilgotnosc [i]=-120;
slonce [i]=-120;
}
}

void loop() {
  float h = 0;
  float t = 0;
if(millis()%1440==0)
{
if (sensors.available())
  {
     temperature = sensors.readTemperature(address);
  }
     h = dht.readHumidity();
     t = dht.readTemperature();
     sensors.request(address);
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float Stemperatura=(t+temperature)/2;
int pr= analogRead(prezystor);
pomiary_temp[nr_pomiaru]=Stemperatura;
wilgotnosc[nr_pomiaru]=h;
slonce[nr_pomiaru]=pr;
nr_pomiaru++;


if(nr_pomiaru==25){
  digitalWrite(led,HIGH);
plik.println("Temperatura:");
for(int a=0;a<25;a++){
Serial.println(pomiary_temp[a]);
plik.println(pomiary_temp[a]);}
plik.println(" Wilgotnosc:");
for(int b=0;b<25;b++){
  Serial.println(wilgotnosc[b]);
plik.print(" ");
plik.println(wilgotnosc[b]);}
plik.println("/ Naslonecznienie:");
for(int c=0;c<25;c++){
  Serial.println(slonce[c]);
  plik.print("/ ");
plik.println(slonce[c]);}

plik.flush();
Serial.println("Plik zostal zapisany");
nr_pomiaru=0;
for(int i=0;i>=24;i++)                                      //zerowanie tablic
{
pomiary_temp[i]=-120;
wilgotnosc [i]=-120;
slonce [i]=-120;
}
digitalWrite(led,LOW);
}
 

//Serial.print("Temp ");
//Serial.print(t);
//Serial.print(" temperatura2 ");
//Serial.print(temperature);  //Wyswietlenie informacji
//Serial.print(" Temperatur atablica ");
//Serial.print(pomiary_temp[nr_pomiaru-1]);
//Serial.print(" Sr_temp ");
//Serial.print(Stemperatura);
//Serial.print(" Wilgotnosc ");
//Serial.print(h);
//Serial.print(" Swiatlo ");
//Serial.print(pr);
//Serial.print(" nr pomiaru ");
//Serial.println(nr_pomiaru);

}
}
