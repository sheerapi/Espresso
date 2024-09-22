#pragma once
#include <string>
#include <vector>

namespace Espresso
{
	struct ApplicationID
	{
	public:
		std::string Domain{"com"};
		std::string Org;
		std::string Name;

		ApplicationID(const std::string& appId);
		ApplicationID() = default;

		[[nodiscard]] inline auto GetCompoundID() const -> std::string
		{
			return Domain + "." + Org + "." + Name;
		}

		static auto CheckAppID(const std::string& appId) -> bool;
	};

	struct ApplicationInfo
	{
	public:
		std::string Organization;
		std::string Name;
		std::string Version;
		ApplicationID AppID;
	};

	struct EnvironmentInfo
	{
	public:
		std::string WorkingDirectory;
		std::string ExecutablePath;
		std::string AssetsDirectory;
		std::string RootPath;
		std::vector<std::string> CommandLineArgs;
	};
}