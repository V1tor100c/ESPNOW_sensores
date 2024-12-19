
## Transmitter code 
This code sets up an ESP32 as a transmitter using ESP-NOW. It monitors a button connected to GPIO 14, which has an external pull-up resistor. When the button is pressed (contact closed), the ESP32 sends a fixed message, "input contact closed," to a specified receiver device via ESP-NOW.

## Receiver code
This code sets up an ESP32 receiver using ESP-NOW to receive messages. When the message "input contact closed" is received, the built-in LED on GPIO2 is turned on for 1 second. The code also displays the sender's MAC address and the received message on the Serial Monitor.

## Transmitter PCB
Easy EDA Link: https://oshwlab.com/samuelsilveira14/espnow_receiver

* The receiver doesn't require a pcb, aside from the esp32. 