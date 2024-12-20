#include <WiFi.h>  // Biblioteca necessária para manipular o WiFi e obter o endereço MAC

void setup() {
  // Inicia a comunicação serial
  Serial.begin(115200);

  // Inicializa o WiFi em modo estação (STA) para obter o endereço MAC
  WiFi.mode(WIFI_MODE_STA);

  // Obtém o endereço MAC como um array de bytes
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);

  // Imprime o endereço MAC formatado
  Serial.print("Endereço MAC do ESP32: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void loop() {
  // Não há nada a ser feito no loop
}