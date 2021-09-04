//Motor
int EN = 11;
int ina = 9;
int inb = 10;
//encoder phases
int A = 2;
int B = 3;
//position variables
int Ant = 0;
volatile int pos = 0;

void setup() {
  // put your setup code here, to run once:
  //motor and encoder pin declaration
  pinMode(EN, OUTPUT);
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  pinMode(A, INPUT);
  pinMode(B, INPUT);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(A),encoder,LOW);

  digitalWrite(EN, 255); //mitad de velocidad
  digitalWrite(ina, HIGH); //adelante ina = 1 out 1 = voltage
  digitalWrite(inb, LOW);

  Serial.println("config done");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(pos != Ant){
    Serial.println(pos);
    Ant=pos;
    }
}

void encoder(){
  if (digitalRead(B) == HIGH){
    pos--;
    }else{
    pos++;
    }
}
