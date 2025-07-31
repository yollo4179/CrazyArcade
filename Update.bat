:: 명령어			옵션		원본 파일의 위치					사본 파일을 저장할 위치

::For Client

xcopy			/y		.\Engine\Bin\Debug\Engine.dll				.\Client\Bin\Debug\
xcopy			/y		.\Engine\Bin\Release\Engine.dll				.\Client\Bin\Release\
xcopy 			/y/s	.\Engine\Bin\Engine.pdb				.\Client\Bin\
xcopy			/y		.\Engine\Bin\Debug\Engine.lib				.\EngineSDK\Lib\Debug\
xcopy			/y		.\Engine\Bin\Release\Engine.lib				.\EngineSDK\Lib\Release\

::From EngineSDK

xcopy			/y		.\Engine\ThirdPartyLibrary\*.lib		.\EngineSDK\Lib\Debug\
xcopy			/y		.\Engine\ThirdPartyLibrary\*.lib		.\EngineSDK\Lib\Release\

xcopy			/y		.\Engine\ThirdPartyLibrary\imgui\*.lib		.\EngineSDK\Lib\Debug\
xcopy			/y		.\Engine\ThirdPartyLibrary\imgui\*.lib		.\EngineSDK\Lib\Release\


xcopy			/y/s	.\Engine\Public\*.*					.\EngineSDK\Inc\
xcopy 		/y 	.\Engine\HLSL\*.*				.\EngineSDK\HLSL\