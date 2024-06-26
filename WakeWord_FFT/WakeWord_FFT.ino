#include <fix_fft.h>

const int analogSensorPin = 26;
const int ledPin = LED_BUILTIN;
const int sampleRate = 1028;
const int sampleTime = 1;
const int totalSamples = sampleRate * sampleTime;

int16_t vReal[totalSamples];
int16_t vImag[totalSamples];

unsigned long startTime;

void fft_wrapper(int16_t* vReal, int16_t* vImag, int n, int inverse) {
    fix_fft((char*)vReal, (char*)vImag, n, inverse);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //Serial.print("fft calculate");
  digitalWrite(ledPin, HIGH);
  startTime = millis();

  for (int i = 0; i < totalSamples; i++) {
    vReal[i] = analogRead(analogSensorPin);
    vImag[i] = 0; // 허수부는 0으로 초기화
    while (millis() < startTime + (i * 1000.0 / sampleRate));
  }

  digitalWrite(ledPin, LOW);

  // FFT 계산
  fft_wrapper(vReal, vImag, 10, 0);

  // FFT 결과 출력
  for (int i = 0; i < totalSamples / 2; i++) {
    double frequency = (i * 1.0 * sampleRate) / totalSamples;
    double magnitude = sqrt(vReal[i] * vReal[i] + vImag[i] * vImag[i]);
    //Serial.print(frequency);
    //Serial.print(",");
    Serial.println(magnitude);
  }

  delay(2000);
}