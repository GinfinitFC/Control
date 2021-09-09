//this code is for configure the pot value in a range from 0 to 20 so that the cart tries to move at a wished position controled from the encoder
//Motor
#define EN 11
#define ina 9
#define inb 10
//const int maxSteps = 255;//motor internal steps
//encoder phases
#define A 2 //interrupt mode
#define B 3
//ultrasonic
const int Trigger = 4; //this one could be interruption
const int Echo = 5; 
volatile long t; //time taken on capturing the signal
volatile long d; //distance in cm
//position variables
volatile int prev = 0; //previous position
volatile int pos = 0; //actual position
//speed control 
int PWM_ctrl=0;
//time variables
volatile int i=0;//sample 0.02 sec
//control variables
int e=0;//error
int ref=0;//position desired read from the pot
int kp = 30; //proportional control part

void setup() {
  // put your setup code here, to run once:
  //motor and encoder pin declaration
  pinMode(EN, OUTPUT);
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  //ultrasonic
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);  

  attachInterrupt(digitalPinToInterrupt(A),ISR_encoder,RISING);

  digitalWrite(Trigger, LOW);//start trigger with 0
  
  while(d < 21){//set to origin routine
    ultrasonic_read();//refresh value
    analogWrite(EN, 127); 
    digitalWrite(ina, LOW); 
    digitalWrite(inb, HIGH);
  }
  //motor and encoder initialization
  analogWrite(EN, 0); //stop routine
  digitalWrite(ina, LOW);
  digitalWrite(inb, LOW);
  pos=0;
  delay(500);
  
  Serial.begin(9600);
  
  Serial.println("# PWM Error Encoder Ref Cm Time");
}

void loop() {
  // put your main code here, to run repeatedly:
  ultrasonic_read();//as flag
  //control routine
  int pot=analogRead(A0);
  ref=map(pot,0,1023,0,20);//this modifies the potentiometer value so that the position works now by the distance the cart moves
  e=ref-(0.0016*pos-0.1483);//error now works under the distance controlled by the encoder
  PWM_ctrl = kp*e;
  //Serial.print("PWM= ");
  //Serial.println(PWM_ctrl);
  control();
  if(pos != prev){
    //data print routine
    Serial.print(i);
    Serial.print(" ");
    Serial.print(PWM_ctrl);
    Serial.print(" ");
    Serial.print(e);
    Serial.print(" ");
    Serial.print(pos);
    Serial.print(" ");
    Serial.print(ref);
    Serial.print(" ");
    Serial.print(d*-1+20);//invert read this si because of hardware design
    Serial.print(" ");
    Serial.println(i*0.02);
    i++;//iteration
    prev=pos;
  }
  if(d < 4){//if near end
    analogWrite(EN, 0); //stop routine
    digitalWrite(ina, LOW);
    digitalWrite(inb, LOW);
    if(0.0016*pos-0.1483!=d*-1+20){//an object is on the trail
      Serial.println("please remove object");
      delay(1000);
    }else{
    Serial.println("warning to close to the edge");
    }
  }
  if(d>21){//near motor
    analogWrite(EN, 0); //stop routine
    digitalWrite(ina, LOW);
    digitalWrite(inb, LOW);
    Serial.print("warning to close to the motor");
  }
}

void ultrasonic_read(){

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
  delay(20);
}
void control(){
  if(PWM_ctrl>0){//clockwise routine
    if(PWM_ctrl>255){
      PWM_ctrl=255;
    }
    analogWrite(EN, PWM_ctrl); 
    digitalWrite(ina, HIGH); 
    digitalWrite(inb, LOW);
  }else{//counterclockwise routine
    PWM_ctrl=PWM_ctrl*-1;//pwm cannot be negative
    if(PWM_ctrl>255){
      PWM_ctrl=255;
    }
    analogWrite(EN, PWM_ctrl); //0-255
    digitalWrite(ina, LOW); 
    digitalWrite(inb, HIGH);
  }
}
void ISR_encoder(){
  if (digitalRead(B) == HIGH){
    pos--;
    }else{
    pos++;
    }
}
