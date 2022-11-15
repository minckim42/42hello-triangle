# OpenGL에 대해
## OpenGL은 API가 아니라 정의이다
- 구체적인 코드를 정의한 API가 아니다
- 단지 함수 이름과 역할의 정의일 뿐
- 마치 C, C++ 같은 언어와 같음
- 크로노스(Khronos) 재단에서 관리

## 그래픽카드와 OpenGL
- 그래픽카드 제조사마다 명령어체계가 다르다
- 하지만 OpenGL이라는 공통의 "라이브러리"로 다룰 수 있게 되어있다.

## 새로운 OpenGL
- 새로운...이라고 하기에는 벌써 10년도 넘었다!
- 옛 OpenGL은 고정된 파이프라인이 있었다.
	- 쓰기 쉽지만, 유연하지 않다.
- 새로운 버전은 파이프라인 단계마다 일어나는 일을 직접 정의해야 한다.
	- 어렵지만, 훨씬 유연하다. 그리고 빠르다. 아마도
- 우리는 물론 새로운 버전을 공부할 것이다.

## 더 새로운 OpenGL
- 더 새로운 OpenGL은 OpenGL이라는 이름을 버리고 Vulkan으로 새로 태어났다.

## Vulkan vs OpenGL
- OpenGL은 Vulkan에 비해 많은 작업들이 추상화되어 있다. 훨씬 쓰기 쉽다.
- OpenGL은 보이지 않는 곳에 "현재 상태"가 저장되어 있다.
	- 따라서 여러 쓰레드가 OpenGL 명령을 수행하면 되면 충돌이 발생할 것이다.
- Vulkan은 그런 면에서는 자유롭다.

---
# OpenGL 시작하기
---
# 창 띄우기
- 창을 띄우는 것은 OpenGL과 직접 관련있는 것은 아니지만, 창을 띄워야 뭘 확인 할 수 있으니 먼저 배운다.

## GLFW
- 크로스플랫폼 오픈소스 라이브러리
- 역할:
	- 창을 띄우기
	- 화면 출력
	- 키보드와 마우스 입출력을 다루기
- 다운로드
	- https://www.glfw.org/download.html
	- Source package 클릭해서 다운로드

# GLAD
- 크로스플랫폼 오픈소스 라이브러리
- 역할:
	- 그래픽카드상의 함수의 주소를 찾아주는 라이브러리
- 다운로드
	- https://glad.dav1d.de/
	- Language: C/C++
	- Specification: OpenGL
	- API
		- gl: Version 4.6
		- 나머지: 상관없음
	- Profile: Core
	- Extensions: ADD ALL
	- GENERATE 클릭
	- glad.zip 클릭해서 다운로드
- 프로젝트에 임베드

# GLM
- 벡터/행렬 라이브러리
- 다운로드
	- https://github.com/g-truc/glm
- 다른건 다 필요없고 glm 폴더 하나만 있어도 된다.
- 프로젝트에 임베드

# 윈도우 초기화
특별히 바꿀 것이 없음
```c++
GLFWwindow* init_window()
{
    if (!glfwInit())
    {
        throw runtime_error("Error: Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 안티엘리어싱 x4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 최대버전: 그냥 glfw 버전
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 최소버전: 그냥 glfw 버전
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 프로파일 버전: 코어
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
    if (window == NULL)
    {
        throw runtime_error("Error: Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // OpenGL 함수 포인터와 실제 함수를 매핑
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw runtime_error("Error: Failed to initialize GLAD");
    }
    return window;
}
```