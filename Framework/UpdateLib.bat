xcopy   /y      .\Engine\Bin\Engine.lib         .\Reference\Librarys\
xcopy   /y      .\Engine\ThirdPartyLib\*.lib   .\Reference\Librarys\
xcopy   /y      .\Engine\Bin\Engine.dll         .\Client\Bin\
xcopy   /y/s   .\Engine\Public\*.h            .\Reference\Headers\
xcopy   /y/s   .\Engine\Public\*.hpp            .\Reference\Headers\
xcopy   /y/s   .\Engine\Public\*.inl            .\Reference\Headers\
pause



rem   xcopy   /�ɼ�   .�����ؾ��� ���϶Ǵ�����   .������� ����. 
rem   /y   �����Ͽ� �ٿ��ֱ�
rem   /s   �����������ϱ��� ���κ���