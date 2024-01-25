#pragma once

#include "WorldGenerator.h"

#include "nlohmann/json.hpp"
#include <glm/glm.hpp>

#include <vector>

namespace nlohmann {
template<>
struct adl_serializer<glm::vec2> {
    static void to_json(json& j, const glm::vec2& v) {
        j["x"] = v.x;
        j["y"] = v.y;
    }
        
    static void from_json(const json& j, glm::vec2& v) {
        v.x = j["x"];
        v.y = j["y"];
    }
};

template<>
struct adl_serializer<glm::vec3> {
    static void to_json(json& j, const glm::vec3& v) {
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
    }
        
    static void from_json(const json& j, glm::vec3& v) {
        v.x = j["x"];
        v.y = j["y"];
        v.z = j["z"];
    }
};

// Special way of declaring from/to _json for types that shall be moved instead of copied
// In case chunk implementations differ and it's copyable type
// function overload can be declared same as for glm::vecN
template <>
struct adl_serializer<WorldGenerator::FixedSizeChunk> {
    static WorldGenerator::FixedSizeChunk from_json(const json& j) {
        // deserialize chunks origin
        const glm::vec2 chunkOrigin = j["origin"];

        WorldGenerator::FixedSizeChunk chunk(chunkOrigin);

        // unpack zipped chunk data
        std::vector<std::pair<uint8_t,unsigned int>> zippedData = j["zipped_data"];

        // Deserialize data into chunk
        int  i = 0;
        for (auto& item : zippedData) {
            //Cube::Type cubeType = item["cube_id"];
            //int occurrences = item["occurrences"];
            //unzippedData.emplace_back(cubeType, occurrences);
            //std::fill(
            //    chunk.m_data.begin() + i,
            //    chunk.m_data.begin() + i + item.second,
            //    WorldGenerator::FixedSizeChunk::CubeData(static_cast<Cube::Type>(item.first),true)
            //);
            //i += item.second;
            //WorldGenerator::CubeData cubeData(static_cast<Cube::Type>(item.first), true);

            std::fill(
                chunk.m_data.begin() + i,
                chunk.m_data.begin() + i + item.second,
                WorldGenerator::FixedSizeChunk::CubeData(static_cast<Cube::Type>(item.first), true)
            );
            i += item.second;
        }
        chunk.UpdateVisibility();
        std::cout <<"AAAAAAAAAAAAAAAAAAAAAAAA" <<chunk.m_origin.x << std::endl;

        return std::move(chunk);
    }

    static void to_json(json& j, WorldGenerator::FixedSizeChunk chunk) {
        // serialize chunk origin into json
        j["origin"] = chunk.m_origin;

        // zip chunk data into pairs of cube id and number of occurrences
        std::vector<std::pair<uint8_t, int>> zippedData;
        for (int i = 0; i < chunk.m_data.size();) {
            Cube::Type currentType = chunk.m_data[i].m_type;
            int j = i++;
            while (j < chunk.m_data.size() && currentType == chunk.m_data[j].m_type) {
                j++;
            }
            zippedData.emplace_back(static_cast<uint8_t>(currentType), j - i);
            i = j;
        }
        // serialized zipped data into json
        j["zipped_data"] = zippedData;
    }
};

}
