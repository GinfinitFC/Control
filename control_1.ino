//prototipo 1 planta control posicion/velocidad lineal
//Motor
int EN = 11;
int ina = 9;
int inb = 10;
const int maxSteps = 255;//motor internal steps
//encoder phases
int A = 2;
int B = 3;
unsigned char stateA;
unsigned char stateB;
unsigned char prevA;
//ultrasonico
const int Trigger = 4;   //Pin digital 2 para el Trigger del sensor
const int Echo = 5;   //Pin digital 3 para el Echo del sensor
volatile long t; //tiempo que demora en llegar el eco
volatile long d; //distancia en centimetros
//position variables
volatile int Ant = 0;
volatile int pos = 0;
volatile bool IsCW = true;
//time variables
const int timeTreshold = 5; // spin control can be modified
unsigned long currentTime;
unsigned long loopTime;//time taken on a spin

void setup() {
  // put your setup code here, to run once:
  //motor and encoder pin declaration
  pinMode(EN, OUTPUT);
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  //ultrasonic
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  //time
  currentTime = millis();
  loopTime = currentTime;

  Serial.begin(9600);

  //attachInterrupt(digitalPinToInterrupt(A),ISR_encoder,LOW);

  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  //inicializacion motor
  parar();
  delay(1000);
  clockwise();
  
  Serial.println("config done");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  ultrasonic_read();
  Serial.print("cm: ");
  Serial.println(d);
  if(d >= 20){
    clockwise();
    Serial.println("clockwise ");
  }
  if(d <= 5){
    counterclockwise();
    Serial.println("counterclockwise ");
  }
  delay(100);
}

void ultrasonic_read(){

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
}

void reset_pos(){
  pos = 0;
}

void parar(){
  analogWrite(EN, 0); 
  digitalWrite(ina, LOW);
  digitalWrite(inb, LOW);
}
void clockwise(){
  analogWrite(EN, 127); //max speed
  digitalWrite(ina, HIGH); //adelante ina = 1 out 1 = voltage
  digitalWrite(inb, LOW);
}
void counterclockwise(){
  analogWrite(EN, 127); //max speed
  digitalWrite(ina, LOW); //adelante ina = 1 out 1 = voltage
  digitalWrite(inb, HIGH);
}

/*void ISR_encoder(){
  if (digitalRead(B) == HIGH){
    pos--;
    }else{
    pos++;
    }
}*/

void encoder(){
  currentTime = millis();
  if(currentTime >= (loopTime + timeTreshold)){
    stateA = digitalRead(A);
    stateB = digitalRead(B);
    if(stateA != prevA){
      if(stateB){//clockwise
        IsCW = true;
        if(pos+1 <= maxSteps) pos++;
      }else{//counterclockwise
        IsCW = false;
        if(pos-1 <= maxSteps) pos--;
      }
    }
    prevA = stateA;//refresh states
    if(Ant != pos){
      Ant = pos;//refresh position
      Serial.print("Position: ");
      Serial.println(pos);
    }
    loopTime = currentTime;//refresh time
  }
}
