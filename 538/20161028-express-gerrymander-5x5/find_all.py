#!/usr/bin/python3

#import math
import sys

with open('grid-5x5.txt') as file:
    data = [[(char == 'B') for char in line.strip()]
            for line in file.readlines()]
flat = [x for row in data for x in row]

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

def generate_blobs(points, blob, acceptable, size):
    size -= 1
    points_iter = points.keys()  # can't iterate over modified dict
    points = dict(points)  # backtracking directly is too complex, so push/ppop
    acceptable = list(acceptable)
    for p in points_iter:
        for q in neighbors(p):
            iq = ix(q)
            if acceptable[iq]:
                blob[iq] = True
                acceptable[iq] = False
                if size > 0:
                    q_neighbors = 0
                    for r in neighbors(q):
                        if acceptable[ix(r)]:
                            q_neighbors += 1
                        if r in points:
                            r_neighbors = 0
                            for s in neighbors(r):
                                if acceptable[ix(s)]:
                                    r_neighbors += 1
                            if points[r] > 1:
                                points[r] -= 1
                            else:
                                del points[r]
                    if q_neighbors > 0:
                        points[q] = q_neighbors
                    yield from generate_blobs(points, blob,
                                              acceptable, size)
                    if q in points:
                        del points[q]
                else:
                    yield blob
                blob[iq] = False

def blobs(p, size):
    blob = [False] * 25
    blob[ix(p)] = True
    acceptable = [not p for p in blob]
    p_neighbors = len(list(neighbors(p)))
    return generate_blobs({p: p_neighbors}, blob, acceptable, size-1)

count = 0
for b in blobs((0,0), 5):
    count += 1
    dump_bool(b)
print(count)

count = 0
for b in blobs((0,0), 25):
    count += 1
    dump_bool(b)
print(count)
