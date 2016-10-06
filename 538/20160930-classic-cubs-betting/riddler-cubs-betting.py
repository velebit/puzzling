#!/usr/bin/python3
import itertools as it
import numpy as np
import matplotlib.pyplot as plt

def final_winnings(games_to_win=4):
    wins = {}
    for i in range(games_to_win):
        wins[(games_to_win,i)] = +100
        wins[(i,games_to_win)] = -100
    return wins

def before(score, games_to_win=4):
    if score == (0,0):
        return ()
    if score[0] == 0:
        return ((score[0],score[1]-1),)
    if score[1] == 0:
        return ((score[0]-1,score[1]),)
    return ((score[0]-1,score[1]), (score[0],score[1]-1),)

def after(score, games_to_win=4):
    if score[0] == games_to_win or score[1] == games_to_win:
        return ()
    return ((score[0],score[1]+1), (score[0]+1,score[1]),)

def solve(games_to_win=4):
    wins = final_winnings(games_to_win)
    bets = {}
    unknown = set(it.product(range(games_to_win), range(games_to_win)))
    while unknown:
        progress = False
        for score in frozenset(unknown):
            if all([s in wins for s in after(score, games_to_win)]):
                w = [wins[s] for s in after(score, games_to_win)]
                assert len(w) == 2
                bets[score] = ((w[1] - w[0]) / 2)
                wins[score] = ((w[1] + w[0]) / 2)
                unknown.remove(score)
                progress = True
        assert progress
    result = {}
    for i in wins:
        if i not in result: result[i] = {}
        result[i]['winnings'] = wins[i]
    for i in bets:
        if i not in result: result[i] = {}
        result[i]['bets'] = bets[i]
    return result

def make_arrows(score, info):
    rank = sum(score)
    win = info['winnings']
    bet = info['bets']
    return ( ((rank, win), (1, +bet)), ((rank, win), (1, -bet)) )

def arrows(solution):
    return [a for x in solution if 'bets' in solution[x]
              for a in make_arrows(x, solution[x])]

def make_annotations(score, info):
    rank = sum(score)
    win = info['winnings']
    bet = info['bets']
    off = 0.25
    if bet < 20:
        off = 0.4
    return ( {'text': ("$%.2f" % bet).replace('.00', ''),
              'xy': (rank, win), 'xytext': (rank+off, win)}, )

def annotations(solution):
    return [a for x in solution if 'bets' in solution[x]
              for a in make_annotations(x, solution[x])]

# Converts     (((a1,b1),(c1,d1)), ((a2,b2),(c2,d2)), ...)
# into roughly ((a1,a2,...), (b1,b2,...), (c1,c2,...), (d1,d2,...))
def quiverable(result):
    return zip(*((z for y in x for z in y) for x in result))

def main():
    solution = solve()

    x, y, u, v = quiverable(arrows(solution))
    q = plt.quiver(x, y, u, v, scale=1., scale_units='xy', angles='xy')
    plt.xlim((-0.5, 7.5))
    plt.ylim((-110, 110))
    plt.savefig('riddler-20160930-classic-cubs-betting.blank.png')
    plt.xlabel('games played')
    plt.ylabel('total winnings')
    plt.savefig('riddler-20160930-classic-cubs-betting.with-axes.png')

    for a in annotations(solution):
        plt.annotate(a['text'], xy=a['xy'], xytext=a['xytext'],
                     verticalalignment='center')
    plt.savefig('riddler-20160930-classic-cubs-betting.with-bets.png')

    plt.show()

if __name__ == '__main__':
    main()
