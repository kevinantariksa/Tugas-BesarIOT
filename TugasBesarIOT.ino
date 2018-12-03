#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <L3G.h>
#define BLYNK_PRINT Serial
L3G gyro;
 
 /////DIPERSIAPKAN OLEH KEVIN ANTARIKSA DAN DINO ALESANDRO/////
  // lowest and highest sensor readings:
  const int sensorMin = 0;     // sensor minimum
  const int sensorMax = 1024;  // sensor maximum
  int LED = 2; //Opsional, untuk menampilkan feedback
  int isFlamePin = 13;  // input untuk flame pada pin D7
  int isFlame = HIGH;  // HIGH MEANS NO FLAME

  //WIFI SETUP AND API
  char auth[] = "54d641c6a62c468a9b0fdca57d2f7489"; //untuk API
  char ssid[] = "luwak kofi"; //nama Wifi
  char pass[] = "qwer1234"; //password Wifi
 
  void setup() {
    //digunakan untuk setup sensor-sensor
    Serial.begin(9600); 

    Wire.begin();
 
    if (!gyro.init())
    {
      Serial.println("Failed to autodetect gyro type!");
      while (1);
    }
 
    gyro.enableDefault();
    pinMode(12,OUTPUT);//deklarasi pin no 12 berfungsi sebagai outputpinMode(12,OUTPUT);//deklarasi pin no 12 berfungsi sebagai output 
    pinMode(LED, OUTPUT);
    pinMode(isFlamePin, INPUT);
    Serial.begin(9600);

    //digunakan untuk inisialisasi Wifi dan blynk
    WiFi.begin(ssid, pass);
    Blynk.config(auth);
    if (WiFi.status() == WL_CONNECTED) {
      Blynk.connect(5);
    }
   }


  
  void loop() {
    //berjalan berulang-ulang, memanggil prosedur yang menerima input dari sensor
    flame();  
    gas();
    gempa();
    Blynk.run();
  }
  
  void flame()
  {
    //deteksi pada sensor api
    isFlame = digitalRead(isFlamePin);
    if (isFlame== LOW)
    {
      Serial.println("FLAME, FLAME, FLAME");
    Blynk.notify("[SmartHome] - FIRE!! Check Your Room!!");
      digitalWrite(LED, HIGH);
    }
    else
    {
      Serial.println("no flame");
      digitalWrite(LED, LOW);
    }
    delay(10);
  }
  
  void gas()
  {
    //deteksi pada sensor gas
    int sensorReading = analogRead(A0);
    float sensorValue; //membuat tampungan data tipe float bernama sensorValue
   
   sensorValue = analogRead(A0); //meng-inputkan pembacaan fungsi analogRead pada pin A0 ke sensorValue
    Serial.print("sensor value = "); //menampilkan data "sensor value = " pada serial monitor
   Serial.println(sensorValue); //menampilkan data sensorValue pada serial monitor
   if(sensorValue>700) //apabila nilai sensorValue lebih dari 850
   {
    digitalWrite(12,HIGH);
    Blynk.notify("[SmartHome] - GAS LEAKAGE!! Check Your Kitchen!!");
   } //pin no 12 di buat HIGH atau di beri tegangan 5V
   if(sensorValue<700) //apabila nilai sensorValue kurang dari 849
   {
    digitalWrite(12,LOW);
    //Blynk.notify("[SmartHome] - FIRE!! Check Your Room!!");
   } //pin no 12 di buat LOW atau di beri tegangan 0V
   delay(1000); //di beri jeda 1 detik
  }

  void gempa()
  {
    //menampilkan bacaan dari sensor gempa
   gyro.read();
 
  Serial.print("G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);
    if((int)gyro.g.x < -300 ||(int)gyro.g.x > 300 )
  {
    Serial.print("Gempa");
     Blynk.notify("[SmartHome] - EARTHQUAKE!! Evacuate!!");
  }   
  else if((int)gyro.g.y < -300 ||(int)gyro.g.y > 300)
  {
    Serial.print("Gempa");  
     Blynk.notify("[SmartHome] - EARTHQUAKE!! Evacuate!!");
  }
 
  delay(1000); 
  }
