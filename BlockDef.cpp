#include "BlockDef.hpp"

#include <cassert>

void BlockDef::Load(ska::flat_hash_map<block_id_t, BlockDef> &out, json::ondemand::parser &parser, const fs::path &path)
{
    auto json = jsonc::load(path);
    assert(!json.error());

    bool isDebug = (path.parent_path().filename() == "Debug");

    json::ondemand::document doc = parser.iterate(json);

    std::string_view parentName;
    bool hasParentName = !doc["name"].get(parentName);

    json::ondemand::array variants = doc["variants"].get_array();

    for (json::ondemand::value variant : variants)
    {
        block_id_t id;
        assert(!variant["id"].get(id));

        std::string_view texture;
        assert(!variant["texture"].get(texture));

        std::string_view variantName;
        bool hasVariantName = !variant["name"].get(variantName);

        assert(hasParentName || hasVariantName);
        std::string name = hasVariantName ? std::string{variantName} : std::string{parentName};

        BlockDef blockDef{std::move(name), std::move((path.parent_path() / texture).string()), isDebug, {0}};

        assert(out.emplace(id, std::move(blockDef)).second);
    }
}
