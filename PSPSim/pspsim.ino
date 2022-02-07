const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

float voltage = 12.43;
float current = 0.342;
boolean output = true;
boolean remote = false;

void setup() {
 Serial.begin(2400);
 randomSeed(analogRead(0));
}

void loop() {
 recvWithEndMarker();

 if(newData) {
  newData = false;

  current = current + random(-20, 20)/1000.0;
  
  if(strlen(receivedChars) == 1 && receivedChars[0] == 'L') {
    float power = output ? voltage * current : 0;
    
    Serial.print("V");
    if(voltage < 10) Serial.print("0");
    Serial.print(voltage, 2);
    Serial.print("A");
    Serial.print(output ? current: 0, 3);
    Serial.print("W");
    if(power < 10) Serial.print("00");
    else if(power < 100) Serial.print("0");
    Serial.print(power, 1);
    Serial.print("U13I5.00P060F");
    Serial.print(output ? "1" : "0");
    Serial.print("011");
    Serial.print(remote ? "1" : "0");
    Serial.print("0\r\n");
  }
 }else if(strlen(receivedChars) == 3) {
  if(strcmp(receivedChars, "KOE") == 0) {
    output = true;
    remote = true;
  } else if(strcmp(receivedChars, "KOD") == 0) {
    output = false;
    remote = true;
  }
 }else if(strlen(receivedChars) == 8) {
  if(receivedChars[0] == 'S' && receivedChars[1] == 'V' && receivedChars[2] == ' ') {
    voltage = atof(receivedChars+3);
    remote = true;
  }
 }

}

void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\r';
 char rc;
 
 while (Serial.available() > 0 && newData == false) {
   rc = Serial.read();
  
   if (rc != endMarker) {
     receivedChars[ndx] = rc;
     ndx++;
     if (ndx >= numChars) {
       ndx = numChars - 1;
     }
   }
   else {
     receivedChars[ndx] = '\0';
     ndx = 0;
     newData = true;
   }
 }
}
