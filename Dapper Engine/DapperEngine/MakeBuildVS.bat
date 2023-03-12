mkdir "..\..\..\Rushing Tide"
xcopy ..\Release\*.exe "..\..\..\Rushing Tide" /Y
xcopy ..\Release\*.dll "..\..\..\Rushing Tide" /Y
xcopy .\Assets\Music\*.* "..\..\..\Rushing Tide\Assets\Music\*.*" /Y /S
xcopy .\Assets\SFX\*.* "..\..\..\Rushing Tide\Assets\SFX\*.*" /Y /S
xcopy .\Assets\Texture\*.* "..\..\..\Rushing Tide\Assets\Texture\*.*" /Y /S
xcopy .\Assets\Spine\*.* "..\..\..\Rushing Tide\Assets\Spine\*.*" /Y /S
xcopy .\Assets\FMOD Banks\*.* "..\..\..\Rushing Tide\Assets\FMOD Banks\*.*" /Y /S
xcopy .\Assets\FMOD Banks\Desktop\*.* "..\..\..\Rushing Tide\Assets\FMOD Banks\Desktop\*.*" /Y /S
xcopy .\Shaders\*.* "..\..\..\Rushing Tide\Shaders\*.*" /Y /S
xcopy .\Data\*.* "..\..\..\Rushing Tide\Data\*.*" /Y /S
pause