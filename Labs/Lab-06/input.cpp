int main() {
    int x = 10;
    float y = 20.55;
    double z = 100.0;
    
    if (x <= y && y >= 15) {
        x = x + 1;
    }

    if (x == 11 || z != 50.0) {
        bool result = true;
    }

    x = (y * 2) / 5;
    x++;
    y--;

    return 0;
}