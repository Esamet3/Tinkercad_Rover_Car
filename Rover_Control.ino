#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(0);

const int solMotorHiz = 10, solMotorIleri= 9, solMotorGeri= 8;
const int sagMotorHiz = 5, sagMotorIleri= 4, sagMotorGeri=3;
const int kirmiziLed= 13, beyazLed= 2;
const int ontrigPin= 12, onechoPin= 11;
const int arkatrigPin= 7, arkaechoPin= 6;
char sonKomut=' ';
unsigned long eskiZaman=0;
bool ledDurumu= LOW;
void setup(){
    pinMode(solMotorHiz, OUTPUT);
    pinMode(solMotorIleri, OUTPUT);
    pinMode(solMotorGeri, OUTPUT);
    pinMode(sagMotorHiz, OUTPUT);
    pinMode(sagMotorIleri, OUTPUT);
    pinMode(sagMotorGeri, OUTPUT);
    pinMode(kirmiziLed, OUTPUT);
    pinMode(beyazLed, OUTPUT);
  	pinMode(ontrigPin, OUTPUT);
  	pinMode(onechoPin, INPUT);
  	pinMode(arkatrigPin, OUTPUT);
  	pinMode(arkaechoPin, INPUT);
  	Serial.begin(9600);
  	analogWrite(solMotorHiz, 255);
    analogWrite(sagMotorHiz, 255);
  	lcd.begin(16,2);
  	lcd.setBacklight(1);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sistem Hazir");
  	lcd.setCursor(0,1);
  	lcd.print("Komut Bekleniyor");
    Serial.println("--ROVER SISTEM HAZIR--");
    Serial.println("W: Ileri Git");
    Serial.println("S : Geri Git");
    Serial.println("A : Sola Don");
    Serial.println("D : Saga Don");
    Serial.println("Bosluk (Space) : Durdur");
    Serial.println("----------------------------------");          
    
}
void ileri(){
  digitalWrite(solMotorIleri, HIGH);
  digitalWrite(solMotorGeri, LOW);
  digitalWrite(sagMotorIleri, HIGH);
  digitalWrite(sagMotorGeri, LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Durum: Ileri");
  digitalWrite(beyazLed, HIGH);
  digitalWrite(kirmiziLed, LOW);
}
void dur(){
  digitalWrite(solMotorIleri, LOW);
  digitalWrite(solMotorGeri, LOW);
  digitalWrite(sagMotorIleri, LOW);
  digitalWrite(sagMotorGeri, LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Durum: Durdu");
  digitalWrite(beyazLed, LOW);
  digitalWrite(kirmiziLed, HIGH);
}
void geri(){
  digitalWrite(solMotorIleri, LOW);
  digitalWrite(solMotorGeri, HIGH);
  digitalWrite(sagMotorIleri, LOW);
  digitalWrite(sagMotorGeri, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Durum: Geri");
  digitalWrite(beyazLed, LOW);
  digitalWrite(kirmiziLed, HIGH);
}
void solaDon(){
  digitalWrite(solMotorIleri, LOW);
  digitalWrite(solMotorGeri, HIGH);
  digitalWrite(sagMotorIleri, HIGH);
  digitalWrite(sagMotorGeri, LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Durum:Sola Donme");
  digitalWrite(beyazLed, HIGH);
  digitalWrite(kirmiziLed, LOW);
}
void sagaDon(){
  digitalWrite(solMotorIleri, HIGH);
  digitalWrite(solMotorGeri, LOW);
  digitalWrite(sagMotorIleri, LOW);
  digitalWrite(sagMotorGeri, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Durum: Saga Donme");
  digitalWrite(beyazLed, HIGH);
  digitalWrite(kirmiziLed, LOW);
}
int onMesafeOlc(){
  digitalWrite(ontrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ontrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ontrigPin, LOW);
  long gecenSure = pulseIn(onechoPin, HIGH);
  int mesafe = gecenSure*0.034/2;
  return mesafe;
}
int arkaMesafeOlc(){
  digitalWrite(arkatrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(arkatrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(arkatrigPin, LOW);
  long gecenSure = pulseIn(arkaechoPin, HIGH);
  int mesafe = gecenSure*0.034/2;
  return mesafe;
}
void geriLed(){
  unsigned long suankiZaman = millis();
  if ( suankiZaman - eskiZaman >=300){
    eskiZaman = suankiZaman;
    ledDurumu= !ledDurumu;
    digitalWrite(kirmiziLed, ledDurumu);
  }
}

void loop(){
  int onGuncelMesafe= onMesafeOlc();
  int arkaGuncelMesafe= arkaMesafeOlc();
  
  if( sonKomut=='w'|| sonKomut=='W'){
    if(onGuncelMesafe >0 && onGuncelMesafe <=20){
      dur();
      Serial.println("Acil Fren Yapıldı");
      sonKomut=' ';
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("! TEHLIKE !");
      lcd.setCursor(0,1);
      lcd.print("Arac Durduruldu");
    }
    else if (onGuncelMesafe > 20 && onGuncelMesafe<=50){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Engel Var");
      lcd.setCursor(0,1);
      lcd.print("Mesafe: ");
      lcd.print(onGuncelMesafe);
    }
  }
  if( sonKomut=='s'|| sonKomut=='S'){
    geriLed();
    if(arkaGuncelMesafe >0 && arkaGuncelMesafe <=20){
      dur();
      Serial.println("Acil Fren Yapildi");
      sonKomut=' ';
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("! TEHLIKE !");
      lcd.setCursor(0,1);
      lcd.print("Arac Durduruldu");
    }
    else if (arkaGuncelMesafe > 20 && arkaGuncelMesafe<=50){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Engel Var");
      lcd.setCursor(0,1);
      lcd.print("Mesafe: ");
      lcd.print(arkaGuncelMesafe);
    }
  }
  
  if(Serial.available() > 0){
    char komut = Serial.read();
    sonKomut= komut;
    if( komut=='w' || komut=='W'){
      ileri();
      Serial.println("Ileri Gidiyor");
    }
    else if( komut=='s' || komut=='S'){
      geri();
      Serial.println("Geri Gidiliyor");
    }
    else if( komut=='a' || komut=='A'){
      solaDon();
      Serial.println("Sola Donuluyor");
    }
    else if( komut=='d' || komut=='D'){
      sagaDon();
      Serial.println("Saga Donuluyor");
    }else if ( komut==' '){
      dur();
      Serial.println("Durduruldu");
    }
  }
  delay(50);
}
