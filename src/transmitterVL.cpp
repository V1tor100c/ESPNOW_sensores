#include <esp_now.h>
#include <WiFi.h>
#include "VL53_sensors.h"

// Endereço MAC do receptor
uint8_t broadcastAddress1[] = {0xA8, 0x42, 0xE3, 0xAE, 0xF7, 0x30}; //TROCAR SEMPRE QUE USAR OUTRO ESP32!!!

// Definição da mensagem fixa
const char message[] = "input contact closed";

// Estrutura de dados para envio
typedef struct {
  float distance; // Valor numérico da leitura do sensor
} message_struct;


message_struct msgToSend;
esp_now_peer_info_t peerInfo;

VL53_sensors sens_dist;
int dist;

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
  // strncpy(msgToSend.msg, message, sizeof(msgToSend.msg));

  sens_dist.sensorsInit();
}

void loop() {
  
  dist = sens_dist.distTest;
  
  // Envia a mensagem fixa via ESP-NOW
msgToSend.distance = dist; // Preenche a estrutura com a leitura

  // Envia a leitura via ESP-NOW
  esp_err_t result = esp_now_send(0, (uint8_t *)&msgToSend, sizeof(msgToSend));
  if (result == ESP_OK) {
    Serial.printf("Leitura enviada com sucesso: %.2f mm\n", dist);
  } else {
    Serial.println("Erro ao enviar a leitura");
  }

  delay(50); // Debounce simples
}