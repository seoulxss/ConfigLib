#include <codecvt>
#include <iostream>

#include "../../ConfigLib/src/Config.h"

// void* operator new(uintptr_t size)
// {
// 	static int count = 0;
// 	count += 1;
// 	std::cout << "Allocated: " << count << std::endl;
// 	return malloc(size);
// }

int main()
{
	ConfigLib::ConfigManager con(LR"(C:\SeoulessCheese\CSGO\)");
	con.AddConfig(L"Example.json");
	auto& config = con.GetConfig(L"Example.json");

	config.AddInteger("key1", 1);
	config.AddInteger("key2", 2);

	config.AddFloat("f1", 1.0);
	config.AddFloat("f2", 999.9999f);

	config.AddArray("a1", { 255, 255, 255, 255 });
	config.AddArray("a2", { "gay", "gay2", "gay3" });

	config.AddBool("b1", true);
	config.AddBool("b2", false);

	config.AddString("str1", "HELLO");
	config.AddString("str2", "HELLO2");

	config.GetValue<int>("key1");

	config.SaveConfig();


	con.AddConfig(L"GayConf.json");
	auto& config2 = con.GetConfig(L"GayConf.json");
	config2.AddBool("HAY", true);
	config2.SaveConfig();

	con.DeleteConfig(L"GayConf.json");

	con.DeleteAllConfigs();

	return 0;
}
