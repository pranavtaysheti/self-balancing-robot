import serial
from statistics import variance
from typing import cast


COM_PORT = "com3"
BAUD_RATE = 115200
LINES = 1000

VAR_ACC = 0.03
VAR_GYRO = 0.5


class DataError(ValueError):
    def __init__(self, avg_data: list[float]):
        self.avg_data = avg_data


class HighVarianceError(DataError): ...


class NonAlignedAxisError(DataError): ...


class Data:
    """data
    0: -ax
    1: +ax
    2: -ay
    3: +ay
    4: -az
    5: +az
    """

    _data: list[list[float] | None]

    def __init__(self):
        self._data = [None] * 6

    def collect(self, axis: int, line: list[float]):
        self._data[axis] = line

    def is_collected(self, axis: int):
        return not self._data[axis] is None

    def is_complete(self):
        return None not in self._data

    def summary(self):
        assert None not in self._data
        data = cast(list[list[float]], self._data)

        return f"""
        Axis X Scale: {(data[1][0] - data[0][0])/2}\n
        Axis X Bias: {(data[1][0] + data[0][0])/2}\n
        Axis Y Scale: {(data[3][1] - data[2][1])/2}\n
        Axis Y Bias: {(data[3][1] + data[2][1])/2}\n
        Axis Z Scale: {(data[5][2] - data[4][2])/2}\n
        Axis Z Bias: {(data[5][2] + data[4][2])/2}\n
        """


class Reading:
    _raw_data: list[list[float]]

    def __init__(self):
        self._raw_data = []

    def append(self, line: list[float]):
        self._raw_data.append(line)

    def __len__(self):
        return len(self._raw_data)

    def _average(self) -> list[float]:
        sum_data = [0.0] * 6
        for l in self._raw_data:
            for i, n in enumerate(l):
                sum_data[i] += n

        return [n / len(self._raw_data) for n in sum_data]

    @staticmethod
    def _axis(avg_data: list[float]) -> int:
        ver_axis = max(range(3), key=lambda i: abs(avg_data[i]))

        for i, v in enumerate(avg_data[:3]):
            if i == ver_axis:
                if not (0.85 < abs(v) < 1.15):
                    raise NonAlignedAxisError(avg_data)
            else:
                if abs(v) >= 0.15:
                    raise NonAlignedAxisError(avg_data)

        return 2 * ver_axis + (avg_data[ver_axis] > 0)

    def process(self) -> tuple[int, list[float]]:
        avg_data = self._average()
        for axis in range(6):
            series = [r[axis] for r in self._raw_data]
            if (axis < 3 and variance(series) > VAR_ACC) or (
                axis >= 3 and variance(series) > VAR_GYRO
            ):
                raise HighVarianceError(avg_data)

        return (self._axis(avg_data), avg_data)


def get_data() -> Reading:
    with serial.Serial() as ser:
        ser.baudrate = BAUD_RATE
        ser.port = COM_PORT
        ser.open()

        # Tell Microcontroller to start its calibration routine
        ser.write(b"READY\n")

        # Get data
        curr = Reading()
        for _ in range(LINES):
            line = [float(n) for n in ser.readline().decode().strip().split()]
            curr.append(line)

    return curr


data = Data()
while not data.is_complete():
    input("Please change the orientation.")

    # Process Data
    processed = False
    while not processed:
        input("Press Enter when Ready!")
        curr = get_data()
        print(f"Read {len(curr)} lines successfully.")

        try:
            axis, avg_data = curr.process()

        except DataError as e:
            print(f"Recorded Average: {e.avg_data}")

            if isinstance(e, HighVarianceError):
                print("Data variance is too high. Make sure IMU is stable.")

            elif isinstance(e, NonAlignedAxisError):
                print("Make sure IMU is aligned closely with an axis.")

        else:
            data.collect(axis, avg_data)
            processed = True

print(data.summary())
