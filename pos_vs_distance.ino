//this code will compare the position reached from the encoder vs the position read from the ultrasonic
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
//speed control enable is bridged in Hbridge
//time variables
volatile int i=0;

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

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(A),ISR_encoder,RISING);

  digitalWrite(Trigger, LOW);
  
  while(d < 20){//set to origin
    ultrasonic_read();//refresh value
    counterclockwise();
  }
  //motor initialization
  parar();
  pos=0;
  delay(1000);
  Serial.println("# Pos Distance Time");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  ultrasonic_read();//as flag
  clockwise();
  //data print routine
  Serial.print(i);
  Serial.print(" ");
  Serial.print(pos);
  Serial.print(" ");
  Serial.print(d);
  Serial.print(" ");
  Serial.println(i*0.02);
  i++;
  //Serial.print("cm: ");
  //Serial.println(d);
  if(d <= 4){//if near end
    parar();
    delay(100000);
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

void parar(){
  analogWrite(EN, 0); 
  digitalWrite(ina, LOW);
  digitalWrite(inb, LOW);
}
void clockwise(){
  analogWrite(EN, 255); //speed controling
  digitalWrite(ina, HIGH); //adelante ina = 1 out 1 = voltage
  digitalWrite(inb, LOW);
}
void counterclockwise(){
  analogWrite(EN, 255); //0-255
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
