# Design Pattern Task 1 — Balatro-Like Run (Core Loop & System Thinking)

## Refleksi
1. Invariant structure di program ini adalah urutan enam fase yang dijalankan `RunSession`: generate input, compute base score, compute reward, update money, shop, lalu advance round. Struktur ini juga mencakup jumlah ronde yang tetap tiga dan aturan bahwa money hanya boleh berubah di `UpdateMoneyPhase`. Urutan tersebut tidak boleh ditukar karena input harus ada sebelum skor dihitung, dan skor harus ada sebelum reward dihitung. Dengan begitu, setiap ronde selalu mengikuti alur yang sama.

2. Bagian yang mutable adalah pembangkit input, aturan skor, aturan reward, dan isi toko. Semua bagian itu bisa diganti tanpa mengubah struktur alur karena `RunSession` hanya memanggil kontrak antarmuka. Pada tugas ini, `RandomInputGenerator` dan `BonusRewardRule` ditambahkan, lalu dipakai di `main` tanpa menyentuh `RunSession`.

3. Saat InputGenerator diganti, `RunSession` tidak berubah karena ia hanya bergantung pada interface `IInputGenerator` dan tidak mengenal class konkret. `RunSession` cukup memanggil `Generate(round)`, sehingga implementasi apa pun yang memenuhi kontrak dapat dipakai. Prinsip yang sama berlaku untuk aturan skor dan reward.

4. Jika logika skor ditaruh di `RunSession`, setiap perubahan skor memaksa mengedit `RunSession`. Akibatnya risiko merusak urutan fase meningkat, aturan skor tidak bisa ditukar atau diuji terpisah, dan tanggung jawab class menjadi bercampur. Pemisahan melalui interface menjaga `RunSession` tetap stabil sekaligus membuat variasi aturan mudah dilakukan.
