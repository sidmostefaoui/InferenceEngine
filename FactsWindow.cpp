#include "FactsWindow.h"
#include <imgui/imgui.h>
#include <vector>
#include <string>


static bool syntax_error = false;

auto facts = std::vector<std::string>();

auto FactsWindow::Draw() -> void
{
    
    ImGui::Begin("Base de Faits");
    {
        ImGui::InputTextWithHint(" ", "Ajouter un fait", input_, IM_ARRAYSIZE(input_));

        ImGui::SameLine();
        if (ImGui::Button("Ajouter"))
        {
            auto tokens = Fact::Scanner::Scan(input_);

            if (!tokens)
            {
                syntax_error = true;
                goto brk;
            }

            auto fact = Fact::Parser::Parse(*tokens);

            if (!fact)
            {
                syntax_error = true;
                goto brk;
            }

            ie_.AddFact(*fact);
            facts.push_back(input_);
            syntax_error = false;
        }
brk:

        if (syntax_error)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Erreur syntaxique");
        }

        ImGui::Separator();

        for (int i = 0; i < facts.size(); i++)
        {
            ImGui::TextWrapped("%i. %s", i + 1, facts[i].c_str());
        }

    }
    ImGui::End();
}

FactsWindow::FactsWindow(InferenceEngine& ie)
    : ie_(ie), input_()
{

}
