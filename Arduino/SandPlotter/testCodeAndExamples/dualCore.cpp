
TaskHandle_t Task1;

void codeForTask1( void * parameter )
{
    //Runs on Task 0
  for(;;){
    Serial.println("Hello");
    delay(1000);
    Serial.println("Bye");
    delay(1000);

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    codeForTask1,
    "led1Task",
    1000,
    NULL,
    1,
    &Task1,
    0);

    delay(1000); // needed to start-up task 1

    
}

void loop() {
    //Runs on Task 1
  // put your main code here, to run repeatedly:
    Serial.println("      Hello");
    delay(1000);
    Serial.println("      Bye");
    delay(1000);
}