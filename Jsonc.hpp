#ifndef _JSONC_HPP
#define _JSONC_HPP

#include <filesystem>
#include "simdjson.h"

namespace fs = std::filesystem;
namespace json = simdjson;
template <typename T> using json_result = json::simdjson_result<T>;

namespace jsonc
{

/// @brief Loads a JSONC file as padded RFC 8259 JSON.
json_result<json::padded_string> load(const fs::path &path);

} // namespace jsonc

#endif /* _JSONC_HPP */
