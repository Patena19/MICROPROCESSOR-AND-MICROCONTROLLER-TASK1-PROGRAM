/*
//1.Starting the simulation will run the program, LCD will display that reservoir is being filled by water and red led in rgb will activate
//2.The display of reservoir is being filled will last until the maximum capacity reach
//3.When maximum capacity reach the soil moisture sensor will be trigger indicating that water reach its designated level( move soil moisture sensor to maximum) also red led will be off then blue will on indicating that reservoir is full
//4.Press power button to activate buzzer and green led(indicating that tank is full, water linings can be open.
//4.When the tank Is full, it also indicates that PIR will enable,
//5.Pir will detect person who want to enter in reservoir area
//6.When PIR activate it indicates that it trigger by the motion of people then message in lcd will appear again and it will ask a certain button to press in the remote to open and close the gate

//note: toggle switch in dc motor 1 is in activated position before starting the simulation 
//note: we use slide switch to toggle the motion in PIR
*/






#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(32, 16, 2);


int faucetPin = 12;
int mSenPin = A0;
int redLEDpin = 6;
int greenLEDpin = 3;
int blueLEDpin = 5;
int buz = 11;
int moisture;
int waterFilling;
int maxCapacity = 2;

int gatePin = 2;

int IRpin=10;
IRrecv IR(IRpin);

// INPUT VARIABLE
int faucetVal;
int mSenVal;

//PIR sensor
int PIRPin=4;
int PIRVal;

// millis()
unsigned long startms;
unsigned long prevms;
int interval = 500;

// buzzer
int buzzer =9; // lagay mo dito pin;

//
bool buzzerState = LOW;
//int push = 0;

int push = 0;// rgb blink
bool rgbState = LOW;



void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  pinMode(blueLEDpin, OUTPUT);
  pinMode(gatePin, OUTPUT);
  pinMode(PIRPin, INPUT);
  pinMode(faucetPin, INPUT);
  pinMode(mSenPin, INPUT);
  pinMode(buzzer,OUTPUT);
  
  pinMode(IRpin, INPUT);
  IR.enableIRIn();
}

void loop()
{
  faucetVal = digitalRead(faucetPin);
  mSenVal = analogRead(mSenPin);
  
  while(waterFilling < maxCapacity)
  {
    // Faucet on
    if(faucetVal == 1)
    {
      digitalWrite(redLEDpin, HIGH);
      digitalWrite(greenLEDpin, LOW);
      digitalWrite(blueLEDpin, LOW);
      
      lcd.setCursor(0,0);
      lcd.print("Reservoir are");
      lcd.setCursor(0,1);
      lcd.print("being filled");
      delay(500);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("by Water...");
    }
    waterFilling++;
    Serial.println(waterFilling);
  }
  if((waterFilling == maxCapacity) && (mSenVal > 400))
  {
    digitalWrite(redLEDpin, LOW);
    digitalWrite(greenLEDpin, LOW);
    digitalWrite(blueLEDpin, HIGH);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Water level are");
    lcd.setCursor(0,1);
    lcd.print("know stable");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("We can supply");
    lcd.setCursor(0,1);
    lcd.print("water now...");
    //delay(500);
  }
  IR_remoteControl();
  
  while(push == 1)
  {
    startms = millis();
    if(startms - prevms >= interval)
    {
      prevms = startms;
      buzzerState = !buzzerState;
      prevms = startms;
      rgbState = !rgbState;
       
    }
    digitalWrite(buzzer, LOW);
    digitalWrite(buzzer, buzzerState);
    digitalWrite(buzzer, LOW);
    digitalWrite(redLEDpin, LOW);
    digitalWrite(greenLEDpin, rgbState);
    digitalWrite(blueLEDpin, LOW);
    message();
    IR_remoteControl();
  }
  
  PIRVal = digitalRead(PIRPin);
  
  while(PIRVal == 1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Someone want");
    lcd.setCursor(0,1);
    lcd.print("to enter");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press 1 to");
    lcd.setCursor(0,1);
    lcd.print("Open the gate");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press 2 to");
    lcd.setCursor(0,1);
    lcd.print("close the gate");
    
    PIRVal = digitalRead(PIRPin);
    
    while(push == 2)
    {
      digitalWrite(gatePin, HIGH);
      IR_remoteControl();
    }
    
    while(push == 3)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Supplying water");
      lcd.setCursor(0,1);
      lcd.print("are continous.");
      digitalWrite(gatePin, LOW);
      PIRVal = digitalRead(PIRPin);
      digitalWrite(redLEDpin, LOW);
      digitalWrite(greenLEDpin, LOW);
      digitalWrite(blueLEDpin, HIGH);
    }
    
    IR_remoteControl();
  }
}

void IR_remoteControl()
{
  while(IR.decode())
  {
    //Serial.println(IR.decodedIRData.decodedRawData, HEX);
    
    if(IR.decodedIRData.decodedRawData == 0xFF00BF00)
    {
      Serial.println("Power");
      push = 1;
    } 
    if(IR.decodedIRData.decodedRawData == 0xFE01BF00)
    {
      Serial.println("Volume+");
    }
    if(IR.decodedIRData.decodedRawData == 0xFD02BF00)
    {
      Serial.println("Func/Stop");
    }
    if(IR.decodedIRData.decodedRawData == 0xFB04BF00)
    {
      Serial.println("Rewind");
    }
    if(IR.decodedIRData.decodedRawData == 0xFA05BF00)
    {
      Serial.println("Pause/Play");
    }
    if(IR.decodedIRData.decodedRawData == 0xF906BF00)
    {
      Serial.println("Fast forward");
    }
    if(IR.decodedIRData.decodedRawData == 0xF708BF00)
    {
      Serial.println("Down");
    }
    if(IR.decodedIRData.decodedRawData == 0xF609BF00)
    {
      Serial.println("Volume-");
    }
    if(IR.decodedIRData.decodedRawData == 0xF50ABF00)
    {
      Serial.println("Up");
    }
    if(IR.decodedIRData.decodedRawData == 0xF30CBF00)
    {
      Serial.println("0");
    }
    if(IR.decodedIRData.decodedRawData == 0xF20DBF00)
    {
      Serial.println("EQ");
      push = 0;
    }
    if(IR.decodedIRData.decodedRawData == 0xF10EBF00)
    {
      Serial.println("ST/REPT");
    }
    if(IR.decodedIRData.decodedRawData == 0xEF10BF00)
    {
      Serial.println("1");
      push = 2;
    }
    if(IR.decodedIRData.decodedRawData == 0xEE11BF00)
    {
      Serial.println("2");
      push = 3;
    }
    if(IR.decodedIRData.decodedRawData == 0xED12BF00)
    {
      Serial.println("3");
    }
    if(IR.decodedIRData.decodedRawData == 0xEB14BF00)
    {
      Serial.println("4");
    }
    if(IR.decodedIRData.decodedRawData == 0xEA15BF00)
    {
      Serial.println("5");
    }
    if(IR.decodedIRData.decodedRawData == 0xE916BF00)
    {
      Serial.println("6");
    }
    if(IR.decodedIRData.decodedRawData == 0xE718BF00)
    {
      Serial.println("7");
    }
    if(IR.decodedIRData.decodedRawData == 0xE619BF00)
    {
      Serial.println("8");
    }
    if(IR.decodedIRData.decodedRawData == 0xE51ABF00)
    {
      Serial.println("9");
    }
    IR.resume();
  }
}

void message()
{
  PIRVal = digitalRead(PIRPin);
  if(PIRVal == 1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Someone want");
    lcd.setCursor(0,1);
    lcd.print("to enter");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press 1 to");
    lcd.setCursor(0,1);
    lcd.print("Open the gate");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press 2 to");
    lcd.setCursor(0,1);
    lcd.print("close the gate");
  }
}
