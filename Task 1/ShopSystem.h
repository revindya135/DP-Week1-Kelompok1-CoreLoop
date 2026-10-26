#ifndef SHOP_SYSTEM_H
#define SHOP_SYSTEM_H

#include <iostream>

using namespace std;

// Toko hanya menampilkan penawaran; tidak mengubah money.
class ShopSystem {
public:
    void ShowOffer(int round) const {
        (void)round;
        cout << "[SHOP] offered: Bonus(+2) cost 2\n";
        cout << "[SHOP] skipped\n";
    }
};

#endif
