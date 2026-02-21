TREE_SIZE = 1100
MAX_HB = TREE_SIZE

Bh = [[0 for j in range(MAX_HB)] for i in range(TREE_SIZE)]
Rh = [[0 for j in range(MAX_HB)] for i in range(TREE_SIZE)]
childs = [[0, 0] for i in range(TREE_SIZE)]
parents = [0 for i in range(TREE_SIZE)]


def dfs(node):
    if node == 0:
        Bh[node][1] = 1
        return None

    dfs(childs[node][0])
    dfs(childs[node][1])
    for h in range(1, MAX_HB):
        Bh[node][h] = (Bh[childs[node][0]][h - 1] + Rh[childs[node][0]][h - 1]) * (
            Bh[childs[node][1]][h - 1] + Rh[childs[node][1]][h - 1]
        )
        Rh[node][h] = Bh[childs[node][0]][h] * Bh[childs[node][1]][h]


n = int(input())
for i in range(1, n + 1):
    a, b = 2 * i, 2 * i + 1  # map(int, input().split())
    # a,b = map(int, input().split())
    if a > 1000:
        a = 0
    if b > 1000:
        b = 0
    childs[i][0] = a
    childs[i][1] = b
    parents[a] = i
    parents[b] = i
root = -1
for i in range(1, TREE_SIZE):
    if parents[i] == 0:
        root = i
        break
dfs(root)
ansB = 0
ansR = 0
for h in range(MAX_HB):
    ansB += Bh[root][h]
    ansR += Rh[root][h]
print(ansB + ansR)
# printf("black root: %d\nRed root: %d\nAns: %d", ansB, ansR, ansB + ansR);
