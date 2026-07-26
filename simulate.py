linear = [
    -1000,
    -833,
    -667,
    -500,
    -333,
    -167,
    0,
    0,
    77,
    154,
    231,
    308,
    385,
    462,
    538,
    615,
    692,
    769,
    846,
    923,
    1000,
]
parabolic = [
    -1000,
    -694,
    -444,
    -250,
    -111,
    -28,
    0,
    0,
    6,
    24,
    53,
    95,
    148,
    213,
    290,
    379,
    479,
    592,
    716,
    852,
    1000,
]


used_arr = parabolic

while 1:
    p = int(input("Pedal value: "))
    if p == 1000:
        print("1000")
        continue
    step = 50
    idx = p // step
    remainder = p % step
    y0 = used_arr[idx]
    y1 = used_arr[idx + 1]
    delta = y1 - y0
    y = y0 + (delta * remainder // step)
    print(f"Output: {y}")
