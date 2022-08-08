xcopy   /y      .\Engine\Bin\Engine.lib         .\Reference\Librarys\
xcopy   /y      .\Engine\ThirdPartyLib\*.lib   .\Reference\Librarys\
xcopy   /y      .\Engine\Bin\Engine.dll         .\Client\Bin\
xcopy   /y/s   .\Engine\Public\*.h            .\Reference\Headers\
xcopy   /y/s   .\Engine\Public\*.hpp            .\Reference\Headers\
xcopy   /y/s   .\Engine\Public\*.inl            .\Reference\Headers\
pause



rem   xcopy   /옵션   .복사해야할 파일또는폴더   .복사받을 폴더. 
rem   /y   복사하여 붙여넣기
rem   /s   하위폴더파일까지 전부복사