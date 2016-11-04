#!/usr/bin/python3

#import math
import sys

with open('grid-5x5.txt') as file:
    grid_data = [[(char == 'B') for char in line.strip()]
                 for line in file.readlines()]
grid = [x for row in grid_data for x in row]

def dump_bool(flat):
    for i in range(5):
        row = map(lambda x: ('*' if x else '.'), flat[5*i:5*(i+1)])
        print("".join(row))
    print()

def ix(p):
    return p[0]+5*p[1]

def neighbors(p):
    (x,y) = p
    if y > 0:
        yield (x,y-1)
    if x < 4:
        yield (x+1,y)
    if y < 4:
        yield (x,y+1)
    if x > 0:
        yield (x-1,y)

def generate_blobs(points, blob, forbidden, size):
    size -= 1
    points_iter = iter(points.keys())  # can't iterate over modified dict
    points = dict(points)  # backtracking directly is too complex, so push/pop
    forbidden = list(forbidden)
    for p in points_iter:
        for q in neighbors(p):
            iq = ix(q)
            if not forbidden[iq]:
                blob[iq] = True
                forbidden[iq] = True
                if size > 0:
                    q_neighbors = 0
                    for r in neighbors(q):
                        if not forbidden[ix(r)]:
                            q_neighbors += 1
                        if r in points:
                            if points[r] > 1:
                                points[r] -= 1
                            else:
                                del points[r]
                    if q_neighbors > 0:
                        points[q] = q_neighbors
                    yield from generate_blobs(points, blob,
                                              forbidden, size)
                    if q in points:
                        del points[q]
                else:
                    yield blob
                blob[iq] = False

def blobs(p, size, forbidden=None):
    blob = [False] * 25
    blob[ix(p)] = True
    if forbidden is None:
        forbidden = list(blob)
    else:
        forbidden = list(forbidden)
        forbidden[ix(p)] = True
    p_neighbors = len(list(neighbors(p)))
    return generate_blobs({p: p_neighbors}, blob, forbidden, size-1)

def count_blue(blob):
    return sum((1 for x in zip(blob, grid) if all(x)))

def find_allowed(forbidden):
    for i in range(5):
        for j in range(i,5):
            if not forbidden[ix((j, i))]:
                return (j, i)
            if not forbidden[ix((i, j))]:
                return (i, j)
    assert False

def combine(*blobs):
    return [any(x) for x in zip(*(b for b in blobs if b is not None))]

def find_solutions(solution=(), start=(0,0), levels=5, size=5, forbidden=None):
    for b in blobs(start, size, forbidden=forbidden):
        blue = count_blue(b)
        if blue == 0 or blue == 3:
            if levels <= 1:
                yield solution + (b,)
            else:
                next_forbidden = combine(forbidden, b)
                next_start = find_allowed(next_forbidden)
                yield from find_solutions(solution + (b,), start=next_start,
                                          levels=levels-1, size=size,
                                          forbidden=next_forbidden)

def main():
    count = 0
    for solution in find_solutions():
        print("--------------------")
        for b in solution:
            dump_bool(b)
        count += 1
    print(count)

if __name__ == '__main__':
    main()
