# Receiver-PPM-ESP32-Second-Core
I have tested the ppm waveshape of flysky fs iA-10B receiver binded with flysky fs-i6.
The waveshape is in the ppm_waveshape folder.

# Note
1. The time period of one ppm data set is 20ms.
2. So, the data frequency is 50 Hz,.
3. PPM signal is decoded using the interrupt and micros().
4. Can be implemented in flight-controlelrs
