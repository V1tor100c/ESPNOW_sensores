#include <VL53L0X.h> // Inclui a biblioteca VL53L0X para sensores de distância

// Definindo os pinos para controle dos sensores
#define SDIST_1 25
#define SDIST_2 23
#define SDIST_3 13

class VL53_sensors
{
private:
    // Aqui podem ser adicionados membros privados, se necessário

public:
    // Configuração dos sensores
    int number_sensor = 2; // Número de sensores ativos
    int x_shut_pins[3] = { SDIST_1, SDIST_2, SDIST_3 }; // Pinos para controle dos sensores
    int dist[3]; // Array para armazenar as distâncias lidas
    int test = 0; // Sensor usado para teste
    int distTest; // Distância lida durante o teste

    // Array de sensores
    VL53L0X sensor[3];

    // Métodos públicos
    void sensorsInit(); // Inicializa os sensores
    void distanceRead(); // Lê as distâncias dos sensores
    void printDistances(); // Imprime as distâncias no Serial Monitor
    void testRead(); // Realiza uma leitura de teste de um sensor específico
};


void VL53_sensors::sensorsInit() {
    // Inicializa a comunicação I2C com os sensores

    // Configuração dos pinos para inicialização dos sensores
    for (uint8_t i = 0; i < number_sensor; i++){
        pinMode(x_shut_pins[i], OUTPUT); // Configura o pino como saída
        digitalWrite(x_shut_pins[i], LOW); // Define o pino para LOW para desligar o sensor
    }

    // Inicializa os sensores
    for (uint8_t i = 0; i < number_sensor; i++){
        pinMode(x_shut_pins[i], INPUT); // Configura o pino como entrada para habilitar o sensor
        sensor[i].init(true); // Inicializa o sensor
        sensor[i].setAddress((uint8_t)0x21 + i); // Define o endereço I2C do sensor
        sensor[i].setTimeout(50); // Define o timeout para a leitura
    }
}

void VL53_sensors::distanceRead() {
    // Lê as distâncias de todos os sensores
    for (uint8_t i = 0; i < number_sensor; i++) {
        dist[i] = sensor[i].readRangeSingleMillimeters(); // Lê a distância em milímetros
    }
}

void VL53_sensors::printDistances() {
    // Imprime as distâncias lidas no Serial Monitor
    for (uint8_t i = 0; i < number_sensor; i++){
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(dist[i]);
        Serial.println(" mm");
    }
}

void VL53_sensors::testRead() {
    // Realiza uma leitura de teste do sensor especificado por `test`
    distTest = sensor[test].readRangeSingleMillimeters(); // Lê a distância em milímetros
    // Serial.print("Leitura do Sensor ");
    // Serial.print(test);
    // Serial.print(": ");
    // Serial.println(distTest);
}
