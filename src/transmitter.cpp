#include <esp_now.h>
#include <WiFi.h>

// Endereço MAC do receptor
uint8_t broadcastAddress1[] = {0xA8, 0x42, 0xE3, 0xAE, 0xF7, 0x30};

// Definição da mensagem fixa
const char message[] = "input contact closed";

// Estrutura de dados para envio
typedef struct {
  char msg[32]; // Tamanho da mensagem ajustado para "input contact closed"
} message_struct;

message_struct msgToSend;
esp_now_peer_info_t peerInfo;

// Pino do botão
const int buttonPin = 14; // Botão no pino 14

// Variáveis para armazenar o estado do botão
bool buttonState = false;
bool lastButtonState = false;

// Callback quando os dados são enviados
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  // Configura o pino do botão como entrada (resistor de pull-up físico já conectado)
  pinMode(buttonPin, INPUT);

  // Configura o Wi-Fi no modo STA (Station)
  WiFi.mode(WIFI_STA);

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Registra o callback para quando os dados forem enviados
  esp_now_register_send_cb(OnDataSent);

  // Registra o receptor (peer)
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6); // Endereço MAC do receptor
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Preenche a mensagem fixa
  strncpy(msgToSend.msg, message, sizeof(msgToSend.msg));
}

void loop() {
  // Lê o estado do botão
  buttonState = digitalRead(buttonPin);

  // Verifica se houve uma mudança no estado do botão
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      // O botão foi pressionado
      Serial.println("Botão pressionado. Enviando mensagem...");

      // Envia a mensagem fixa via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *)&msgToSend, sizeof(msgToSend));
      if (result == ESP_OK) {
        Serial.println("Mensagem enviada com sucesso");
      } else {
        Serial.println("Erro ao enviar a mensagem");
      }
    }
    // Atualiza o último estado do botão
    lastButtonState = buttonState;
  }

  delay(50); // Debounce simples
}