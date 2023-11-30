#include <FalconLoRa.h>

#define RADIO_DRIVER_VERSION "0.1"

#define COMMAND_BUFFER_SIZE 8
#define DATA_BUFFER_SIZE 256

falcon::lora_rx lora;

char cmd[COMMAND_BUFFER_SIZE], data[DATA_BUFFER_SIZE], packet[FALCON_MAX_DATA_LEN];
int data_len;

int freq = 868, spread = 69, bandwidth = 100, power = 23;

int set_config() {
    float val = Serial.parseFloat();

    Serial.readBytesUntil('\n', data, DATA_BUFFER_SIZE);
    if (strcmp(cmd, "FREQ") == 0) {
        freq = val;
        lora.set_frequency(freq);

            Serial.println("OK");

        return 0;
    }

    if (strcmp(cmd, "BWIDTH") == 0) {
        bandwidth = val;
        lora.set_bandwidth(bandwidth);

            Serial.println("OK");

        return 0;
    }

    if (strcmp(cmd, "SPREAD") == 0) {
        spread = val;
        lora.set_spread(spread);

            Serial.println("OK");

        return 0;
    }

    if (strcmp(cmd, "POWER") == 0) {
        power = val;
        lora.set_power(power);

            Serial.println("OK");

        return 0;
    }

        Serial.println("ERROR");

    return -1;
}

int get_config() {
    Serial.readBytesUntil('\n', data, DATA_BUFFER_SIZE);

    if (strcmp(cmd, "REV") == 0) {
        Serial.println(RADIO_DRIVER_VERSION);
    }

    if (strcmp(cmd, "FREQ") == 0) {
            
            Serial.println(freq);
        
        return 0;
    }

    if (strcmp(cmd, "BWIDTH") == 0) {
            
            Serial.println(bandwidth);
        
        return 0;
    }

    if (strcmp(cmd, "SPREAD") == 0) {
            
            Serial.println(spread);

        return 0;
    }

    if (strcmp(cmd, "POWER") == 0) {
        
            Serial.println(power);

        return 0;
    }

        Serial.println("ERROR");

    return -1;
}

int parse_packet() {
    Serial.readBytesUntil('\n', data, DATA_BUFFER_SIZE);
    if (strcmp(cmd, "PACKET") == 0) {
        if (lora.send(cmd) == FALCON_OK) {
            
                Serial.println("OK");
            
            return 0;
        } else {
                
                Serial.println("ERROR");

            return -1;
        }
    }
};


int handle_command() {
    memset(cmd, 0, COMMAND_BUFFER_SIZE);
    memset(data, 0, DATA_BUFFER_SIZE);

    char c = Serial.read();

    Serial.readBytesUntil('=', cmd, COMMAND_BUFFER_SIZE);

    switch (c) {
        case '!': return set_config();
        case '?':  return get_config();
        case '+': return parse_packet();
        default: 
            Serial.readBytesUntil('=', cmd, COMMAND_BUFFER_SIZE);
            Serial.println("ERROR");
    }

    return -1;
};

void forward_packets() {
    memset(packet, 0, FALCON_MAX_DATA_LEN);

    if (lora.recv(packet) == FALCON_OK) {
        Serial.println(packet);
    }
}

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(100);
}

void loop() {
    forward_packets();
    if (Serial.available() > 0) handle_command();
    delay(10);
}