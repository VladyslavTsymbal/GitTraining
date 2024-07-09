#pragma once

#include "CommandFactory.hpp"

#include <vector>

namespace git
{

class HighLevelCommandFactory: public CommandFactory
{
    public:
        static
        HighLevelCommandFactory& getInstance()
        {
            static HighLevelCommandFactory factory;
            return factory;
        }

        std::unique_ptr<GitCommand>
        createCommand(const int argc, char** argv) override
        {
            if (argc < 2)
            {
                throw std::runtime_error("HighLevelCommandParser::parse - argc < 2");
            }

            for (const auto factory : m_factories)
            {
                if (auto command = factory->createCommand(argc, argv); command != nullptr)
                {
                    return command;
                }
            }

            return nullptr;
        }

        void
        addFactory(CommandFactory* factory)
        {
            if (!factory)
            {
                throw std::runtime_error("HighLevelCommandParser::addFactory - parser is nullptr");
            }

            m_factories.push_back(factory);
        }

    private:
        HighLevelCommandFactory() = default;

    private:
        std::vector<CommandFactory*> m_factories;
};

} // namespace git
