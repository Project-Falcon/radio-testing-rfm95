#include <Arduino.h>

#define COMMAND_BUFFER_SIZE 8
#define DATA_BUFFER_SIZE 256

char cmd[COMMAND_BUFFER_SIZE], data[DATA_BUFFER_SIZE];
int data_len;

int freq = 868, spread = 69, bandwidth = 100, power = 23;

int set_config() {
    float val = Serial.parseFloat();

    Serial.readBytesUntil('\n', data, DATA_BUFFER_SIZE);
    if (strcmp(cmd, "FREQ") == 0) {
        freq = val;

        Serial.print("set freq to");
        Serial.println(val);
    }

    if (strcmp(cmd, "BWIDTH") == 0) {
        bandwidth = val;

        Serial.print("set bandwidth to");
        Serial.println(val);
    }

    if (strcmp(cmd, "SPREAD") == 0) {
        spread = val;

        Serial.print("set spread to");
        Serial.println(val);
    }

    if (strcmp(cmd, "POWER") == 0) {
        power = val;

        Serial.print("set tx power to");
        Serial.println(val);
    }


    return 0;
}

int get_config() {
    Serial.readBytesUntil('\n', data, DATA_BUFFER_SIZE);
    if (strcmp(cmd, "FREQ") == 0) {
        Serial.print("freq is");
        Serial.println(freq);
    }

    if (strcmp(cmd, "BWIDTH") == 0) {
        Serial.print("bandwidth is");
        Serial.println(bandwidth);
    }

    if (strcmp(cmd, "SPREAD") == 0) {
        Serial.print("spread is");
        Serial.println(spread);
    }

    if (strcmp(cmd, "POWER") == 0) {
        Serial.print("tx power is");
        Serial.println(power);
    }

    return 0;
}

int parse_packet() {
    Serial.readBytesUntil('\n', data, DATA_BUFFER_SIZE);
    if (strcmp(cmd, "PACKET") == 0) Serial.println(data);

    return 0;
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

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(100);
}

void loop() {
    if (Serial.available() == 0) return;

    handle_command();
    // forward_packets();
}