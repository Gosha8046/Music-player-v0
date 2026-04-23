# MusicPlayer — C++20 + Qt6

Минималистичный музыкальный проигрыватель с тёмной темой.

## Возможности

- Добавление файлов (mp3, flac, wav, ogg, aac, m4a, wma, opus)
- Добавление целой папки (рекурсивно)
- Плейлист с двойным кликом для воспроизведения
- Кнопки: предыдущий / пауза / стоп / следующий
- Прогресс-бар с перемоткой
- Регулятор громкости
- Автоматический переход к следующему треку
- Тёмная тема (Catppuccin Mocha)

## Требования

| Компонент | Версия |
|-----------|--------|
| Qt        | 6.4+   |
| CMake     | 3.20+  |
| MSVC      | 2022   |
| C++       | 20     |

## Сборка (Windows)

```bat
cmake -B build -G "Visual Studio 17 2022" -A x64 ^
      -DCMAKE_PREFIX_PATH="C:/Qt/6.7.0/msvc2019_64"
cmake --build build --config Release
cd build\Release
windeployqt MusicPlayer.exe --no-translations --multimedia
```

## Структура

```
MusicPlayer/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── MainWindow.h/.cpp     — главное окно, меню, плеер
│   ├── PlayerControls.h/.cpp — панель управления
│   └── PlaylistModel.h/.cpp  — модель плейлиста
└── resources/
    └── resources.qrc
```
