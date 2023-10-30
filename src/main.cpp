#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define PWM 4
int velocidad = 1000;
char dato;
int contconexion = 0;
const char *ssid = "MotoG";
const char *password = "ras123456";
char SERVER[50] = "postman.cloudmqtt.com";
int SERVERPORT = 13730;
String USERNAME = "aplicada";
char PASSWORD[50] = "0123456789";

unsigned long previousMillis = 0;
char charColor[40];
String strColor;
String strColorUltimo;

char charFigura[15];
String strFigura;
String strFiguraUltimo;
char PLACA[50];
char valueStr[40];
char valueStr1[15];
//String strtemp = "";

char COLOR[50];
char FIGURA[50];

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  char PAYLOAD[5] = "    ";
  Serial.print("Mensaje Recibido: [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    PAYLOAD[i] = (char)payload[i];
  }

  Serial.println(PAYLOAD);
}

void reconnect()
{
  uint8_t retries = 3;
  while (!client.connected())
  {
    Serial.print("Intentando conexion MQTT...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    USERNAME.toCharArray(PLACA, 50);
    if (client.connect("", PLACA, PASSWORD))
    {
      Serial.println("conectado");
    }
    else
    {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" intenta nuevamente en 5 segundos");
      delay(5000);
    }
    retries--;
    if (retries == 0)
    { // esperar a que el WDT lo reinicie
      while (1)
        ;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  pinMode(PWM, OUTPUT);

  while (!Serial)
  {
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED and contconexion < 50)
  {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }

  if (contconexion < 50)
  {
    Serial.println("");
    Serial.println("WiFi conectado");
  }
  else
  {
    Serial.println("");
    Serial.println("Error de conexion");
  }

  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);

  String color = "/" + USERNAME + "/" + "Color";
  color.toCharArray(COLOR, 50);
  String fugura = "/" + USERNAME + "/" + "Figura";
  fugura.toCharArray(FIGURA, 50);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  analogWrite(PWM, velocidad);
  client.loop();

  if (Serial.available() > 0)
  {
    char dato = Serial.read();
    if (dato == '2')
    {
      strColor = "Potenca: 0.1w";
      //Serial.println("Velociad: 0= 100");
      velocidad = 250;
      //strFigura = "CIRCULO";
    }
    else if (dato == '3')
    {
      strColor = "Potenca: 0.15w";
      //Serial.println("Velociad: 0= 100");
      velocidad = 300;
      //strFigura = "TRIANGULO";
    }
    else if (dato == '4')
    {
      strColor = "Potenca: 0.2w";
      //Serial.println("Velociad: 0= 100");
      velocidad = 400;
      //strFigura = "CUADRADO";
    }
    else if (dato == '0')
    {
      strColor = "Calculando...";
      //Serial.println("Velociad: 0= 100");
      velocidad = 100;
      //strFigura = "SIN FIGURA";
    }
    else
    {
      strColor = "Calculando...";
      //Serial.println("Velociad: 0= 100");
      velocidad = 100;
      //strFigura = "SIN FIGURA";
    }
  }

  if (strColor != strColorUltimo)
  {
    strColorUltimo = strColor;
    strColor.toCharArray(valueStr, 15);
    Serial.println("Enviando: [" + String(COLOR) + "] " + strColor);
    client.publish(COLOR, valueStr);
    /*strFigura.toCharArray(valueStr1, 15);
    Serial.println("Enviando: [" + String(FIGURA) + "] " + strFigura);
    client.publish(FIGURA, valueStr1);*/
  }
}