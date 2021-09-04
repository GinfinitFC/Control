//DC motor controlled by PWM and H-bridge direction change by a interrupt button

//Motor
#define EN 11
#define in1 9
#define in2 10
//buttons
#define spin 2
/*//encoder phases
#define A 3
#define B 4
//position variables
int Ant = 0;
volatile int pos = 0;*/
//motor variables
boolean buttonState = LOW;
int PWM_rot = 0;
int pressed = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
  pinMode(spin, INPUT);
  //set initial rotation direction
  parar();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potValue = analogRead(A0);
  int pwmOut = map(potValue, 0, 1023, 0, 255);

  analogWrite(EN, pwmOut);//send pwm signal to H-bridge

  //Read button-debounce
  if(digitalRead(spin) == true){
    pressed = !pressed;
  }
  while(digitalRead(spin) == true);
  delay(20);

  //if button is pressed - change motor direction 
  if(pressed == true & PWM_rot == 0){
    Serial.println("clockwise");
    clockwise();
  }
  //if button is pressed - change motor direction
  if(pressed == false & PWM_rot == 1){
    Serial.println("counterclockwise");
    counterclockwise();
  }
}

void parar(){
  analogWrite(EN, 0); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
void clockwise(){
  digitalWrite(in1, HIGH); //adelante ina = 1 out 1 = voltage
  digitalWrite(in2, LOW);
  PWM_rot = 1;
  delay(20);
}
void counterclockwise(){
  digitalWrite(in1, LOW); //adelante ina = 1 out 1 = voltage
  digitalWrite(in2, HIGH);
  PWM_rot = 0;
  delay(20);
}
