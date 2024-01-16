# TODO
- нужно получить документное дерево из директории с файлами. 

значения у ноды:
1. время доступа
2. имя пользователя
3. имя владельца
4. имя файла
5. mime type

`cd src/rpc && protoc-c --c_out=. common.proto`
...
`cmake build`
...
`export LD_LIBRARY_PATH=/usr/local/lib/`
...
`./cmake-build-debug/bin/server`