#pragma once

#include <string_view>
#include <ostream>

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
            catFile(const std::string_view hash, std::ostream& ouput);
    };

} // git