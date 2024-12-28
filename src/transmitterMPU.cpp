#include <esp_now.h>
#include <WiFi.h>

#include "mpu.h"

// Endereço MAC do dispositivo receptor
uint8_t broadcastAddress[] = {0xA8, 0x42, 0xE3, 0xAE, 0xF7, 0x30}; // Substitua pelo MAC do receptor

// Estrutura para enviar dados do MPU6050
typedef struct {
    float yaw;    // Ângulo de guinada
    float pitch;  // Ângulo de inclinação
    float roll;   // Ângulo de rotação
    float accelX; // Aceleração no eixo X
    float accelY; // Aceleração no eixo Y
    float accelZ; // Aceleração no eixo Z
    float gyroX;  // Velocidade angular no eixo X
    float gyroY;  // Velocidade angular no eixo Y
    float gyroZ;  // Velocidade angular no eixo Z
} sensor_data_t;

sensor_data_t sensorData;

// Callback para envio
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Status do envio: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falha");
}

// Configuração do ESP-NOW
void setupEspNow() {
    WiFi.mode(WIFI_STA); // Modo Station
    if (esp_now_init() != ESP_OK) {
        Serial.println("Erro ao inicializar o ESP-NOW");
        return;
    }
    esp_now_register_send_cb(OnDataSent);

    // Adiciona receptor
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Erro ao adicionar receptor");
        return;
    }
}

void setup() {
    Serial.begin(115200);

    // Inicializa o MPU utilizando o código fornecido
    if (imu_setup(Wire)) {
        Serial.println("MPU inicializado com sucesso");
    } else {
        Serial.println("Erro ao inicializar o MPU");
        while (1); // Pausa se houver erro
    }

    // Configura o ESP-NOW
    setupEspNow();
}

void loop() {
    // Obtém leituras do sensor usando suas funções
    float *ypr = imu_get_ypr();
    float *accel = linear_acceleration();
    float *gyro = angular_velocity();

    // Preenche a estrutura com os dados obtidos
    sensorData.yaw = ypr[0] * 180 / M_PI; // Converte de radianos para graus
    sensorData.pitch = ypr[1] * 180 / M_PI;
    sensorData.roll = ypr[2] * 180 / M_PI;

    sensorData.accelX = accel[0];
    sensorData.accelY = accel[1];
    sensorData.accelZ = accel[2];

    sensorData.gyroX = gyro[0];
    sensorData.gyroY = gyro[1];
    sensorData.gyroZ = gyro[2];

    // Envia os dados via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sensorData, sizeof(sensorData));
    if (result == ESP_OK) {
        Serial.println("Dados enviados com sucesso!");
    } else {
        Serial.println("Erro ao enviar os dados");
    }

    delay(100); // Aguarda 100ms entre os envios
}
