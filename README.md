# PS/CP C++ 템플릿

PS/CP(Problem Solving / Competitive Programming)용 C++ 템플릿 모음입니다.
대회에서 바로 사용할 수 있도록 알고리즘별 구현을 파일 단위로 정리해 두었습니다.

## 알고리즘 위치 안내

- `Automata.cpp`
  - Trie (`_trie`)
  - Aho–Corasick (`_ac`)
- `Combinatorics.cpp`
  - Combination Table (`_comb`)
  - Lucas's Theorem (`_lucas`)
  - CRT 기반 조합 (`_comb_crt`)
- `DSU.cpp`
  - Union-Find (`_uf`)
  - Weighted Union-Find (`_wuf`)
  - Union-Find Set (`_ufset`)
- `FFT.cpp`
  - FFT (`_fft`)
- `Flow.cpp`
  - Dinic (`_flow`)
  - MCMF (`_mcmf`)
- `Geometry.cpp`
  - CCW (`_ccw`)
  - 선분/직선 기하 (`_li`, `_line`, `_inter`)
  - Convex hull (`_ch`)
  - Rotating Calipers (`_rc`)
  - 다각형 넓이 (`_area`)
  - 점/벡터 연산 (`_dot`)
  - 모듈러 행렬 연산 (`_mod_matrix`)
  - GF(2) 선형대수 (`_gf2`)
- `Graph.cpp`
  - LCA (`_lca`)
  - Sparse Table (`_st`)
  - MST (`_mst`)
  - SCC (`_scc`)
  - 2-SAT (`_2sat`)
  - Bipartite Matching (`_bm`)
  - Topology Sort (`_tsort`)
  - BCC (`_bcc`)
  - 3/4-cycle Search (`_3cycle`, `_4cycle`)
- `Hash.cpp`
  - Prefix Sum Hash (`_hash`)
  - Deque Hash (`_deque_hash`)
- `Number_Theory.cpp`
  - modint (`_mint`)
  - 에라토스테네스의 체 (`_es`)
  - 분할정복 거듭제곱 (`_pow`)
  - Extended Euclidean (`_gcd`)
  - CRT (`_crt`)
  - 이산 로그 (`_dlog`)
  - Pollard Rho (`_pr`)
  - Linear Sieve/φ, μ (`_ls`)
  - Prime Tower (`_pt`)
- `Others.cpp`
  - Binary Search (`_bs`)
  - LIS (`_lis`)
  - LCS (`_lcs`)
  - Mo’s Algorithm (`_mos`)
  - Ternary Search (`_ts`)
  - Convex Hull Trick (`_cht`)
  - Parallel Binary Search (`_pbs`)
- `Segtree.cpp`
  - Sum Segment Tree (`_sseg`)
  - Segment Tree (`_seg`)
  - Lazy Propagation (`_prop`)
  - HLD (`_hld`)
  - Merge Sort Tree (`_mt`)
  - Fenwick Tree (`_fw`)
  - 2D Fenwick Tree (`_fw2d`)
  - Persistent Segment Tree (`_pst`)
- `Shortest_Path.cpp`
  - Dijkstra (`_dij`)
  - Floyd–Warshall (`_floyd`)
  - SPFA (`_spfa`)
- `String.cpp`
  - KMP (`_kmp`)
  - Suffix Array + LCP (`_sa`)
  - Manacher (`_mnc`)
  - Z-algorithm (`_z`)

## snippets/

`snippets/`에는 VS Code용 code snippets이 들어있습니다. 각 알고리즘명에 대응하는
`.code-snippets` 파일이 존재하며 위 템플릿 파일들과 1:1로 매칭됩니다.
