
int timer = 100;
int ledPins[] = {2,3,4,5,6,7};
int pinCount = 6;
int repeatCount;
int alarmPin = 11;

const int trigPin = 9;
const int echoPin = 10;

long duration1;
long duration2;
int distance1;
int distance2;
int distanceDiff;

void setup()
  {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(alarmPin,OUTPUT);

  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin],OUTPUT);
  }

  Serial.begin(9600);
  }

void loop()
  {
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);

    duration1 = pulseIn(echoPin, HIGH);
    distance1 = duration1*0.034/2;

    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    
    duration2 = pulseIn(echoPin, HIGH);
    distance2 = duration2*0.034/2;
    
    distanceDiff = distance1 - distance2;

    if(distanceDiff > 50)
      {
        repeatCount = 0;
        while(repeatCount < 5)
          {
          for(int thisPin = 0; thisPin < pinCount; thisPin++)
            {
              digitalWrite(ledPins[thisPin],HIGH);
              digitalWrite(alarmPin,HIGH);
              delay(timer);
              digitalWrite(ledPins[thisPin],LOW);
              delay(timer);
              digitalWrite(alarmPin,LOW);
            }
          repeatCount++;
          }
      }

  Serial.print("Distance (cm): ");
  Serial.println(distanceDiff);
  }
