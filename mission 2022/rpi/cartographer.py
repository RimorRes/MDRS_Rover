import csv
from datetime import datetime


def create_csv_file(file_path):
    """
    Creates output file for the
    recorded data

    args:
        file path: Path object
    """

    with open(file_path, 'w', newline='') as f:
        writer = csv.writer(f)
        # Write headers
        writer.writerow(
            (
                'Time',
                'GPS',
                'Accelerometer',
                'Gyroscope',
                'Magnetometer',
                'Wind speed',
                'Wind direction',
                'Temp (C)',
                'Pressure (hPa)',
                'Humidity (%),
            )
        )

def store_to_csv(file_path, values):
    """
    Stores the data in a csv file.

    args:
        file path: Path object
        data: list
    """

    with open(file_path, 'a', newline='') as f:
        writer = csv.writer(f)
        # Write data to row
        writer.writerow([datetime.now()] + values)
        f.close()
