#include <Servo.h>
#include <SoftwareSerial.h>
//Bluetooth
SoftwareSerial bluet(6, 7); // tx->12 rx->13
Servo miServo;
//Temperatura y Humedad
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x3F,16,2);
#include <DHT.h>
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Declaramos nuestros pines
int habilitaMA = 3;
int mAdelanteA = 2;
int mAtrasA = 4;
int buzzer = 11;
int sensorPir = 9;
int echo = 12;
int trigger = 13;
int gasPin = A0; // El pin analógico al que está conectado el sensor de gas
int gasValue; // Variable para almacenar el valor del sensor de gas
char datos;

void setup() {
  // Inicializamos nuestro monitor serial
  Serial.begin(9600);
  // Cambia a 38400
  bluet.begin(9600);
  // Motores
  pinMode(mAdelanteA, OUTPUT);
  pinMode(mAtrasA, OUTPUT);
  pinMode(habilitaMA, OUTPUT);
  velocidadBaja();
  //Buzzer
   pinMode(buzzer, OUTPUT);
  // Volante
  miServo.attach(5);
  // Humedad y Temperatura
    Serial.begin(9600);
    lcd.init () ;
    lcd.backlight();
    lcd.print("");
    dht.begin();
  // Movimiento
  pinMode(sensorPir, INPUT);
  //Ultrasonico
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
}

void loop() {
  if (bluet.available()) {
    datos = bluet.read();

    if (datos == 'A') {
      // Adelante
      carroAdelante();
    } else if (datos == 'B') {
      // Atras
      carroAtras();
    } else if (datos == 'C') {
      // Izquierda
      carroIzquierda();
    } else if (datos == 'D') {
      // Derecha
      carroDerecha();
    } else if (datos == 'E') {
      // Centro
      centrarRuedas();
    } else if (datos == 'F') {
      // Ningun motor funcionando
      carroDetener();
    } else if (datos == 'L') {
      // Baja
      velocidadBaja();
    } else if (datos == 'H') {
      // Alta
      velocidadAlta();
    } else if (datos == 'J') {
      alarmaSirena();
    } else if (datos == 'K') {
      alarmaClaxon();
    } else if (datos == 'G') {
      alarmaApagadas();
    }
  }

  temperaturaHumedad();
  movimientoSensor();
  alarmaReversa();
  alarmaSirena();
  sensorGas();
}


//FUNCIONES AUTOMOVIL
void carroAdelante() {
  digitalWrite(mAdelanteA, HIGH);
  digitalWrite(mAtrasA, LOW);
  Serial.println("El carro esta en marcha");
}

void carroAtras() {
  digitalWrite(mAdelanteA, LOW);
  digitalWrite(mAtrasA, HIGH);
  Serial.println("El carro esta en marcha atras");
}

void carroDetener() {
  digitalWrite(buzzer, LOW);
  digitalWrite(mAdelanteA, LOW);
  digitalWrite(mAtrasA, LOW);
  Serial.println("El carro esta detenido");
  alarmaApagadas();
}

void carroDerecha() {
 miServo.write(50);
 Serial.println("El carro esta hiendo hacia la derecha");
}

void centrarRuedas() {
 miServo.write(90);
}

void carroIzquierda() {
  miServo.write(135);
   Serial.println("El carro esta hiendo hacia la izquierdo");
}

void velocidadBaja() {
 analogWrite(habilitaMA, 60);
  Serial.println("El carro esta hiendo muy lento");
}

void velocidadAlta() {
 analogWrite(habilitaMA, 150);
  Serial.println("El carro esta hiendo muy rápido");
}

// FUNCIONES SONIDOS
void alarmaSirena(){
   if (digitalRead(mAdelanteA) == HIGH) {
   // Primera parte de la sirena
  tone(buzzer, 880);
  delay(100);
  tone(buzzer, 440);
  delay(100);
   }
}

void alarmaClaxon(){
 tone(buzzer, 440); // Tono de 440Hz durante medio segundo
}
void alarmaApagadas(){
 noTone(buzzer);
}

void alarmaReversa(){
  if (digitalRead(mAtrasA) == HIGH) {


int distancia;
int duracion;

  //Enviar el pulso
  digitalWrite(trigger,HIGH);
  delay(1);
  digitalWrite(trigger,LOW);


//Recibir el pulso y calcular la distancia
  duracion = pulseIn(echo, HIGH);
  distancia=duracion/58.2;


  Serial.println(distancia);
  delay(1);


if(distancia>=5 and distancia<20){
  // Sonidos intermitentes
 digitalWrite(buzzer, HIGH);
 delay(30);
 digitalWrite(buzzer, LOW);
 delay(30);
}
if(distancia>=20 and distancia<30){
 digitalWrite(buzzer, HIGH);
 delay(60);
 digitalWrite(buzzer, LOW);
 delay(60);
}
if(distancia>=30 and distancia<50){
  digitalWrite(buzzer, HIGH);
 delay(120);
 digitalWrite(buzzer, LOW);
 delay(120);
}
if(distancia>=50){
  digitalWrite(buzzer, HIGH);
}
}
}

void alarmaBloquea(){
 noTone(buzzer); // Detiene el tono
}

//FUNCIONES HUMEDAD Y TEMPERATURA
void temperaturaHumedad(){
      // Esperamos 5 segundos entre medidas
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahrenheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  // Calcular el índice de calor en Fahrenheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
  lcd.setCursor(0,0);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");
  }
// FUNCIONES SENSOR DE MOVIMIENTO --- LISTO
void movimientoSensor(){
   int pirmov = digitalRead(sensorPir);
  if (digitalRead(mAtrasA) == HIGH || digitalRead(mAdelanteA) == HIGH ) {
  }else{
if(pirmov==1){
   mostrarMensajeLCD("ADVERTENCIA:","Movimiento");
      digitalWrite(buzzer, HIGH);
       delay(50);
       digitalWrite(buzzer, LOW);
       delay(50);   
       digitalWrite(buzzer, HIGH);
       delay(50);  
           digitalWrite(buzzer, LOW);
       delay(500);   
  }
  }
  }
//FUNCIONES SENSOR DE GAS
void sensorGas(){
if (digitalRead(mAtrasA) == HIGH || digitalRead(mAdelanteA) == HIGH ) {
  }else{
      gasValue = analogRead(gasPin); // Leer el valor del sensor de gas
      Serial.print("Valor del sensor de gas: ");
      Serial.println(gasValue); // Imprimir el valor del sensor de gas en el monitor serie

      if(gasValue >= 400 ){
         mostrarMensajeLCD("PELIGRO:","Gas");
         digitalWrite(buzzer, HIGH);
         delay(15);
         digitalWrite(buzzer, LOW);
         delay(15);
      }
  }
}
// FUNCIONES DISPLAY 2*16
void mostrarMensajeLCD(String asunto, String mensaje){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(asunto);
  lcd.setCursor(0,1);
  lcd.print(mensaje);
}
