# MX106R и Arduino

Для подключения MX106R к Arduino необходимо использовать конвертер RS485 to TTL. Распиновка MX160R: GND, VCC, A, B. А и В подключается к конвертеру. Подключение к Arduino: RSE -> 2 (digital port), DI -> RX, RO -> TX, GND -> GND, VCC -> 3,3V. Рекомендуемое напряжение для MX106R: 12,4 В. GND лучше сделать общей. 