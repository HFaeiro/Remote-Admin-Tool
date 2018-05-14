@echo off
C:\masm32\bin\ml /c /coff Fork.asm
C:\masm32\bin\rc -r Fork.rc
C:\masm32\bin\link /subsystem:console Fork.obj Fork.res
pause>nul