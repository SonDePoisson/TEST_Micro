#include <Arduino.h>
#include <driver/i2s.h>

// Connections to INMP441 I2S microphone
#define I2S_WS  25   // GPIO pin for Word Select signal
#define I2S_SD  33   // GPIO pin for Serial Data signal
#define I2S_SCK 32  // GPIO pin for Serial Clock signal

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length
#define bufferLen 1024                      // Number of buffers
int16_t buffer1[bufferLen];               // Buffer to store the audio samples                   // Index of the current buffer being filled

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),                       // I2S mode: Master, Receive
    .sample_rate = 8000,                                                    // Sample rate
    .bits_per_sample = i2s_bits_per_sample_t(16),                            // Bits per sample
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,                             // Channel format: Only left channel
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),    // Communication format: Standard I2S
    .intr_alloc_flags = 0,                                                   // Interrupt allocation flags
    .dma_buf_count = 8,                                                      // DMA buffer count
    .dma_buf_len = bufferLen,                                                // Length of each DMA buffer
    .use_apll = false                                                        // Use APLL for master clock
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);   // Install and configure the I2S driver
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,                // Serial Clock pin
    .ws_io_num = I2S_WS,                  // Word Select pin
    .data_out_num = -1,                   // Data output pin (not used in this case)
    .data_in_num = I2S_SD                 // Serial Data pin
  };

  i2s_set_pin(I2S_PORT, &pin_config);     // Set the I2S pin configuration
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);                   // Initialize the Serial Monitor with baud rate 115200
  Serial.println(".");                    // Print a blank line

  delay(1000);   // Delay for 1 second

  // Set up I2S
  i2s_install();             // Install and configure the I2S driver
  i2s_setpin();              // Set the I2S pin configuration
  i2s_start(I2S_PORT);       // Start the I2S driver
  delay(500);                // Delay for 500 milliseconds
}

  void loop() 
{
  size_t bytesIn = 0;

  // Read data from I2S into buffer 1
  i2s_read(I2S_PORT, buffer1, bufferLen*2, &bytesIn, portMAX_DELAY);
  Serial.write((uint8_t *)buffer1, bytesIn);
  // for (int i = 0; i < bufferLen; i++) {
  //   Serial.println(buffer1[i]);
  // }  
}
  