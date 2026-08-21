#include "Jsonc.hpp"

#include <cassert>
#include <string>

/// @brief Strips JSONC comments, converting to pure RFC 8259 JSON.
static std::string strip_comments(const std::string &jsonc)
{
    assert(!jsonc.empty());

    std::string json;
    json.reserve(jsonc.size());

    enum class State
    {
        Default,

        // We're looking for the end of a comment, skipping escaped quotes: '\"'
        //
        // {
        //   "funny": "When the \"imposter\" is sus!"
        // }
        String,

        // We're looking for //...\n and /*...*/ comments, but quoted strings can contain slashes:
        //
        // {
        //   "url": "https://google.com/" // comment
        // }
        BlockComment,
        LineComment
    } state = State::Default;

    for (std::size_t i = 0; i < jsonc.size() - 1; i++)
    {
        switch (state)
        {
            default:
                if (jsonc[i] == '"')
                {
                    state = State::String;
                }
                else if (jsonc[i] == '/')
                {
                    if (jsonc[i + 1] == '*')
                    {
                        state = State::BlockComment;
                        i++;
                        break;
                    }
                    else if (jsonc[i + 1] == '/')
                    {
                        state = State::LineComment;
                        i++;
                        break;
                    }
                }
                json += jsonc[i];
                break;

            case State::String:
                json += jsonc[i];
                if (jsonc[i] == '\\')
                {
                    json += jsonc[++i];
                }
                else if (jsonc[i] == '"')
                {
                    state = State::Default;
                }
                break;

            case State::LineComment:
                if (jsonc[i] == '\n')
                {
                    state = State::Default;
                    json += jsonc[i];
                }
                break;

            case State::BlockComment:
                if ((jsonc[i] == '*') && (jsonc[i + 1] == '/'))
                {
                    state = State::Default;
                    i++;
                }
                break;
        }
    }
    json += jsonc.back();

    return json;
}

json_result<json::padded_string> jsonc::load(const std::filesystem::path &path)
{
    auto source = json::padded_string::load(path.string());
    if (source.error())
    {
        return source.error();
    }

    std::string jsonc(source.value().data(), source.value().size());

    return json::padded_string(strip_comments(jsonc));
}
