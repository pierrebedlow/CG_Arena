#pragma once
#include <string>
#include <sstream>

#define NB_INGREDIENT 4

class Recipe {

public:
    
    int delta[NB_INGREDIENT];

    std::string CHARS[NB_INGREDIENT] = { "A", "B", "C", "D" };

    Recipe(int a, int b, int c, int d) {
        delta[0] = a;
        delta[1] = b;
        delta[2] = c;
        delta[3] = d;
    }

    Recipe() {
        delta[0] = 0;
        delta[1] = 0;
        delta[2] = 0;
        delta[3] = 0;
    }

    Recipe(const Recipe& other) {
        delta[0] = other.delta[0];
        delta[1] = other.delta[1];
        delta[2] = other.delta[2];
        delta[3] = other.delta[3];
    }

    std::string toString() {
        std::stringstream sb;
        if (IntStream.of(this.delta).allMatch(b -> b == 0)) {
            return "∅";
        }

        if (IntStream.of(this.delta).anyMatch(b -> b > 0)) {
            sb.append("+");
            for (int i = 0; i < 4; ++i) {
                for (int k = 0; k < this.delta[i]; ++k) {
                    sb.append(CHARS[i]);
                }
            }
        }

        if (IntStream.of(this.delta).anyMatch(b -> b < 0)) {
            if (IntStream.of(this.delta).anyMatch(b -> b > 0)) {
                sb.append('\n');
            }
            sb.append("-");
            for (int i = 0; i < 4; ++i) {
                for (int k = this.delta[i]; k < 0; ++k) {
                    sb.append(CHARS[i]);
                }
            }
        }

        return sb.toString();
    }

    std::string toPlayerString() {
        return std::string.format("%d %d %d %d", delta[0], delta[1], delta[2], delta[3]);
    }

    void add(int idx, int x) {
        this->delta[idx] += x;
    }

    int getTotal() {
        return (delta[0] + delta[1] + delta[2] + delta[3]);
    }

    int getTotalLoss() {
        return -IntStream.of(delta)
            .filter(i -> i < 0)
            .sum();
    }

    int getTotalGain() {
        return IntStream.of(delta)
            .filter(i -> i > 0)
            .sum();
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(this.delta);
    }

    bool opeartor==(const Recipe& recipe) {
        
        return Arrays.equals(this.delta, other.delta);
    }
}
