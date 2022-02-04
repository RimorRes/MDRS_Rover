from sense_hat import SenseHat
from gpiozero import CPUTemperature


sense = SenseHat()


def get_raw_imu():
    accel = sense.get_accelerometer_raw()
    gyro = sense.get_gyroscope_raw()
    compass = sense.get_compass_raw()

    return accel, gyro, compass


def get_environmental():
    t = round(sense.get_temperature(), 4)
    p = round(sense.get_pressure(), 4)
    h = round(sense.get_humidity(), 4)
    cpu_t = round(CPUTemperature().temperature, 4)

    return {'t': t, 'p': p, 'h': h, 'cpu_t': cpu_t}
