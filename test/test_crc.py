import e2e.crc


# fmt: off
def test_calculate_crc8():
    # test check
    assert e2e.crc.CRC8_CHECK == e2e.crc.calculate_crc8(b"123456789")

    # test magic check
    assert 0x59 == e2e.crc.calculate_crc8(b"\x00\x00\x00\x00")
    assert (
        e2e.crc.CRC8_MAGIC_CHECK 
        == e2e.crc.calculate_crc8(b"\x00\x00\x00\x00\x59") ^ e2e.crc.CRC8_XOR_VALUE
    )

    # test different input types
    assert 0x59 == e2e.crc.calculate_crc8(bytearray(b"\x00\x00\x00\x00"))
    assert 0x59 == e2e.crc.calculate_crc8(b"\x00\x00\x00\x00")

    assert 0x37 == e2e.crc.calculate_crc8(b"\xF2\x01\x83")
    assert 0x79 == e2e.crc.calculate_crc8(b"\x0F\xAA\x00\x55")
    assert 0xB8 == e2e.crc.calculate_crc8(b"\x00\xFF\x55\x11")
    assert 0xCB == e2e.crc.calculate_crc8(b"\x33\x22\x55\xAA\xBB\xCC\xDD\xEE\xFF")
    assert 0x8C == e2e.crc.calculate_crc8(b"\x92\x6B\x55")
    assert 0x74 == e2e.crc.calculate_crc8(b"\xFF\xFF\xFF\xFF")

    # test start_value and first_call
    first_call = True
    crc = e2e.crc.CRC8_INITIAL_VALUE
    data = b"\x33\x22\x55\xAA\xBB\xCC\xDD\xEE\xFF"
    for i, _val in enumerate(data):
        crc = e2e.crc.calculate_crc8(data[i:i+1], start_value=crc, first_call=first_call)
        first_call = False
    assert 0xCB == crc

# fmt: on
