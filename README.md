# PS/CP C++ 템플릿

이 레포는 PS/CP(Problem Solving / Competitive Programming)용 C++ 템플릿 모음입니다.
대회에서 바로 사용할 수 있도록 알고리즘별 구현을 파일 단위로 정리해 두었습니다.

## 알고리즘 위치 안내

- `Automata.cpp`
  - Trie
  - Aho–Corasick
- `Combinatorics.cpp`
  - 조합/팩토리얼/역원 테이블 (`_comb`)
  - Lucas 정리 (`_lucas`)
  - CRT 기반 조합 (`_comb_crt`)
- `DSU.cpp`
  - Union-Find (`_uf`)
  - Weighted Union-Find (`_wuf`)
  - Union-Find Set (`_ufset`)
- `FFT.cpp`
  - FFT 기반 곱셈 템플릿 (`_fft`)
- `Flow.cpp`
  - Dinic 최대 유량 (`_flow`)
  - 최소 비용 최대 유량 (`_mcmf`)
- `Geometry.cpp`
  - CCW/방향성 판별 (`_ccw`)
  - 선분/직선 기하 (`_li`, `_line`, `_inter`)
  - 볼록 껍질/Rotating Calipers (`_ch`, `_rc`)
  - 다각형 넓이 (`_area`)
  - 점/벡터 연산 (`_dot`)
  - 모듈러 행렬 연산 (`_mod_matrix`)
  - GF(2) 선형대수 (`_gf2`)
- `Graph.cpp`
  - LCA (`_lca`)
  - 희소 배열(스파스 테이블) 점프 (`_st`)
  - MST (`_mst`)
  - SCC (`_scc`)
  - 2-SAT (`_2sat`)
  - 이분 매칭 (`_bm`)
  - 위상 정렬 (`_tsort`)
  - BCC (`_bcc`)
  - 3/4-cycle 탐색 (`_3cycle`, `_4cycle`)
- `Hash.cpp`
  - 문자열 해시 (`_hash`)
  - 덱 해시 (`_deque_hash`)
- `Number_Thoery.cpp`
  - 모듈러 정수 (`_mint`)
  - 에라토스테네스의 체 (`_es`)
  - 거듭제곱/빠른 거듭제곱 (`_pow`)
  - 확장 유클리드/역원 (`_gcd`)
  - CRT (`_crt`)
  - 이산 로그(BSGS) (`_dlog`)
  - Pollard Rho 소인수분해/소수 판정 (`_pr`)
  - 선형 체/φ, μ (`_ls`)
  - Prime Tower/지수 탑 모듈러 계산 (`_pt`)
- `Others.cpp`
  - 이분 탐색 템플릿 (`_bs`)
  - LIS (`_lis`)
  - LCS (`_lcs`)
  - Mo’s algorithm (`_mos`)
  - 삼분 탐색 (`_ts`)
  - Convex Hull Trick (`_cht`)
  - Parametric Binary Search (`_pbs`)
- `Segtree.cpp`
  - 합 세그먼트 트리 (`_sseg`)
  - 일반 세그먼트 트리 (`_seg`)
  - Lazy Propagation (`_prop`)
  - HLD (`_hld`)
  - Merge Sort Tree (`_mt`)
  - Fenwick Tree (`_fw`)
  - 2D Fenwick Tree (`_fw2d`)
  - Persistent Segment Tree (`_pst`)
- `Shortest_Path.cpp`
  - BFS (`_bfs`)
  - Dijkstra (`_dij`)
  - Floyd–Warshall (`_floyd`)
  - SPFA (`_spfa`)
- `String.cpp`
  - KMP (`_kmp`)
  - Suffix Array + LCP (`_sa`)
  - Manacher (`_mnc`)
  - Z-algorithm (`_z`)

## snippets/

`snippets/`에는 VS Code용 code snippets가 들어있습니다. 각 알고리즘명에 대응하는
`.code-snippets` 파일이 존재하며 위 템플릿 파일들과 1:1로 매칭됩니다.
