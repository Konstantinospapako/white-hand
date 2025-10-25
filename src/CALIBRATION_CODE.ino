

//variables for my glove
const int flexPin52 = A0;      // Pin connected to voltage divider output for index finger
const int flexPin25 = A1;      // Pin connected to voltage divider output for thumb finger



void setup() {
  Serial.begin(9600);
  //setup pins
  pinMode(flexPin52, INPUT);
  pinMode(flexPin25, INPUT);
}

void loop() {
  //flex sensors reading
  int ADCflexIn = analogRead(flexPin52);
  int ADCflexTh = analogRead(flexPin25);
  Serial.println("FLEX INDEX: "+ String(ADCflexIn) + "              "  +"FLEX THUMB: "+ String(ADCflexTh) );

 delay(1000);
}
