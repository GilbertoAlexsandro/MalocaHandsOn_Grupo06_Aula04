#define TRIGGER_PIN 13     // Pino Trigger do sensor HC-SR04
#define ECHO_PIN 12        // Pino Echo do sensor HC-SR04
#define BUTTON_PIN 14      // Pino do botão
#define PIN_BUZZER 2       // Pino do buzzer
#define PIN_PIR 23         // Pino do sensor PIR

long duration;            // Variável para armazenar a duração do pulso
int distancia;            // Variável para armazenar a distância calculada
int contador = 0;         // Contador para registrar o número de usos do dispenser
int distanciaLimite = 20; // Distância limite para detectar proximidade (em cm)

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Configura os pinos como entrada ou saída
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PIR, INPUT);

  Serial.println("Sistema de Monitoramento de Dispenser Iniciado");
}

void loop() {
  // Verifica se o sensor PIR detectou movimento
  if (digitalRead(PIN_PIR) == HIGH) {
    Serial.println("Movimento detectado!");

    // Gera um pulso de 10 microssegundos no pino Trigger para iniciar a medição do HC-SR04
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Lê a duração do pulso no pino Echo
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calcula a distância em centímetros
    distancia = duration * 0.034 / 2;

    // Verifica a distância
    if (distancia > 0 && distancia <= distanciaLimite) {
      tone(PIN_BUZZER, 3000);  
      Serial.print("Pessoa detectada a ");
      Serial.print(distancia);
      Serial.println(" cm do dispenser"); 
      // Verifica se o botão foi pressionado para simular o uso do dispenser
      if (digitalRead(BUTTON_PIN) == LOW) { // LOW indica que o botão foi pressionado
        tone(PIN_BUZZER, 2 );  
        delay(500); // debounce
        if (digitalRead(BUTTON_PIN) == LOW) {
          contador++; // Incrementa o contador de usos do dispenser
          Serial.print("Dispenser utilizado. Total de usos: ");
          Serial.println(contador);5
          
          while (digitalRead(BUTTON_PIN) == LOW); // Aguarda até que o botão seja solto
        }
        tone(PIN_BUZZER, LOW);  
      }
    } else {
      tone(PIN_BUZZER, LOW);  
      Serial.println("Nada está próximo ao dispenser");
    }
  } else {
    tone(PIN_BUZZER, LOW);  
    Serial.println("Nenhum movimento detectado");
  }

  delay(100); // Aguarda um pouco antes da próxima verificação
}