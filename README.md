# UnityResolveVR

> Credits to https://github.com/issuimo for UnityResolve, I just modified it to use a hpp for symbols, it can be found at: https://github.com/issuimo/UnityResolve.hpp/tree/main

> ### 类型 (Type)

> - \[X] Camera

> - \[X] Transform

> - \[X] Component

> - \[X] Object (Unity)

> - \[X] LayerMask

> - \[X] Rigidbody

> - \[x] MonoBehaviour

> - \[x] Renderer

> - \[x] Mesh

> - \[X] Behaviour

> - \[X] Physics

> - \[X] GameObject

> - \[X] Collider

> - \[X] Vector4

> - \[X] Vector3

> - \[X] Vector2

> - \[X] Quaternion

> - \[X] Bounds

> - \[X] Plane

> - \[X] Ray

> - \[X] Rect

> - \[X] Color

> - \[X] Matrix4x4

> - \[X] Array

> - \[x] String

> - \[x] Object (C#)

> - \[X] Type (C#)

> - \[X] List

> - \[X] Dictionary

> - \[X] Animator

> - \[X] CapsuleCollider

> - \[X] BoxCollider

> - \[X] Time

> - \[X] FieldInfo

> - More...

> ### 功能 (Function)

> - \[X] Mono注入 (Mono Inject)

> - \[X] DumpToFile

> - \[X] 附加线程 (Thread Attach / Detach)

> - \[X] 修改静态变量值 (Modifying the value of a static variable)

> - \[X] 获取对象 (Obtaining an instance)

> - \[X] 创建C#字符串 (Create C# String)

> - \[X] 创建C#数组 (Create C# Array)

> - \[X] 创建C#对象 (Create C# instance)

> - \[X] 世界坐标转屏幕坐标/屏幕坐标转世界坐标 (WorldToScreenPoint/ScreenToWorldPoint)

> - \[X] 获取继承子类的名称 (Get the name of the inherited subclass)

> - \[X] 获取函数地址(变量偏移) 及调用(修改/获取) (Get the function address (variable offset) and invoke (modify/get))

> - \[x] 获取Gameobject组件 (Get GameObject component)

> - More...

<hr>

<h3 align="center">功能使用 (How to use)</h3>

<hr>



\#### 使用GLM (use glm)

\[GLM](https://github.com/g-truc/glm)

> ``` C++

> #define USE\_GLM

> #include "UnityResolve.hpp"

> ```



\#### 更改平台 (Change platform)

> ``` c++

> #define WINDOWS\_MODE 1 // 如果需要请改为 1 | 1 if you need

> #define ANDROID\_MODE 0

> #define LINUX\_MODE 0

> ```



\#### 初始化 (Initialization)

> ``` c++

> UnityResolve::Init(GetModuleHandle(L"GameAssembly.dll | mono.dll"), UnityResolve::Mode::Mono);

> // Linux or Android

> UnityResolve::Init(dlopen(L"GameAssembly.so | mono.so", RTLD\_NOW), UnityResolve::Mode::Mono);

> ```

> 参数1: dll句柄 \\

> Parameter 1: DLL handle \\

> 参数2: 使用模式 \\

> Parameter 2: Usage mode

> - Mode::Il2cpp

> - Mode::Mono



\#### 附加线程 (Thread Attach / Detach)

> ``` c++

> // C# GC Attach

> UnityResolve::ThreadAttach();

> 

> // C# GC Detach

> UnityResolve::ThreadDetach();

> ```



\#### Mono注入 (Mono Inject)

> ``` c++

> UnityResolve::AssemblyLoad assembly("./MonoCsharp.dll");

> UnityResolve::AssemblyLoad assembly("./MonoCsharp.dll", "MonoCsharp", "Inject", "MonoCsharp.Inject:Load()");

> ```



\#### 获取函数地址(变量偏移) 及调用(修改/获取) (Get the function address (variable offset) and invoke (modify/get))

> ``` c++

> const auto assembly = UnityResolve::Get("assembly.dll | 程序集名称.dll");

> const auto pClass   = assembly->Get("className | 类名称");

>                    // assembly->Get("className | 类名称", "\*");

>                    // assembly->Get("className | 类名称", "namespace | 空间命名");

> 

> const auto field       = pClass->Get<UnityResolve::Field>("Field Name | 变量名");

> const auto fieldOffset = pClass->Get<std::int32\_t>("Field Name | 变量名");

> const int  time        = pClass->GetValue<int>(obj Instance | 对象地址, "time");

>                       // pClass->GetValue(obj Instance\*, name);

>                        = pClass->SetValue<int>(obj Instance | 对象地址, "time", 114514);

>                       // pClass->SetValue(obj Instance\*, name, value);

> const auto method      = pClass->Get<UnityResolve::Method>("Method Name | 函数名");

>                       // pClass->Get<UnityResolve::Method>("Method Name | 函数名", { "System.String" });

>                       // pClass->Get<UnityResolve::Method>("Method Name | 函数名", { "\*", "System.String" });

>                       // pClass->Get<UnityResolve::Method>("Method Name | 函数名", { "\*", "", "System.String" });

>                       // pClass->Get<UnityResolve::Method>("Method Name | 函数名", { "\*", "System.Int32", "System.String" });

>                       // pClass->Get<UnityResolve::Method>("Method Name | 函数名", { "\*", "System.Int32", "System.String", "\*" });

>                       // "\*" == ""

> 

> const auto functionPtr = method->function;

> 

> const auto method1 = pClass->Get<UnityResolve::Method>("method name1 | 函数名称1");

> const auto method2 = pClass->Get<UnityResolve::Method>("method name2 | 函数名称2");

> 

> method1->Invoke<int>(114, 514, "114514");

> // Invoke<return type>(args...);

>

> // Cast<return type, args...>(void);

> // Cast(UnityResolve::MethodPointer<return type, args...>\&);

> const UnityResolve::MethodPointer<void, int, bool> ptr = method2->Cast<void, int, bool>();

> ptr(114514, true);

>

> UnityResolve::MethodPointer<void, int, bool> add;

> ptr = method1->Cast(add);

>

> std::function<void(int, bool)> add2;

> method->Cast(add2);

>

> UnityResolve::Field::Variable<Vector3, Player> syncPos;

> syncPos.Init(pClass->Get<UnityResolve::Field>("syncPos"));

> auto pos = syncPos\[playerInstance];

> auto pos = syncPos.Get(playerInstance);

>

> ```



\#### 转存储到文件 (DumpToFile)

> ``` C++

> UnityResolve::DumpToFile("./output/");

> ```



\#### 创建C#字符串 (Create C# String)

> ``` c++

> const auto str     = UnityResolve::UnityType::String::New("string | 字符串");

> std::string cppStr = str.ToString();

> ```



\#### 创建C#数组 (Create C# Array)

> ``` c++

> const auto assembly = UnityResolve::Get("assembly.dll | 程序集名称.dll");

> const auto pClass   = assembly->Get("className | 类名称");

> const auto array    = UnityResolve::UnityType::Array<T>::New(pClass, size);

> std::vector<T> cppVector = array.ToVector();

> ```



\#### 创建C#对象 (Create C# instance)

> ``` c++

> const auto assembly = UnityResolve::Get("assembly.dll | 程序集名称.dll");

> const auto pClass   = assembly->Get("className | 类名称");

> const auto pGame    = pClass->New<Game\*>();

> ```



\#### 获取对象 (Obtaining an instance)

> ``` c++

> const auto assembly = UnityResolve::Get("assembly.dll | 程序集名称.dll");

> const auto pClass   = assembly->Get("className | 类名称");

> std::vector<Player\*> playerVector = pClass->FindObjectsByType<Player\*>();

> // FindObjectsByType<return type>(void);

> playerVector.size();

> ```



\#### 世界坐标转屏幕坐标/屏幕坐标转世界坐标 (WorldToScreenPoint/ScreenToWorldPoint)

> ``` c++

> Camera\* pCamera = UnityResolve::UnityType::Camera::GetMain();

> Vector3 point   = pCamera->WorldToScreenPoint(Vector3, Eye::Left);

> Vector3 world   = pCamera->ScreenToWorldPoint(point, Eye::Left);

> ```



\#### 获取继承子类的名称 (Get the name of the inherited subclass)

> ``` c++

> const auto assembly = UnityResolve::Get("UnityEngine.CoreModule.dll");

> const auto pClass   = assembly->Get("MonoBehaviour");

> Parent\* pParent     = pClass->FindObjectsByType<Parent\*>()\[0];

> std::string child   = pParent->GetType()->GetFullName();

> ```



\#### 获取Gameobject组件 (Get GameObject component)

> ``` c++

> std::vector<T\*> objs = gameobj->GetComponents<T\*>(UnityResolve::Get("assembly.dll")->Get("class")));

>                     // gameobj->GetComponents<return type>(Class\* component)

> std::vector<T\*> objs = gameobj->GetComponentsInChildren<T\*>(UnityResolve::Get("assembly.dll")->Get("class")));

>                     // gameobj->GetComponentsInChildren<return type>(Class\* component)

> std::vector<T\*> objs = gameobj->GetComponentsInParent<T\*>(UnityResolve::Get("assembly.dll")->Get("class")));

>                     // gameobj->GetComponentsInParent<return type>(Class\* component)

> ```



\#### 类映射使用 (ClassMap Usage)

> ``` c++



> auto func = map\["assembly.dll"]\["Namespace"]\["Class"]\["MethodName"].get(); // returns UnityResolve::Method\*



> auto field = map\["assembly.dll"]\["Namespace"]\["Class"].field("fieldName").getField(); // returns UnityResolve::Field\*



> auto field\_value = map\["assembly.dll"]\["Namespace"]\["Class"].field("fieldName").get<return type>();



> or


> auto field\_value = map\["assembly.dll"]\["Namespace"]\["Class"].field("fieldName").get<return type>(instance); // returns the return type.



> auto method\_value = map\["assembly.dll"]\["Namespace"]\["Class"]\["MethodName"].call<Return Type>(instance, arg1, arg2, arg3); // returns the type. and also args and instance are not needed, if method is static with no args just put nothing in the (), but if it is static and args, then do (instance, arg1, arg2, arg3, etc..) args can be any amount, also instance can just be put as nullptr if static.



> auto arr = map\["assembly.dll"]\["Namespace"]\["Class"].FindObjectsByType<UnityResolve::UnityType::GameObject\*>(); // this simply returns a std::vector<T>, I will add documentation on how to use this later :).



> map\["assembly.dll"]\["Namespace"]\["Class"].field("FieldName").set<bool>(nullptr, true); // must not do a auto field =, also <bool> can be anything, and nullptr is if the field is static, and then true must match the type, like if int, then 0, or any number.



> ```



That's it, hope this helps you out :)

