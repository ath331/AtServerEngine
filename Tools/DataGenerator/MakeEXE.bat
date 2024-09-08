pyinstaller --onefile DataGenerator.py

copy dist\DataGenerator.exe .\
del /Q dist\DataGenerator.exe

#pause