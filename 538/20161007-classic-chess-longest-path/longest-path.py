#!/usr/bin/python3

import math

def map_union(*maps):
    m = dict()
    # reverse the order so the value from the *leftmost* arg wins collisions
    for i in maps[::-1]:
        m.update(i)
    return m

def reflect_point(p):
    x,y = p
    return (p, (y,x), (-x,y), (-y,x), (x,-y), (y,-x), (-x,-y), (-y,-x))

assert set(reflect_point((2,1))) == {(2,1),(-2,1),(-2,-1),(2,-1),
                                     (1,2),(-1,2),(-1,-2),(1,-2)}

def reflect_set(s):
    return {r for p in s for r in reflect_point(p)}

assert reflect_set({(2,1),(4,3)}) == {(2,1),(-2,1),(-2,-1),(2,-1),
                                      (1,2),(-1,2),(-1,-2),(1,-2),
                                      (4,3),(-4,3),(-4,-3),(4,-3),
                                      (3,4),(-3,4),(-3,-4),(3,-4)}

def reflect_map_p2p(m):
    return dict([r for (k,v) in m.items()
                 for r in zip(reflect_point(k), reflect_point(v))])

assert reflect_map_p2p({(2,1): (4,3)}) == \
    {(2,1): (4,3), (-2,1): (-4,3), (-2,-1): (-4,-3), (2,-1): (4,-3),
     (1,2): (3,4), (-1,2): (-3,4), (-1,-2): (-3,-4), (1,-2): (3,-4)}

# Based on: http://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
def ccw_number(a,b,c):
    return (c[1]-a[1])*(b[0]-a[0]) - (b[1]-a[1])*(c[0]-a[0])

assert ccw_number((0,-1), (0,1), (-1,0)) > 0
assert ccw_number((0,-1), (0,1), (-1,1)) > 0
assert ccw_number((0,-1), (0,1), (-1,2)) > 0
assert ccw_number((0,-1), (0,1), (0,2)) == 0
assert ccw_number((0,-1), (0,1), (1,2)) < 0
assert ccw_number((0,-1), (0,1), (1,1)) < 0
assert ccw_number((0,-1), (0,1), (1,0)) < 0

assert ccw_number((0,-1), (0,1), (0,1)) == 0
assert ccw_number((0,-1), (0,1), (0,0)) == 0
assert ccw_number((0,-1), (0,1), (0,-1)) == 0
assert ccw_number((0,-1), (0,1), (0,-2)) == 0

# Based on: http://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
def is_ccw(a,b,c):
    return ccw_number(a,b,c) > 0
def intersects(a, b):
    return ((is_ccw(a[0],b[0],b[1]) != is_ccw(a[1],b[0],b[1])) and
            (is_ccw(a[0],a[1],b[0]) != is_ccw(a[0],a[1],b[1])))

# PROBLEM: does not handle cases with colinear points correctly
for y in range(5):
    for x in range(5):
        expected = (x >= 1 and x <= 3 and y >= 1 and y <= 3)
        got = intersects(((1,3),(3,3)), ((x,y),(x,y+2)))
        assert got == expected, ("failed #1 (%s) for x=%d,y=%d" % (got,x,y))
        got = intersects(((3,3),(1,3)), ((x,y),(x,y+2)))
        assert got == expected, ("failed #2 (%s) for x=%d,y=%d" % (got,x,y))
        got = intersects(((1,3),(3,3)), ((x,y+2),(x,y)))
        assert got == expected, ("failed #3 (%s) for x=%d,y=%d" % (got,x,y))
        got = intersects(((3,3),(1,3)), ((x,y+2),(x,y)))
        assert got == expected, ("failed #4 (%s) for x=%d,y=%d" % (got,x,y))

def ck_intersects(a, b):
    permutations = (intersects((a[0],a[1]),(b[0],b[1])),
                    intersects((a[1],a[0]),(b[0],b[1])),
                    intersects((a[0],a[1]),(b[1],b[0])),
                    intersects((a[1],a[0]),(b[1],b[0])),
                    intersects((b[0],b[1]),(a[0],a[1])),
                    intersects((b[1],b[0]),(a[0],a[1])),
                    intersects((b[0],b[1]),(a[1],a[0])),
                    intersects((b[1],b[0]),(a[1],a[0])))
    assert all(permutations) == any(permutations)
    return permutations[0]
assert ck_intersects(((0,0),(1,0)),((2,0),(3,0))) == False, "colinear aabb"
assert ck_intersects(((0,0),(2,0)),((1,0),(3,0))) == True, "colinear abab"
assert ck_intersects(((0,0),(3,0)),((1,0),(2,0))) == True, "colinear abba"
