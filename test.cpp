#include <iostream>

using namespace std;

char gambar[32][32];

int main() {
   int x1 = 1;
   int y1 = 7;
   int x2 = 8;
   int y2 = 1;

   int y = y1;
   int dy = y2 - y1;

   int grad = (y2-y1)*(x2-x1);

   if (grad > 0) {
    int dxdy = y2 - y1 + x1 - x2;
    int F = y2 - y1 + x1 - x2; 
    for (int x = x1; x <= x2; x++) {
        gambar[y][x] = '*';
        if (F < 0) {
            F += dy;
        } else {
            y++;
            F += dxdy;
        }
    }
   } else {
        int dxdy = y2 - y1 + x1 - x2;
        int F = y2 - y1 + x1 - x2; 
        for (int x = x1; x <= x2; x++) {
            gambar[y][x] = '*';
            if (F > 0) {
                F += dy;
            } else {
                y--;
                F += dxdy;
            }
        
        }
   }
   
   for(int i = 0; i < 32; i++) {
       for(int j = 0; j < 32; j++) {
           cout << gambar[i][j] << " ";
       }
       cout << endl;
   }
}