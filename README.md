<h1>Requirements</h1>
<p>You need to link nhlohmann json with this project</p>

<h1>Example</h1>

```
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
	con.DeleteConfig(L"GayConf.json");
	con.DeleteAllConfigs();
```
