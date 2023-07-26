//INCLUDE SECTION
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//DEFINE SECTION
#define DHT_PIN 2
#define gasSensor A0

//GLOBAL VARIABLES SECTION
//SDA: A4, SCL:A5
LiquidCrystal_I2C lcd (0x27,2,1,0,4,5,6,7);// DIR,E,RW,RS,D4,D5,D6,D7 
DHT dht_sensor(DHT_PIN,DHT11);
typedef struct Lecturas
{
  byte temp = 0;
  byte hum = 0;
  byte gas[4] = {0};
};

struct Lecturas datos;
  
void setup() {
  Wire.begin(); // Inicia el puerto I2C
  dht_sensor.begin();
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(9600); // Establece la velocidad de datos del puerto serie

}

void loop() {
  Leer_Mostrar_Datos();
  delay(500);
  enviar_por_serie();
}

void enviar_por_serie()
{
  Serial.write((const char*)&datos, sizeof(struct Lecturas));
}

void Leer_Mostrar_Datos()
{
  datos.temp = dht_sensor.readTemperature();
  datos.hum = dht_sensor.readHumidity();
  unsigned long gas = analogRead(gasSensor);
  fromIntToBytes(datos.gas,gas);
  lcd.setCursor(0,0);
  lcd.print("TMP:");
  lcd.print(datos.temp,DEC);
  lcd.print(" HUM:");
  lcd.print(datos.hum,DEC);lcd.print("%  ");
  lcd.setCursor(0,1);
  lcd.print("NIVEL GAS:");
  lcd.print(gas,DEC);lcd.print("    ");
}

void fromIntToBytes(byte* bytes, long lng)
{
  bytes[0] = (byte)((lng & 0xFF000000) >> 24);
  bytes[1] = (byte)((lng & 0x00FF0000) >> 16);
  bytes[2] = (byte)((lng & 0x0000FF00) >> 8);
  bytes[3] = (byte)((lng & 0x000000FF));
}
