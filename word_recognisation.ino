#include <Arduino.h>
#include <kuk22-project-1_inferencing.h>
#include <driver/i2s.h>

#define LED_PIN 2

#define I2S_WS   25
#define I2S_SCK  26
#define I2S_SD   34

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 16000

int16_t audio_buffer[BUFFER_SIZE];

void setupI2S() {
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    i2s_pin_config_t pins = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pins);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_PIN, OUTPUT);
    setupI2S();

    Serial.println("INMP441 + Edge Impulse ready");
}

void loop() {

    Serial.println("Recording...");

    int32_t sample;
    size_t bytesRead;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytesRead, portMAX_DELAY);

        int32_t s = sample >> 14;  // correct scaling
        audio_buffer[i] = (int16_t)s;
    }

    Serial.println("Running inference...");

    signal_t signal;
    signal.total_length = BUFFER_SIZE;
    signal.get_data = &get_audio_sample;

    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);

    if (err != EI_IMPULSE_OK) {
        Serial.println("Classifier error");
        return;
    }

    float max_val = 0;
    String label;

    for (int i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        float v = result.classification[i].value;

        Serial.print(ei_classifier_inferencing_categories[i]);
        Serial.print(": ");
        Serial.println(v);

        if (v > max_val) {
            max_val = v;
            label = ei_classifier_inferencing_categories[i];
        }
    }

    Serial.print("Detected: ");
    Serial.println(label);

    if (max_val > 0.7) {
        if (label == "on") digitalWrite(LED_PIN, HIGH);
        if (label == "off") digitalWrite(LED_PIN, LOW);
    }

    delay(300);
}

static int get_audio_sample(size_t offset, size_t length, float *out_ptr) {
    for (size_t i = 0; i < length; i++) {
        out_ptr[i] = audio_buffer[offset + i] / 32768.0f;
    }
    return 0;
}
