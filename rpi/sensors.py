import numpy as np
from scipy.spatial.transform import Rotation
from time import sleep
from sense_hat import SenseHat
from gpiozero import CPUTemperature


g = 9.80665
sense = SenseHat()


def calc_acceleration_vector():
    raw_accel = list(sense.get_accelerometer_raw().values())
    pitch, roll, yaw = list(sense.get_gyroscope().values())
    r = Rotation.from_euler('ZYX', [yaw, pitch, roll], degrees=True)
    vec = r.apply(np.array(raw_accel))

    return vec


def calibrate_imu():
    n = 10
    t = 0.01

    s = np.zeros(3)
    for _ in range(n):
        s += calc_acceleration_vector()
        sleep(t)
    correction = s/n

    return correction


def get_imu():
    accel = calc_acceleration_vector()
    gyro = list(sense.get_gyroscope().values())
    compass = sense.get_compass()

    return accel, gyro, compass


def get_environmental():
    t = round(sense.get_temperature(), 4)
    p = round(sense.get_pressure(), 4)
    h = round(sense.get_humidity(), 4)
    cpu_t = round(CPUTemperature().temperature, 4)

    return {'t': t, 'p': p, 'h': h, 'cpu_t': cpu_t}
