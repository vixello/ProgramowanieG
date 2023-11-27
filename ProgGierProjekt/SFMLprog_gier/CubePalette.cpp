#include "CubePalette.h"

CubePalette::CubePalette() {
    m_palette.emplace(Cube::Type::None, Cube("grass_debug.jpg"));
    m_palette.emplace(Cube::Type::Grass, Cube("grass.jpg"));
    m_palette.emplace(Cube::Type::Stone, Cube("stone.jpg"));
}

const Cube& CubePalette::LookUp(Cube::Type type) const {
    auto it = m_palette.find(type);
    if (it != m_palette.end()) {
        return it->second;
    }
    else {
        // Return a default cube or handle the missing cube type appropriately
        //return m_palette.at(Cube::Type::None);
        throw std::runtime_error("Default cube not found in palette");

    }
}
