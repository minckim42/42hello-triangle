# Vertex

```c++
struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coord;
};


vector<Vertex> vertices = {
	{{ 0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.0f}, {1.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.0f}, {0.0f, 1.0f}},
};

...
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coord));
...

```


# 간단한 이미지처리 라이브러리
- https://github.com/nothings/stb.git
- 헤더 하나만 임베드해서 사용 할 수 있음

```c++
// header files
#include <stb_image.h>
```

```c++
// source file
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
```

# 텍스처
1. 텍스처 객체 생성 및 바인딩
```c++
GLuint texture_id;
glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &texture_id);
```
예전에는
```c++
glGenTextures(1, &texture_id);
glBindTexture(GL_TEXTURE_2D, texture_id);
```

2. 래핑/필터링 방법 설정
```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

- ```GL_TEXTURE_WRAP_S```: x축
- ```GL_TEXTURE_WRAP_T```: y축
- ```GL_REPEAT```: 이미지를 바둑판처럼 반복(기본값)
- ```GL_MIRRORED_REPEAT```: 반전해서 반복
- ```GL_CLAMP_TO_EDGE```: 가장자리 색 반복
- ```GL_CLAMP_TO_BORDER```: 딱 0 ~ 1 사이만 출력

- ```GL_TEXTURE_MIN_FILTER```: 필터링
- ```GL_TEXTURE_MAG_FILTER```: 샘플링
- ```GL_LINEAR```: 선형 보간
- ```GL_NEAREST```: 가장 가까운 값
- ```GL_NEAREST_MIPMAP_NEAREST```: 
- ```GL_LINEAR_MIPMAP_NEAREST```: 
- ```GL_NEAREST_MIPMAP_LINEAR```: 
- ```GL_LINEAR_MIPMAP_LINEAR```: 

3. 텍스처 데이터를 디바이스에 전송
```c++
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
```
2D텍스처, 레벨, 디바이스에서 쓸 색 포맷, 너비, 높이, 경계, 원본의 색 포맷, 원본의 픽셀 타입, 데이터포인터

4. 밉맵 생성
```c++
glGenerateMipmap(GL_TEXTURE_2D);
```

5. 랜더링 루프에서
```c++
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture_id);
glUniform1i(glGetUniformLocation(shader_program, "texture_id"), 0);
```