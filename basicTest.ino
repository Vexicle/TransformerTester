//define pin numbers for different wired connections
//Code will write to transformerWritePinx, and expect a response from transformerReadPinx
#define transformerWritePin1 1
#define transformerWritePin2 1
#define transformerWritePin3 1
#define transformerReadPin1 1
#define transformerReadPin2 1
#define transformerReadPin3 1
//if power is recieved from this pin, assume button is pressed
#define buttonReadPin 1
void setup() {
  //setting modes for transformer testing read and write pins
  pinMode(transformerWritePin1, OUTPUT);
  pinMode(transformerWritePin2, OUTPUT);
  pinMode(transformerWritePin3, OUTPUT);
  pinMode(transformerReadPin1, INPUT);
  pinMode(transformerReadPin2, INPUT);
  pinMode(transformerReadPin3, INPUT);
  //pin that can be digitalread to get signal if button is pressed
  pinMode(buttonReadPin, INPUT);
  Serial.begin(9600);
}
//check if button is pressed
bool isButtonPressed(int pinNumber=buttonReadPin){
  bool result = digitalRead(pinNumber);
  if(result){
    return(1);
  } 
  else{
    return(0);
    }
}
//test continuity between pins, returns a bool array with results for each pair
bool* testTransformerPins(//fairly limited/static way of testing, but simple, might be worth returning full results for all pin tests to check for any faulty shorts
uint8_t wpin1 = transformerWritePin1,
uint8_t wpin2 = transformerWritePin2,
uint8_t wpin3 = transformerWritePin3,
uint8_t rpin1 = transformerReadPin1,
uint8_t rpin2 = transformerReadPin2,
uint8_t rpin3 = transformerReadPin3){
  static bool results[]={0,0,0};
  //test pair 1
  digitalWrite(wpin1, HIGH);
  delay(10);
  if (digitalRead(rpin1)){results[0]=1;}
  else{results[0]=0;}
  //test pin 2
  digitalWrite(wpin2, HIGH);
  delay(10);
  if (digitalRead(rpin2)){results[1]=1;}
  else{results[1]=0;}
  //test pin 3
  digitalWrite(wpin3, HIGH);
  delay(10);
  if (digitalRead(rpin3)){results[2]=1;}
  else{results[2]=0;}
  Serial.println(results[0]);
  return (results);
}
//takes a bool array like the one from testTransformerPins, and basically just acts as an and gate, outputting true if all are true, otherwise outputting false
bool checkPairs( bool* results=NULL){//in the same vein as testTransformerPins, this one could do with a little more flexability
  if (results==NULL){Serial.println("pin reading results not passed in correctly to checkPairs");}
//check if all have continuity
  if(results[0]&&results[1]&&results[2]){
    Serial.println("pass");
    return(true);}
  else{return(false);}

}
//checks if button is pressed, if so tries to test the transformer continuity
bool checkManualTest(bool logging=1){
  Serial.println("Checking for button press");
  if (isButtonPressed()){
    Serial.println("Button press detected, testing continuity");
    checkPairs(testTransformerPins());
  }
  else{Serial.println("No button press detected");}
}
void loop() {
  // put your main code here, to run repeatedly:
  checkManualTest();
 delay(100);
}
