#include <Arduino.h>
#include "Zigbee.h"

#define ZIGBEE_ENDPOINT 1

ZigbeeThermostat zbThermostat(ZIGBEE_ENDPOINT);

void receiveTemperature(float temperature)
{
    log_i("Temperature received: %.2f °C", temperature);
}

void receiveHumidity(float humidity)
{
    log_i("Humidity received: %.2f %%", humidity);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    log_i("ESP32-C6 Zigbee Thermostat starting");

    zbThermostat.onTempReceive(receiveTemperature);
    zbThermostat.onHumidityReceive(receiveHumidity);

    zbThermostat.setManufacturerAndModel(
        "DIY Smart Room",
        "ESP32-C6 Thermostat"
    );

    zbThermostat.allowMultipleBinding(true);

    Zigbee.addEndpoint(&zbThermostat);

    log_i("Starting Zigbee");

    if (!Zigbee.begin(ZIGBEE_ROUTER))
    {
        log_e("Failed to start Zigbee");
        ESP.restart();
    }

    log_i("Waiting for Zigbee network");

    while (!Zigbee.connected())
    {
        delay(100);
    }

    log_i("Connected to Zigbee network");
    log_i("Waiting for temperature sensor to join or rejoin");
}

void loop() {}