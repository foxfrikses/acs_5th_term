count = 10000000
with open('input.txt', 'w') as f:
    f.write(str(count) + ' ')
    for _ in range(count//2):
        f.write(str(1) + ' ')

    for _ in range(count-count//2):
        f.write(str(2) + ' ')

    for _ in range(count//2):
        f.write(str(3) + ' ')

    for _ in range(count-count//2):
        f.write(str(4) + ' ')

