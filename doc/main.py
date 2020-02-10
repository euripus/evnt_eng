import math, random
from PIL import Image

def num_points_for_cell(point_probs):
    r = random.random()

    for i in range(10):
        if r < point_probs[i]:
            return i

    return i

def distance(p1, p2, maxDist):  # L2 distance function
    dx = abs(p1[0] - p2[0])
    dy = abs(p1[1] - p2[1])

    if dx > 0.5:
        dx = 1 - dx

    if dy > 0.5:
        dy = 1 - dy

    d = math.sqrt(dx * dx + dy * dy) / maxDist

    if d > 1:
        d = 1

    return d

def main():
    width = 1024   # image size
    height = width
    cellSize = 8   # 8 cells per width(height)
    density = 4.0  # point density
    maxDist = 0.11
    k1 = 1.0 / float(cellSize)
    k2 = 1.0 / float(cellSize)

    # compute probabilities for various
    # numbers of points
    point_probs = []
    mFact = 1.0   # m!
    lPower = 1.0  # pow ( density, m )
    s = 0.0

    for m in range(10):
        p = lPower * math.exp(-density) / mFact
        s = s + p
        mFact = mFact * (m + 1)
        lPower = lPower * density
        point_probs.append(s)

    im = Image.new("RGB", (width, height))
    points = []

    # generate random points
    cells = []
    for i1 in range(cellSize):
        cells.append([])
        for i2 in range(cellSize):
            cells[i1].append([])
            x1 = i1 * k1
            y1 = i2 * k2
            numPoints = num_points_for_cell(point_probs)

            for i3 in range(numPoints):
                x = x1 + k1 * random.random()
                y = y1 + k2 * random.random()
                points.append((x, y))
                cells[i1][i2].append((x, y))

    # create textures F1, F2, F3, F4
    for x in range(width):
        for y in range(height):
            d = []
            pt = (float(x) / float(width), float(y) / float(height))
            # cell of this point
            i1 = int((x * cellSize) / width)
            j1 = int((y * cellSize) / height)

            cellList = [(i1, j1)]  # create candidate list of cells

            for i in range(-1, 2):  # add every neighbouring cell
                for j in range(-1, 2):
                    i2 = i1 + i
                    j2 = j1 + j

                    if i2 < 0:
                        i2 = cellSize + i2

                    if i2 >= cellSize:
                        i2 = i2 - cellSize

                    if j2 < 0:
                        j2 = cellSize + j2

                    if j2 >= cellSize:
                        j2 = j2 - cellSize

                    cellList.append((i2, j2))

            for (i, j) in cellList:
                for p in cells[i][j]:
                    d.append(distance(p, pt, maxDist))

            d.sort()

            # now take four closest distances and map to colors
            # excluding duplicates
            fs = []
            j = 0

            for i in range(4):
                if j >= len(d):
                    j = len(d) - 1

                v = d[j]
                j = j + 1

                fs.append(v)
                while j < len(d) and d[j] == v:
                    j = j + 1

            red = int(255 * fs[0] + 0.5)
            green = int(255 * fs[1] + 0.5)
            blue = int(255 * fs[2] + 0.5)

            im.putpixel((x, y), (red, green, blue))

    im.save("celltex.png", "png")

if __name__ == '__main__':
    main()
