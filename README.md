<h1>What does this library do?</h1>
<p>This library is for people who want to add configs quickly and save them on the disk. If you care about performance, this is probably the wrong place. I designed this library to make it as easy as possible. If you care about performance you <b>CAN</b> still use it if you dont write that much to the json file (I mean saving with that).</p>

<h1>Requirements</h1>
<p>You need to link nhlohmann json with this project (I did use the static one with vcpkg)</p>

<h1>Example</h1>

```
	#include "ConfigLib.h"

	ConfigLib::ConfigManager con(LR"(C:\YourPath\)");
	con.AddConfig(L"Example.json");
```
<p> Now you can get the config by: </p>

```
	auto& config = con.GetConfig(L"Example.json");
```

<p> Add things </p>

```
	config.AddInteger("key1", 1);
	config.AddInteger("key2", 2);

	config.AddFloat("f1", 1.0);
	config.AddFloat("f2", 999.9999f);

	config.AddArray("a1", { 255, 255, 255, 255 });
	config.AddArray("a2", { "one", "two", "three" });

	config.AddBool("b1", true);
	config.AddBool("b2", false);

	config.AddString("str1", "HELLO");
	config.AddString("str2", "HELLO2");
```

<p>Saving etc</p>

```
config.SaveConfig();
```
<p> Deleting configs</p>



```
	con.DeleteConfig(L"Example.json");
	con.DeleteAllConfigs();
```

<h1>The problem with this library</h1>
<p>Even if this library is very small, it allocates a lot on heap (Mostly strings). That makes it sometimes slow, aswell as the json library by nlohmann itself is not that fast.</p>
