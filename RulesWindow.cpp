#include "RulesWindow.hpp"
#include <imgui/imgui.h>
#include <vector>
#include <string>

RulesWindow::RulesWindow(InferenceEngine& ie, const std::vector<std::string>& rules)
	: ie_(ie), input_(), rules_(rules)
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
            std::string rule_str = input_;
            std::string target = "->";
            int count = 0;
            size_t pos = 0;

            while ((pos = rule_str.find(target, pos)) != std::string::npos)
            {
                count += 1;
                pos += target.length();
            }

            if (count != 1)
            {
                syntax_error = true;
                goto brk;
            }

            std::string expr_str = rule_str.substr(0, rule_str.find(target));
            std::string fact_str = rule_str.substr(rule_str.find(target) + target.length());

            auto fact_tokens = Fact::Scanner::Scan(fact_str);

            if (!fact_tokens)
            {
                syntax_error = false;
                goto brk;
            }

            auto fact = Fact::Parser::Parse(*fact_tokens);

            if (!fact)
            {
                syntax_error = true;
                goto brk;
            }

            auto expr_tokens = Expr::Scanner::Scan(expr_str);

            if (!expr_tokens)
            {
                syntax_error = false;
                goto brk;
            }

            auto expr = Expr::Parser::Parse(*expr_tokens);

            if (!expr)
            {
                syntax_error = true;
                goto brk;
            }

            ie_.AddRule({*expr, *fact});
            rules_.push_back(input_);
            syntax_error = false;
        }
    brk:

        if (syntax_error)
        {
            ImGui::Text("erreur syntaxique");
        }

        ImGui::Separator();

        for (auto& f : rules_)
            ImGui::Text(f.c_str());

    }
    ImGui::End();
}

