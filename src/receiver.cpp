#include <esp_now.h>
#include <WiFi.h>

// Tamanho máximo esperado para a mensagem recebida
#define MAX_MSG_SIZE 32

// Buffer para armazenar a mensagem recebida
char receivedMessage[MAX_MSG_SIZE];

// Pino do LED incorporado (GPIO2)
const int ledPin = 2;

// Função de callback executada quando os dados são recebidos
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Copia os dados recebidos para o buffer
  memset(receivedMessage, 0, MAX_MSG_SIZE); // Limpa o buffer
  memcpy(receivedMessage, incomingData, len); // Copia os dados para o buffer
  
  // Exibe o endereço MAC do remetente
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("Message from: ");
  Serial.println(macStr);

  // Exibe a mensagem recebida
  Serial.print("Message: ");
  Serial.println(receivedMessage);

  // Verifica se a mensagem recebida é "input contact closed"
  if (strcmp(receivedMessage, "input contact closed") == 0) {
    // Liga o LED incorporado por 1 segundo
    digitalWrite(ledPin, HIGH);  // Acende o LED
    delay(1000);                  // Aguarda 1 segundo
    digitalWrite(ledPin, LOW);   // Apaga o LED
  }
}

void setup() {
  // Inicializa o Monitor Serial
  Serial.begin(115200);
  
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);

  // Configura o dispositivo como Station
  WiFi.mode(WIFI_STA);

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Registra o callback para receber os dados
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW Receiver Initialized");
}

void loop() {
  // O loop principal não faz nada, pois o callback é chamado automaticamente
}
