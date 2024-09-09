#pragma once
#include <string>
#include <vector>

namespace Espresso
{
	struct ApplicationID
	{
	public:
		std::string Domain{"com"};
		std::string Organization;
		std::string Name;

		[[nodiscard]] inline auto GetCompoundID() const -> std::string
		{
			return Domain + "." + Organization + "." + Name;
		}
	};

	struct EnvironmentInfo
	{
	public:
		std::string WorkingDirectory;
		std::string ExecutablePath;
		std::string AssetsDirectory;
		std::vector<std::string> CommandLineArgs;
	};
}