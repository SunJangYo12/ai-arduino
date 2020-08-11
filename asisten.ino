/* **********************************
 * SERIAL VOLTMETER   radius 0v-32v     |
                                        |
  ------|--wwww----|----wwww---|------  |
    +test   100k   A0    10k   GND      |
 ************************************
 
 * SERIAL OHMETER
 
 -------|--wwww--|--wwww---|-------
       +5v  1k  A0   test  GND  
 *************************************
*/

// menu
int index = -1;
String inputCMD = "";
boolean isDone = false;

// voltmeter
float volt, voltage, vin;
float vR1 = 100000.0;
float vR2 = 10000.0;

// ohmeter
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 10000;
float R2 = 0;
float buffer = 0;

// servo
int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;
int delayTime = 1000;
int loopStep = 10; //530

//IO
int IO1 = 2;
int IO2 = 3;

//PINCEK
int cekpinout = 0;
int cekpinin = 0;
int datacekpinout = 0;
int datacekpinin = 0;

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IO1, INPUT);
  pinMode(IO2, INPUT);
  
  Serial.begin(9600);
  inputCMD.reserve(200);
}

void loop()
{
  menu();
  
  if (index == 0) {
    voltmeter();
  }
  else if (index == 1) {
    ohmeter();
  }
  else if (index == 2) {
    for (int i=0; i<loopStep; i++) {
       servoStep1();
       delay(delayTime);
       servoStep2();
       delay(delayTime);
       servoStep3();
       delay(delayTime);
       servoStep4();
       delay(delayTime);
    }
    Serial.println("servo done");
    index = -1;
    servoStop();
  }
  else if (index == 3) {
    for (int i=0; i<loopStep; i++) {
       servoStep4();
       delay(delayTime);
       servoStep3();
       delay(delayTime);
       servoStep2();
       delay(delayTime);
       servoStep1();
       delay(delayTime);
    }
    Serial.println("servo done");
    index = -1;
    servoStop();
  }
  else if (index == 4) {
    Serial.print("pin 2: "); Serial.println(digitalRead(IO1));
    Serial.print("pin 3: "); Serial.println(digitalRead(IO2));

    Serial.println("");
    delay(100);
  }
  else if (index == 5) {
    if (datacekpinout == 0) {
      Serial.println("Masukan OUTPIN: ");
    }
    else {
      Serial.print("Cekpin OUT: ");Serial.println(datacekpinout);
      digitalWrite(datacekpinout, HIGH);
      delay(300);
      digitalWrite(datacekpinout, LOW);
    }
    delay(300);
  }
  else if (index == 6) {
     if (datacekpinin == 0) {
       Serial.println("Masukan INPIN: ");
     }
     else {
       Serial.print("Cekpin IN: ");Serial.println(datacekpinin);
       Serial.print("Data Wire: ");Serial.println(digitalRead(datacekpinin));
     }
     delay(300);
  }
}

void menu() {
  if (isDone) 
  {
    if (inputCMD.equals("voltmeter\n")) {
      Serial.println("~~~ VOLTMETER dibuka! ~~~\n");
      index = 0;
    }
    if (inputCMD.equals("ohmeter\n")) {
      Serial.println("~~~ OHMETER dibuka! ~~~\n");
      index = 1;
    }
    if (inputCMD.equals("servoLeft\n")) {
      Serial.println("~~~ SERVO left dibuka! ~~~\n");
      index = 2;
    }
    if (inputCMD.equals("servoRight\n")) {
      Serial.println("~~~ SERVO right dibuka! ~~~\n");
      index = 3;
    }
    if (inputCMD.equals("input\n")) {
      Serial.println("~~~ PIN INPUT dibuka! ~~~\n");
      index = 4;
    }
    if (inputCMD.equals("cekpinout\n")) {
      Serial.println("~~~ cekpinout dibuka ~~~\n");
      index = 5;
      cekpinout = 1;
    }
    if (inputCMD.equals("cekpinin\n")) {
      Serial.println("~~~ cekpinin dibuka ~~~\n");
      index = 6;
      cekpinin = 1;
    }
    if (inputCMD.equals("exit\n")) {
      Serial.println("keluar berhasil");
      index = -1;
      cekpinout = 0;
      cekpinin = 0;
    }
    if (inputCMD.equals("bantuan\n")) {
      Serial.println("\n~~~~ Asisten v1.0 ~~~~\n");
      Serial.println("1) voltmeter");
      Serial.println("2) ohmeter");
      Serial.println("3) servoLeft");
      Serial.println("4) servoRight");
      Serial.println("5) input");
      Serial.println("6) exit");
      Serial.println("7) bantuan");
      Serial.println("8) cekpinout");
      Serial.println("9) cekpinin");
      Serial.println("\n");
      index = -1;
    }
    if (cekpinout == 1) {
      datacekpinout = inputCMD.toInt();
      pinMode(datacekpinout, OUTPUT);
    }
    if (cekpinin == 1) {
      datacekpinin = inputCMD.toInt();
      pinMode(datacekpinin, INPUT);
    }
    
    inputCMD = "";
    isDone = false;
  }
}

void servoStep1() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}
void servoStep2() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void servoStep3() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void servoStep4() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void servoStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void ohmeter() {
  raw = analogRead(A0);
  if (raw) {
    buffer = raw * Vin;
    Serial.println("________");
    Serial.print("raw: ");
    Serial.println(raw);
    Serial.print("Vout: ");
    Serial.println(Vout);
    Vout = (buffer) / 1024.0;
    buffer = (Vin / Vout) - 1;
    Serial.print("buffer2: ");
    Serial.println(buffer);
    R2 = R1 * buffer;
    Serial.print("Vout: ");
    Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2);
    delay(1000);
  }
}

void voltmeter() {
  volt = analogRead(A0);
  //voltage = volt * (5.0 / 1023.0);
  voltage = (volt * 5.0) / 1024.0;
  vin = voltage / (vR2/(vR1+vR2));
  
  if (vin < 0.09) vin=0.0;
  
  Serial.print("Voltage: ");
  Serial.print(vin);
  Serial.println(" V");
  delay(100);
}


void serialEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read();
    inputCMD += inChar;
    if (inChar == '\n') {
      isDone = true;
    }
  }
}
