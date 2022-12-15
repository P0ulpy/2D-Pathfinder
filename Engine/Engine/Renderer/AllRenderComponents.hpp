//
// Created by Flo on 14/12/2022.
//

#ifndef PATHFINDER_ALLRENDERCOMPONENTS_HPP
#define PATHFINDER_ALLRENDERCOMPONENTS_HPP

namespace Engine
{
    namespace {
        template<typename... Ts>
        struct TypeList {};
    }

    // Insert Renderer Component Type here
    using AllRenderableComponents = TypeList<>;
}

#endif //PATHFINDER_ALLRENDERCOMPONENTS_HPP
