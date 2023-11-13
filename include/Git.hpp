#pragma once

#include <string_view>

namespace git
{

    class Git
    {
        public:
            static void
            processCommand(const int argc, char** argv);

        private:
            static void
            init();

            static void
            catFile(const std::string_view hash);
    };

} // git