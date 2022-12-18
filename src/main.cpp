#include <Arduino.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <RTClib.h>

#define C1 1
#define C2 3
#define C3 13
#define C4 15

RTC_DS3231 rtc;

String ssid     = "Lenovo";
String password = "burbano99";
#define FIREBASE_HOST "ejemploesp8266-533d5-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "sy1Ldul8ayYSW8NBNoLqutnmMcYTAsJr8ZlUXO9G"

String path = "/users/bei26NtS0lbsqdX4fqsWDLJbLUx1/";

//FIREBASE/*
FirebaseData firebaseData;                                      // Objeto de tipo FirebaseData, ayuda a leer y escribri en la base de datos.
FirebaseJson canalesData;
FirebaseJsonData jsData;
String jsStr;

byte cont = 0;
byte max_intentos = 50;

void connectionWifi(void);
void connectionFirebase(void);
void causaError(void);
void timedActuators(byte PIN);
void checkTimer(byte channelPin);

void setup() {
  // Inicia Serial
  Serial.begin(9600);
  pinMode(C1,OUTPUT);
  pinMode(C2,OUTPUT);
  pinMode(C3,OUTPUT);
  pinMode(C4,OUTPUT);
  pinMode(2,OUTPUT);

  digitalWrite(C1,HIGH);
  digitalWrite(C2,HIGH);
  digitalWrite(C3,HIGH);
  digitalWrite(C4,HIGH);

   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  //rtc.adjust(DateTime(2000+ano, mes, dia, hora, minuto, segundo));
  //rtc.adjust(DateTime(2022, 12, 17, 18, 56, 0));
  connectionWifi();
  connectionFirebase();

}

void loop() {

  /*
  if(Firebase.get(firebaseData,path+"/channels/channel1")){
      //digitalWrite(2,HIGH);
      jsStr = firebaseData.jsonString();
      canalesData.setJsonData(jsStr);

      canalesData.get(jsData,"/estado");
      bool canal1 = jsData.boolValue;
      //Serial.print("Canal 2: "); Serial.println(canal2);

      digitalWrite(C1,!canal1);

  }else{causaError();}

    if(Firebase.get(firebaseData,"/users/bei26NtS0lbsqdX4fqsWDLJbLUx1/channels/channel2")){
      //digitalWrite(2,HIGH);
      jsStr = firebaseData.jsonString();
      canalesData.setJsonData(jsStr);

      canalesData.get(jsData,"/estado");
      bool canal2 = jsData.boolValue;
      Serial.print("Canal 2: "); Serial.println(canal2);

      digitalWrite(2,canal2);
      digitalWrite(C2,canal2);

  }else{causaError();}

    if(Firebase.get(firebaseData,"/users/bei26NtS0lbsqdX4fqsWDLJbLUx1/channels/channel3")){
      //digitalWrite(2,HIGH);
      jsStr = firebaseData.jsonString();
      canalesData.setJsonData(jsStr);

      canalesData.get(jsData,"/estado");
      bool canal3 = jsData.boolValue;
      Serial.print("Canal 3: "); Serial.println(canal3);

      digitalWrite(C3,canal3);

  }else{causaError();}
*/
  timedActuators(C1); 
  delay(60);
  timedActuators(C2);
}

void connectionWifi(){
    Serial.println("\n");

    // Conexión WIFI
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED and cont < max_intentos) { //Cuenta hasta 50 si no se puede conectar lo cancela
      cont++;
      delay(500);
      Serial.print(".");
    }

    Serial.println("");

    if (cont < max_intentos) {  //Si se conectó  
        digitalWrite(2,HIGH);
        Serial.println("********************************************");
        Serial.print("Conectado a la red WiFi: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("macAdress: ");
        Serial.println(WiFi.macAddress());
        Serial.println("*********************************************");
    }
    else { //No se conectó
        digitalWrite(2,LOW);
        Serial.println("------------------------------------");
        Serial.println("Error de conexion");
        Serial.println("------------------------------------");
    }

}

void connectionFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

    //Establezca el tiempo de espera de lectura de la base de datos en 1 minuto (máximo 15 minutos)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  
  //Tamaño y  tiempo de espera de escritura, tiny (1s), small (10s), medium (30s) and large (60s).
  //tiny, small, medium, large and unlimited.
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void timedActuators(byte PIN) {
  //limiteMax=29;
  //limiteMin=23;
  DateTime fecha = rtc.now();

  uint8_t i=0;
  uint8_t h_on_rele1  = 6;
  uint8_t h_off_rele1 =12;

  uint8_t h_on_rele2  = 6;
  uint8_t h_off_rele2 =12;

  uint8_t h_on_rele3  = 6;
  uint8_t h_off_rele3 =12;

  uint8_t h_on_rele4  = 6;
  uint8_t h_off_rele4 =12;

  byte m_on = 0;
  byte m_off = 0;

  bool activar=false;
  //int tem = dht.readTemperature();  // Leemos la temperatura en grados centígrados (por defecto)
  switch(PIN){
    case C1:{

//      Serial.print(fecha.hour(), DEC);
//      Serial.print(':');
//      Serial.println(fecha.minute(), DEC);

      //checkTimer(C1);
   
      if(Firebase.getBool(firebaseData, path + "/channels/channel1/activar")){
        activar=firebaseData.boolData();
        //Serial.print("Activar:"); Serial.println(activar);
      } else {causaError();}
      
      if(activar){
        if (Firebase.getInt(firebaseData, path + "/channels/channel1/h_on_rele1")){
          h_on_rele1=firebaseData.intData();
          //Serial.print("h_on_rele1: "); Serial.println(h_on_rele1);
        }else{causaError();}
  
        if(Firebase.getInt(firebaseData, path + "/channels/channel1/m_on_rele1")){
          m_on=firebaseData.intData();
          //Serial.print("m_on:"); Serial.println(m_on);
        } else {causaError();}
  
        if (Firebase.getInt(firebaseData, path + "/channels/channel1/h_off_rele1")){
          h_off_rele1=firebaseData.intData();
          //Serial.print("h_off_rele1: ");Serial.println(h_off_rele1);
        }else{causaError();}
  
        if(Firebase.getInt(firebaseData, path + "/channels/channel1/m_off_rele1")){
          m_off=firebaseData.intData();
          //Serial.print("m_off:"); Serial.println(m_off);
        } else {causaError();}  

            
        if(h_off_rele1==h_on_rele1){
          //Serial.println("hof=hon");
          if(fecha.minute() >= m_on && fecha.minute() < m_off)  i=1;
          else                                                  i=0;
        }      
        else if(h_off_rele1 > h_on_rele1){ 
          //i=0;
          //Serial.println("hof>hon");
          //Serial.print("-->"); Serial.print("i=");Serial.println(i);
          if(h_on_rele1 == fecha.hour() && fecha.minute() >= m_on)          i=1;
          else if(fecha.hour()>h_on_rele1 && fecha.hour() < h_off_rele1)   i=1;//digitalWrite(PINLUZ, HIGH);
          else if(fecha.hour() == h_off_rele1 && fecha.minute()< m_off)     i=1;   
          else                                                              i=0;  //digitalWrite(PINLUZ,LOW);
        }
        else if(h_off_rele1 < h_on_rele1){
          //Serial.println("hof<hon");
          if(h_on_rele1 == fecha.hour() && fecha.minute() >= m_on)        i=1;
          else if(fecha.hour() > h_on_rele1 && fecha.hour() <=23)         i=1;  //digitalWrite(PINLUZ, HIGH); 
          else if(fecha.hour() >=0 && fecha.hour() < h_off_rele1)         i=1;  //digitalWrite(PINLUZ, HIGH); 
          else if(fecha.hour() == h_off_rele1 && fecha.minute()< m_off)   i=1; //digitalWrite(PINLUZ, HIGH); 
          else                                                            i=0;
        }
        
        if(i){

          digitalWrite(C1, LOW);
          if (Firebase.setBool(firebaseData, path + "/channels/channel1/estado", true)){/*InforSetLuzSensor();*/}else{causaError();}
        

        }
        else{
          digitalWrite(C1, HIGH);
          if (Firebase.setBool(firebaseData, path + "/channels/channel1/estado", false)){/*InforSetLuzSensor();*/}else{causaError();}
          
        }
      } else {
          bool estado = false;
          if (Firebase.getBool(firebaseData, path + "/channels/channel1/estado")){
            estado=firebaseData.boolData();
          }else{causaError();}

          digitalWrite(C1, !estado);
      }
    break;}
    case C2:{

//      Serial.print(fecha.hour(), DEC);
//      Serial.print(':');
//      Serial.println(fecha.minute(), DEC);

      //checkTimer(C1);
   
      if(Firebase.getBool(firebaseData, path + "/channels/channel2/activar")){
        activar=firebaseData.boolData();
        //Serial.print("Activar:"); Serial.println(activar);
      } else {causaError();}
      
      if(activar){
        if (Firebase.getInt(firebaseData, path + "/channels/channel2/h_on_rele2")){
          h_on_rele1=firebaseData.intData();
          //Serial.print("h_on_rele1: "); Serial.println(h_on_rele1);
        }else{causaError();}
  
        if(Firebase.getInt(firebaseData, path + "/channels/channel2/m_on_rele2")){
          m_on=firebaseData.intData();
          //Serial.print("m_on:"); Serial.println(m_on);
        } else {causaError();}
  
        if (Firebase.getInt(firebaseData, path + "/channels/channel2/h_off_rele2")){
          h_off_rele1=firebaseData.intData();
          //Serial.print("h_off_rele1: ");Serial.println(h_off_rele1);
        }else{causaError();}
  
        if(Firebase.getInt(firebaseData, path + "/channels/channel2/m_off_rele2")){
          m_off=firebaseData.intData();
          //Serial.print("m_off:"); Serial.println(m_off);
        } else {causaError();}  

            
        if(h_off_rele1==h_on_rele1){
          //Serial.println("hof=hon");
          if(fecha.minute() >= m_on && fecha.minute() < m_off)  i=1;
          else                                                  i=0;
        }      
        else if(h_off_rele1 > h_on_rele1){ 
          //i=0;
          //Serial.println("hof>hon");
          //Serial.print("-->"); Serial.print("i=");Serial.println(i);
          if(h_on_rele1 == fecha.hour() && fecha.minute() >= m_on)          i=1;
          else if(fecha.hour()>h_on_rele1 && fecha.hour() < h_off_rele1)   i=1;//digitalWrite(PINLUZ, HIGH);
          else if(fecha.hour() == h_off_rele1 && fecha.minute()< m_off)     i=1;   
          else                                                              i=0;  //digitalWrite(PINLUZ,LOW);
        }
        else if(h_off_rele1 < h_on_rele1){
          //Serial.println("hof<hon");
          if(h_on_rele1 == fecha.hour() && fecha.minute() >= m_on)        i=1;
          else if(fecha.hour() > h_on_rele1 && fecha.hour() <=23)         i=1;  //digitalWrite(PINLUZ, HIGH); 
          else if(fecha.hour() >=0 && fecha.hour() < h_off_rele1)         i=1;  //digitalWrite(PINLUZ, HIGH); 
          else if(fecha.hour() == h_off_rele1 && fecha.minute()< m_off)   i=1; //digitalWrite(PINLUZ, HIGH); 
          else                                                            i=0;
        }
        
        if(i){

          digitalWrite(C2, LOW);
          if (Firebase.setBool(firebaseData, path + "/channels/channel2/estado", true)){/*InforSetLuzSensor();*/}else{causaError();}
        

        }
        else{
          digitalWrite(C2, HIGH);
          if (Firebase.setBool(firebaseData, path + "/channels/channel2/estado", false)){/*InforSetLuzSensor();*/}else{causaError();}
          
        }
      } else {
          bool estado = false;
          if (Firebase.getBool(firebaseData, path + "/channels/channel2/estado")){
            estado=firebaseData.boolData();
          }else{causaError();}

          digitalWrite(C2, !estado);
      }
    break;}
  }


}

void causaError(){

    //Serial.println("FAILED");
    //Serial.println("REASON: " + firebaseData.errorReason());
    //Serial.println("------------------------------------");
    //Serial.println();
}

void checkTimer(byte channelPin){

}