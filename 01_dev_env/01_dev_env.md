# 윈도우즈
## Chocolatey 설치
- Powershell 관리자권한으로 실행
- https://chocolatey.org/
	- Get Started
		```powershell
		Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
		```
- 잘 설치되었는지 ```choco -v```로 확인

## MinGW
```powershell
choco install mingw
```

## Makefile 설치하기
```powershell
choco install make
```


## CMake 설치하기
```powershell
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
refreshenv
```
powershell 재시작

잘 설치되었는지 확인:
```powershell
cmake -version
```


빌드할 때
```powershell
cmake .. -G "MinGW Makefiles"
```

# 리눅스
## CMake 설치
```bash
sudo apt install cmake
```

# 맥OS
## CMake 설치
```bash
brew install cmake
```

# CMake 테스트
## CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.11)

project(
	hello_cmake
)

add_executable(
	hello_cmake_executable
	main.cpp
)
```

## main.cpp
```c++
#include <iostream>

int main()
{
	std::cout << "Hello World!" << std::endl;
}
```