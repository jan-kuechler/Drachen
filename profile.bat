cp Profile/Drachen.exe Drachen_Profile.exe

SET pt="C:\Program Files (x86)\Microsoft Visual Studio 10.0\Team Tools\Performance Tools"

%pt%\vsperfcmd /start:sample /output:profile.vsp
%pt%\vsperfcmd /launch:".\Drachen_Profile.exe"
%pt%\vsperfcmd /shutdown

%pt%\vsperfreport /summary:all profile.vsp

pause
