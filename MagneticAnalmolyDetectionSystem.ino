#include <QMC5883LCompass.h>
#include <WiFi.h>

const char* ssid     = "amoghphone";
const char* password = "abcde25#";
const char* host     = "192.168.86.227";
const uint16_t port  =  60000;

int rx = 200, ry = 200, rz = 200; //difference calibration
//int ex=300,ey=800,ez=700;
int X1=0, Y1=0, Z1=0, X2=0, Y2=0, Z2=0;
const int s0=23, D1 = 32, D2 = 33;
const int setWifi= 25;
const int calibration = 26;
bool isWifiON=false;
bool isServerON=false;
WiFiClient client;
void setupWifi()
{
  WiFi.begin( ssid, password );
  while( WiFi.status() != WL_CONNECTED ) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println( "" );
  Serial.println( "WiFi connected.");
  Serial.println( "IP address: "   );
  Serial.println( WiFi.localIP()   );
  isWifiON=true;
  if (!client)
  {
    if (client.connect(host, port))
    {
      Serial.println("Connected to server");
      isServerON=true;
    }
    else
    {
      Serial.println("Not Connected to Server");
      isServerON=false;
    }
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(s0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(setWifi,OUTPUT);
  pinMode(calibration,OUTPUT);
}

void loop()
{
  if(digitalRead(setWifi)==HIGH)
  {
    isWifiON=false;
    isServerON=false;
    setupWifi();
  }
  digitalWrite(D2, HIGH);
  digitalWrite(s0,LOW);
  QMC5883LCompass *compass = new QMC5883LCompass();
  compass->init();
  compass->read();
  X1 = compass->getX();
  Y1 = compass->getY();
  Z1 = compass->getZ();
  delete compass;
  delay(10);

  digitalWrite(s0,HIGH);
  compass = new QMC5883LCompass();
  compass->init();
  compass->read();
  X2 = compass->getX();
  Y2 = compass->getY();
  Z2 = compass->getZ();
  delete compass;
  delay(10);

  Serial.print("X1: ");
  Serial.print(X1);
  Serial.print(" Y1: ");
  Serial.print(Y1);
  Serial.print(" Z1: ");
  Serial.print(Z1);
  Serial.print(" X2: ");
  Serial.print(X2);
  Serial.print(" Y2: ");
  Serial.print(Y2);
  Serial.print(" Z2: ");
  Serial.print(Z2);
  Serial.print(" RX: ");  
  Serial.print(rx);
  Serial.print(" RY: ");
  Serial.print(ry);
  Serial.print(" RZ: ");
  Serial.println(rz);

  if(digitalRead(calibration)==HIGH)
  {
    rx=X2-X1;
    ry=Y2-Y1;
    rz=Z2-Z1;
  }

  String data="X1: "+String(X1)+"  Y1: "+String(Y1)+"  Z1: "+String(Z1)+"  X2: "+String(X2)+"  Y2: "+String(Y2)+"  Z2: "+String(Z2);
  if(isWifiON && isServerON)
    client.println(data);
  
  if ( !((rx-70 < X2 - X1 || X2 - X1 > rx+70)  && (ry-70 < Y2 - Y1 || Y2 - Y1 > ry+70) && (rz-70 < Z2 - Z1 || Z2 - Z1 > rz+70)) )
    analogWrite(D1, 127);
  else
    analogWrite(D1, 0);

  digitalWrite(D2, LOW);

  delay(20);
}
