#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <optional>

#include <nlohmann/json.hpp>

namespace ConfigLib
{
	namespace _Config
	{


		class Config
		{
		public:
			Config(const wchar_t* ConfigName, const wchar_t* FilePath) : m_FileName(ConfigName), m_FilePath(FilePath)
			{
				m_FileStream.open(m_FilePath, std::fstream::in | std::fstream::out | std::fstream::app);

				if (m_FileStream.is_open() && m_FileStream.peek() == std::fstream::traits_type::eof())
				{
					m_FileStream.close();
				}

				if (m_FileStream.is_open() && m_FileStream.peek() != std::fstream::traits_type::eof())
				{
					m_Json = nlohmann::json::parse(m_FileStream);
					m_FileStream.close();
				}
			}

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (int)
			//! @return If it was successfully added (bool)
			bool AddInteger(const char* key, int value);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (float)
			//! @return If it was successfully added (bool)
			bool AddFloat(const char* key, float value);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (std::vector)
			//! @return If it was successfully added (bool)
			bool AddArray(const char* key, const std::vector<nlohmann::json>& values);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (bool)
			//! @return If it was successfully added (bool)
			bool AddBool(const char* key, bool value);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (const char*)
			//! @return If it was successfully added (bool)
			bool AddString(const char* key, const char* value);

			//! 
			//! @tparam _Type The returned type (int, bool, float, etc..)
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @return The value or std::nullopt
			template<typename _Type>
			std::optional<_Type> GetValue(const char* key);

			//! Simply saves the config
			//! @return If the saving was suceessfully
			bool SaveConfig();

		public:
			std::wstring m_FileName = {};
			std::wstring m_FilePath = {};
			std::fstream m_FileStream = {};
			nlohmann::json m_Json = nullptr;

		};

		template <typename _Type>
		std::optional<_Type> Config::GetValue(const char* key)
		{
			if (!m_Json.contains(key))
				return std::nullopt;

			auto it = m_Json.find(key);
			if (it != m_Json.end())
			{
				if (_Type val = it.value().get<_Type>())
					return val;
			}
			return std::nullopt;
		}

	}


	class ConfigManager
	{
	public:
		ConfigManager(const wchar_t* ConfigFolderPath);

		//! Simply adds all configs in the folder path to the Hashmap
		void GetAllConfigs();

		//! 
		//! @param FileName The config name 
		//! @return A reference to the config
		ConfigLib::_Config::Config& GetConfig(const wchar_t* FileName);

		//! 
		//! @param ConfigName The config name (needs to be unique)
		//! @return If the config was successfully added
		bool AddConfig(const wchar_t* ConfigName);

		//! 
		//! @param ConfigName The config name (needs to be unique)
		//! @return If the config was successfully removed
		bool DeleteConfig(const wchar_t* ConfigName);

		//! 
		//! @return True if all configs got deleted, false if something fails (mostly wrong path))
		bool DeleteAllConfigs();

	private:
		std::unordered_map<std::wstring, ConfigLib::_Config::Config> m_Configs;

		const wchar_t* m_FolderPath = nullptr;

	};


}
