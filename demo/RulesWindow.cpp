#include "RulesWindow.hpp"
#include <imgui/imgui.h>
#include <vector>
#include <string>

RulesWindow::RulesWindow(InferenceEngine& ie)
	: ie_(ie), input_(), rules_()
{

}

static bool syntax_error = false;


auto RulesWindow::Draw() -> void
{

    ImGui::Begin("Base de Regles");
    {
        ImGui::InputTextWithHint(" ", "Ajouter une regle", input_, IM_ARRAYSIZE(input_));

        ImGui::SameLine();
        if (ImGui::Button("Ajouter"))
        {
            auto rule = Rule::Parser::Parse(input_);

            if (!rule)
            {
                syntax_error = true;
                goto brk;
            }

            ie_.AddRule({rule->expr, rule->fact});
            rules_.push_back(input_);
            syntax_error = false;
        }
    brk:

        if (syntax_error)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Erreur syntaxique");
        }

        ImGui::Separator();

        for (int i = 0; i < rules_.size(); i++)
        {
            ImGui::TextWrapped("%i. %s", i + 1, rules_[i].c_str());
        }

    }
    ImGui::End();
}

