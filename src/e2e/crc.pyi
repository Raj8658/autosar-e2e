import typing

def calculate_crc8(
    data: bytes, start_value: int = 0xFF, first_call: bool = True
) -> int: ...

CRC8_INITIAL_VALUE: typing.Final[int]
CRC8_XOR_VALUE: typing.Final[int]
CRC8_CHECK: typing.Final[int]
CRC8_MAGIC_CHECK: typing.Final[int]

