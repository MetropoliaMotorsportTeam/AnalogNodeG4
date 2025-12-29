Analog Node are used to convert analog sensor data such as potentiometers, thermistors and buttons into meaningful CAN bus data.

<img width="797" height="172" alt="AnalogNode drawio" src="https://github.com/user-attachments/assets/beddd99b-b599-4a6a-aafb-c8af21c5f1d9" />

1. Sensor. An analog sensor.
2. ADC. Analog to Digital converter. All drivers are provided by HAL library.
3. Transfer function. Takes digital signal and converts into meaningful numbers, for example into temperature in C for thermistors and steering angle degree.
