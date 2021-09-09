//this code will add 1 to the PWM signal that controls the speed of the motor, using an ultrasonic sensor as a flag to reset the original position
// in order to have a better measure

//Motor
int EN = 11;
int ina = 9;
int inb = 10;
//const int maxSteps = 255;//motor internal steps
//encoder phases
int A = 2; //interrupt mode
int B = 3;
//ultrasonico
const int Trigger = 4;   //Pin digital 4 para el Trigger del sensor
const int Echo = 5;   //Pin digital 5 para el Echo del sensor
volatile long t; //tiempo que demora en llegar el eco
volatile long d; //distancia en centimetros
//position variables
volatile int Ant = 0;
volatile int pos = 0;
//speed control
volatile int PWM=30;//start motor with 45 pwm below doesnt move so we can do 15 lectures +5 each cicle
//time variables
//const int timeTreshold = 5; // spin control can be modified
volatile int i=0;
unsigned long currentTime;
unsigned long storedTime; //reset time

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
  //time
  currentTime = millis();
  storedTime = currentTime;

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(A),ISR_encoder,RISING);

  digitalWrite(Trigger, LOW);
  //motor initialization
  parar();
  delay(1000);
  clockwise();
  
  Serial.println("# PWM Pos Ti");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime=millis();
  ultrasonic_read();//as flag
  //Serial.print("cm: ");
  //Serial.println(d);
  if(d >= 20){
    PWM = PWM+5;
    if(PWM > 255){
      PWM=0;//stop motor
      Serial.print("done");
    }
    delay(20);
    while(pos <= 1000){//motor will go clockwise until encoder reaches position
      //data print routine
      Serial.print(i);
      Serial.print(" ");
      Serial.print(PWM);
      Serial.print(" ");
      Serial.print(pos);
      Serial.print(" ");
      Serial.println(i*0.025);
      //Serial.print("  ");
      //Serial.println(currentTime);
      clockwise();
      i++;
    }
    currentTime=millis()-storedTime;
  }
  else{//set to origin position
    parar();
    delay(500);
    while(d < 20){//set to origin
      ultrasonic_read();//refresh value
      counterclockwise();
      Serial.print("returning...  ");
      Serial.println(d);
    }
    parar();
    delay(500);
     //restore and update iteration
    i=0;
    storedTime=currentTime;
    pos=0;
  }
}

void ultrasonic_read(){

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
}

void parar(){
  analogWrite(EN, 0); 
  digitalWrite(ina, LOW);
  digitalWrite(inb, LOW);
}
void clockwise(){
  analogWrite(EN, PWM); //speed control
  digitalWrite(ina, HIGH); //adelante ina = 1 out 1 = voltage
  digitalWrite(inb, LOW);
}
void counterclockwise(){
  analogWrite(EN, 70); //0-255
  digitalWrite(ina, LOW); //adelante ina = 1 out 1 = voltage
  digitalWrite(inb, HIGH);
}

void ISR_encoder(){
  if (digitalRead(B) == HIGH){
    pos--;
    }else{
    pos++;
    }
}
