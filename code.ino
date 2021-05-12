#include <LiquidCrystal_I2C.h>    // Inclui a Biblioteca
#include <RTClib.h>               // Inclui a Biblioteca
#include <Servo.h>
#define MAX_SIZE 10 

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Inicia o display 16x2 no endereço 0x27
Servo myservo;

int pos = 0;

int b_main = 9;
int b_up = 10;
int b_down = 7;
int b_left = 0;
int b_right = 1;
bool configuracao = false;
bool pisca = false;

int hours[MAX_SIZE];
int minutes[MAX_SIZE];
int index = 0;
int posicao = 1;

void setup()  {
  rtc.begin();                                        // Inicia o módulo RTC
  Serial.begin(9600);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));     // Ajuste Automático da hora e data
  //rtc.adjust(DateTime(2019, 11, 29, 10, 23, 00));   // Ajuste Manual (Ano, Mês, Dia, Hora, Min, Seg)
 
  lcd.init();           // Inicia o Display
  lcd.backlight();      // Inicia o Backlight

  myservo.attach(3);

  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  //pinMode(1,INPUT_PULLUP);
  //pinMode(0,INPUT_PULLUP);

}
 
void loop()   {
  //eat();
    lcd.clear();
    pisca = !pisca;
    DateTime now = rtc.now();
    //lcd.print();
    Serial.print(now.minute());
    if(configuracao){
      configurar();
    }
    else{
      hora_atual();
      for(int i = 0; i < MAX_SIZE; i++){
      if(hours[i] == now.hour() && minutes[i] == now.minute() && now.second() == 0)
        eat();
      }
      Serial.print(digitalRead(b_main));    
      if(digitalRead(b_main) == LOW ){
        configuracao = true;
      }
  }
  delay(500);
}

void eat(){
  lcd.clear();
  lcd.print("**Alimentando!**");
  for(pos = 0; pos<180;pos++){
    myservo.write(pos);
    delay(15);
  }
  delay(3000);
  for(pos = 180; pos>=0;pos--){
    myservo.write(pos);
    delay(15);
  }

}

void hora_atual(){
  DateTime now = rtc.now();

  lcd.setCursor(1, 0);              // Posiciona o cursor na primeira linha
  lcd.print("Hora: ");              // Imprime o texto "Hora: "
  lcd.print(rtc.now().hour());     // Imprime a Hora
  lcd.print(":");                   // Imprime o texto entre aspas
  lcd.print(rtc.now().minute());    // Imprime o Minuto
  lcd.print(":");                   // Imprime o texto entre aspas
  lcd.print(rtc.now().second());    // Imprime o Segundo
 
  lcd.setCursor(0, 1);              // Posiciona o cursor na segunda linha
  lcd.print("Data: ");              // Imprime o texto entre aspas
  lcd.print(rtc.now().day());       // Imprime o Dia
  lcd.print("/");                   // Imprime o texto entre aspas
  lcd.print(rtc.now().month());     // Imprime o Mês
  lcd.print("/");                   // Imprime o texto entre aspas
  lcd.print(rtc.now().year());      // Imprime o Ano
}

void configurar(){
  Serial.print("configurar");
  lcd.setCursor(1, 0);
  lcd.print(index+1);
  lcd.print(": ");
  if(pisca || posicao != 1){
    lcd.print(hours[index]);    
  }
  else{
    lcd.print("  ");
  }
  lcd.print(":");
  if(pisca || posicao != 2){
    lcd.print(minutes[index]);
  }
  else{
    lcd.print("  ");
  }

  Serial.print(digitalRead(b_main)); Serial.print(digitalRead(b_up));   Serial.print(digitalRead(b_down));   Serial.print(digitalRead(b_left));   Serial.print(digitalRead(b_right)); Serial.print("\n");   
  if(digitalRead(b_up) == LOW){
    Serial.print("UP\n");
    if(posicao == 1){
       hours[index]++;
       if(hours[index] == 24) hours[index] = 0;
    }
    else{
       minutes[index]++;
       if(minutes[index] == 60) minutes[index] = 0;
    }

  }/*
  if(digitalRead(b_down) == LOW){
    Serial.print("DOWN\n");
    if(posicao == 1){
      hours[index]--;
      if(hours[index] == -1) hours[index] = 23;
    }
    else{
      minutes[index]--;
      if(minutes[index] == -1) minutes[index] = 59;
    }

  }*/
  if(digitalRead(b_down) == LOW){
    Serial.print("RIGHT\n");
    Serial.print(digitalRead(b_left));
    if(posicao == 1){
      posicao = 2;
    }
    else{
      posicao = 1;
      index++;
      if(index == 10){
        index = 0;
      }
    }
  }/*
  if(digitalRead(b_left) == LOW){
    Serial.print("LEFT\n");
    if(posicao == 1){
      index--;
      if(index == -1){
        index = 9;
      }
    }
    else{
      posicao = 1;
    }
  } */
  if(digitalRead(b_main) == LOW){
    Serial.print("MENU\n");
    index = 0;
    configuracao = false;
    posicao = 1;
  } 
}
