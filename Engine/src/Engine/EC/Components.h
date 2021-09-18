#pragma once

#include "pch.h"

#include <glm/glm.hpp>

namespace tsEngine
{
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };
}