#pragma once

#include "Model/CardModel.h"

#include <array>
#include <vector>


namespace TestConfigs
{
    class Tableau
    {
    public:
        std::array<std::vector<CardModel>, 10> m_stacks;
        std::vector<CardModel> m_stock;
        std::vector<CardModel> m_packs;
    public:
        Tableau(
            const std::array<std::vector<CardModel>, 10>& stacks,
            const std::vector<CardModel>& stock,
            const std::vector<CardModel>& packs);
    };

    extern Tableau One;
}