#include "attribute.h"
#include <stdexcept>

Attribute intToAttribute(int value) {
    switch (value) {
        case 1: return Attribute::Fire;
        case 2: return Attribute::Water;
        case 3: return Attribute::Earth;
        case 4: return Attribute::Light;
        case 5: return Attribute::Dark;
        case 6: return Attribute::Heal;
        default: throw std::invalid_argument("Invalid attribute value");
    }
}
