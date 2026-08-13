#include "Application.h"

namespace Engine {
	namespace Editor {

		class ExampleLayer : public Layer
		{
		public:
			void OnAttach() override
			{
				// initialization
			}

			void OnDetach() override
			{
				// cleanup
			}

			void OnUpdate(float ts) override
			{
				// update
			}

			void OnUIRender() override
			{
				ImGui::Begin("Console");

                for (const auto& line : CORE::Logger::consolebuffer.lines)
                {
                    DrawConsoleLine(line);
                }
				ImGui::End();

				ImGui::ShowDemoWindow();
			}
            DATA::ConsoleLine ParseANSI(const std::string& input)
            {
                DATA::ConsoleLine result;

                ImVec4 currentColor = ImVec4(1, 1, 1, 1);

                size_t pos = 0;
                size_t textStart = 0;

                while (pos < input.size())
                {
                    if (input[pos] == '\033' &&
                        pos + 1 < input.size() &&
                        input[pos + 1] == '[')
                    {
                        // متن قبل از ANSI code
                        if (pos > textStart)
                        {
                            result.segments.push_back({
                                input.substr(textStart, pos - textStart),
                                currentColor
                                });
                        }

                        size_t m = input.find('m', pos);

                        if (m == std::string::npos)
                            break;

                        std::string codes =
                            input.substr(pos + 2, m - pos - 2);

                        // Reset
                        if (codes == "0")
                        {
                            currentColor = ImVec4(1, 1, 1, 1);
                        }

                        // Bold Red
                        else if (codes == "1;31")
                        {
                            currentColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                        }

                        // Bold Green
                        else if (codes == "1;32")
                        {
                            currentColor = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                        }

                        // Bold Yellow
                        else if (codes == "1;33")
                        {
                            currentColor = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
                        }

                        // Bold Blue
                        else if (codes == "1;34")
                        {
                            currentColor = ImVec4(0.3f, 0.5f, 1.0f, 1.0f);
                        }

                        // Bold Magenta
                        else if (codes == "1;35")
                        {
                            currentColor = ImVec4(1.0f, 0.3f, 1.0f, 1.0f);
                        }

                        pos = m + 1;
                        textStart = pos;
                    }
                    else
                    {
                        pos++;
                    }
                }

                // باقی متن
                if (textStart < input.size())
                {
                    result.segments.push_back({
                        input.substr(textStart),
                        currentColor
                        });
                }

                return result;
            }
            void DrawConsoleLine(const std::string& line)
            {
                DATA::ConsoleLine parsed = ParseANSI(line);

                bool first = true;

                for (const auto& segment : parsed.segments)
                {
                    if (!first)
                        ImGui::SameLine(0.0f, 0.0f);

                    ImGui::TextColored(
                        segment.color,
                        "%s",
                        segment.text.c_str()
                    );

                    first = false;
                }
            }
		};

		Application* CreateApplication(int argc, char** argv)
		{
			ApplicationSpecification spec;
			spec.Name = "Umbrella Engine";
			spec.windowapp = Engine::API::InitWindow(1600, 900, "Umbrella Engine");

			Application* app = new Application(spec);
			app->PushLayer<ExampleLayer>();
			app->SetMenubarCallback([app]()
				{
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("Exit"))
						{
							app->Close();
						}
						ImGui::EndMenu();
					}
				});
			return app;
		}

	}
}