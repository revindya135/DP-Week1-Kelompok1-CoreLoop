# Design Pattern Task 2 — Core Loop: Balatro-Like Run

Game yang dipakai adalah desain run ala Balatro dari Task 1: dimainkan 3 ronde, tiap ronde pemain beraksi, sistem menghitung skor, reward diberikan sebagai uang, lalu ada fase toko. Task ini hanya berupa kerangka arsitektur, jadi logikanya berupa placeholder (cetak teks).

## Cara Compile & Jalankan

```
g++ -std=c++17 -Wall -Wextra -o game main.cpp
./game              (Windows PowerShell: .\game.exe)
./game --seed 7     (hasil bisa diulang)
```

## Struktur File

| File | Isi | Sifat |
|---|---|---|
| `Types.h` | struct `TurnInput` | data |
| `Interfaces.h` | `IInputGenerator`, `IScoringRule`, `IRewardRule` | kontrak untuk bagian mutable |
| `InputGenerators.h` | `RandomInputGenerator` | mutable |
| `Rules.h` | `SimpleScoringRule`, `BonusRewardRule` | mutable |
| `ShopSystem.h` | `ShopSystem` | mutable (isi penawaran) |
| `GameState.h` | `GameState` (money, ronde) | pemegang state |
| `RunSession.h` | `RunSession` (loop dan urutan fase) | **invariant** |
| `main.cpp` | merakit objek, memanggil `StartGame()` | perakit |

## Step 1 — Core Loop

1. Pemain beraksi: sistem menghasilkan input aksi pemain untuk ronde ini (`GenerateInputPhase`).
2. Sistem mengevaluasi aksi tersebut menjadi skor dasar (`ResolveScorePhase`).
3. Reward dihitung dari skor dasar (`RewardPhase`).
4. State game diperbarui: reward ditambahkan ke money (`UpdateStatePhase`).
5. Fase toko: penawaran ditampilkan (`ShopPhase`).
6. Maju ke ronde berikutnya (`AdvanceRoundPhase`).
7. Cek apakah game selesai (`IsGameOver`). Jika belum, ulangi dari langkah 1.

## Step 2 — Invariants

1. **Langkah yang tidak boleh berubah urutannya:** seluruh urutan 1 sampai 7 di atas. Yang paling kritis adalah aksi → skor → reward → update money, karena tiap langkah memakai hasil langkah sebelumnya.
2. **Komponen yang harus selalu ada:** `RunSession` (pemilik loop dan urutan), `GameState` (menyimpan money dan ronde), serta sumber aksi, aturan skor, dan aturan reward. Tanpa salah satunya loop tidak punya data untuk diproses.
3. **Yang rusak jika urutan berubah:**
   - Reward sebelum skor: reward memakai skor kosong atau skor ronde sebelumnya.
   - Update money sebelum reward: money bertambah dengan nilai yang belum dihitung.
   - Toko sebelum update money: toko memakai uang lama (penting jika nanti toko menjual barang).
   - `IsGameOver` dicek di tengah ronde: game bisa berhenti sebelum reward dan toko selesai.

## Step 3 — Mutable Elements

| Elemen | Class | Mengapa mutable, bukan invariant |
|---|---|---|
| Cara aksi pemain dihasilkan | `RandomInputGenerator` | Hanya soal dari mana angka datang (acak, tetap, atau input pemain). Urutan loop tidak terpengaruh |
| Rumus skor dasar | `SimpleScoringRule` | Hanya rumus hitung. Selama menerima aksi dan mengembalikan skor, loop tidak peduli isinya |
| Besar reward | `BonusRewardRule` | Saat ini skor + 2. Boleh diganti jadi kali dua, bergantung ronde, atau ditambah penalti |
| Isi penawaran toko | `ShopSystem` | Hanya konten. Fase toko tetap dipanggil di posisi yang sama |
| Jumlah ronde | `GameState(3)` | Hanya angka parameter. Aturan "ulangi sampai `IsGameOver`" tetap sama |

## Step 4 — Pemetaan Loop ke Kode

| Langkah loop | Class | Method | File |
|---|---|---|---|
| 1. Pemain beraksi | `RunSession` → `IInputGenerator` | `GenerateInputPhase()` → `Generate()` | `RunSession.h`, `Interfaces.h` |
| 2. Evaluasi skor dasar | `RunSession` → `IScoringRule` | `ResolveScorePhase()` → `ComputeBaseScore()` | `RunSession.h`, `Interfaces.h` |
| 3. Hitung reward | `RunSession` → `IRewardRule` | `RewardPhase()` → `ComputeReward()` | `RunSession.h`, `Interfaces.h` |
| 4. Update state | `RunSession` → `GameState` | `UpdateStatePhase()` → `AddMoney()` | `RunSession.h`, `GameState.h` |
| 5. Toko | `RunSession` → `ShopSystem` | `ShopPhase()` → `ShowOffer()` | `RunSession.h`, `ShopSystem.h` |
| 6. Maju ronde | `RunSession` → `GameState` | `AdvanceRoundPhase()` → `NextRound()` | `RunSession.h`, `GameState.h` |
| 7. Cek selesai | `RunSession` → `GameState` | `IsGameOver()` → `IsFinished()` | `RunSession.h`, `GameState.h` |

| Invariant | Dijaga di |
|---|---|
| Urutan fase | `RunSession::StartGame()` di `RunSession.h`, satu-satunya tempat fase dipanggil |
| Money hanya berubah di fase update state | `GameState::AddMoney()` (`GameState.h`), hanya dipanggil dari `UpdateStatePhase()` |
| `RunSession` tidak tahu class konkret | `RunSession.h` tidak meng-include `InputGenerators.h` maupun `Rules.h`; keduanya hanya di-include oleh `main.cpp` |
| `main()` tanpa logika game | Hanya membuat objek lalu memanggil `StartGame()` |

## Contoh Output (`./game --seed 7`)

```
=== GAME START ===
Round 1
[PLAY] input generated: 1
[SCORE] base score: 1
[REWARD] gain: 3
[STATE] money: 3
[SHOP] offered: Bonus(+2) cost 2
[SHOP] skipped
Round 2
[PLAY] input generated: 2
[SCORE] base score: 2
[REWARD] gain: 4
[STATE] money: 7
[SHOP] offered: Bonus(+2) cost 2
[SHOP] skipped
Round 3
[PLAY] input generated: 4
[SCORE] base score: 4
[REWARD] gain: 6
[STATE] money: 13
[SHOP] offered: Bonus(+2) cost 2
[SHOP] skipped
=== GAME END ===
Final money: 13
```

## Refleksi

1. **Invariant structure game saya** adalah urutan tujuh langkah yang dikendalikan `RunSession::StartGame()`: aksi pemain, evaluasi skor dasar, perhitungan reward, pembaruan state, fase toko, maju ke ronde berikutnya, lalu pengecekan `IsGameOver()` sebelum mengulang. Urutan ini tidak boleh berubah karena setiap langkah memakai hasil langkah sebelumnya. Skor butuh aksi pemain, reward butuh skor, dan money hanya boleh bertambah setelah reward diketahui. Money juga hanya berubah lewat `GameState::AddMoney()` yang dipanggil di fase update state, jadi ada satu tempat yang jelas untuk melacak perubahan uang. Selain urutan, komponen yang harus selalu ada adalah `RunSession`, `GameState`, sumber aksi, aturan skor, dan aturan reward, karena tanpa salah satunya loop tidak punya data untuk diproses.

2. **Bagian yang mutable** adalah hal-hal yang boleh berubah tanpa menyentuh urutan loop: cara aksi pemain dihasilkan (`RandomInputGenerator`), rumus skor dasar (`SimpleScoringRule`), besar reward (`BonusRewardRule`, saat ini skor ditambah dua), isi penawaran toko (`ShopSystem`), dan jumlah ronde yang dikirim lewat constructor `GameState`. Semuanya berupa isi atau angka, bukan urutan. Karena `RunSession` hanya mengenal interface, bagian-bagian itu bisa diganti dengan mengubah satu baris di `main()`. Saya menganggapnya mutable karena nilainya memang wajar berubah antar versi game, sedangkan urutan langkahnya tidak.

3. **Jika menambah fitur baru**, class yang berubah bergantung pada jenis fiturnya. Penalti atau bonus per ronde cukup menambah class baru yang mengimplementasikan `IRewardRule`. Item toko baru hanya mengubah `ShopSystem`. Rumus skor baru cukup class baru dari `IScoringRule`. `RunSession` baru perlu diubah jika saya menambah fase baru, dan itu adalah keputusan struktural yang harus dipikirkan dengan sengaja, bukan perubahan sehari-hari. Dengan pembagian ini, perubahan kecil tetap terisolasi di class yang bertanggung jawab atas hal tersebut dan tidak merembet ke bagian lain.

4. **Jika urutan loop diubah**, game akan menghasilkan angka yang salah atau berhenti di tempat yang salah. Jika reward dihitung sebelum skor, reward memakai skor lama atau nol. Jika money diperbarui sebelum reward dihitung, uang bertambah dengan nilai yang belum benar. Jika toko dijalankan sebelum money diperbarui, toko memakai uang lama, dan ini akan jadi masalah nyata begitu toko menjual barang. Jika `IsGameOver()` dicek di tengah ronde, game bisa berhenti sebelum reward dan toko selesai. Karena itu urutan ini dikunci di satu tempat, yaitu `StartGame()`.

*Jumlah kata refleksi: 353*
