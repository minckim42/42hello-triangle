# 파일 로더
셰이더 파일을 읽을 때 쓸 간단한 코드
```c++
string file_loader(const filesystem::path& file_name)
{
    ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        throw runtime_error("Error: Failed to open file: ") + file_name.string();
    }
    stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return ss.str();
}
```

# 셰이더 컴파일

1. 셰이더 오브젝트 생성
    ```c++
    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    ```
2. 셰이더 소스코드 
    ```c++
    glShaderSource(vertex_shader, 1, &shader_source_ptr, nullptr);
    ```
3. 셰이더 컴파일
    ```c++
    glCompileShader(vertex_shader);
    ```
4. 컴파일 성공 여부 확인
    ```c++
    int success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(vertex_shader, 512, nullptr, log);
        throw ...
    }
    ```

# 셰이더 프로그램 링크
1. 프로그램 오브젝트 생성
    ```c++
    shader_program = glCreateProgram();
    ```
2. 프로그램에 셰이더 부착
    ```c++
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    ```
3. 링크
    ```c++
    glLinkProgram(shader_program);
    ```
4. 링크 성공 여부 확인
    ```c++
    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, log);
        throw runtime_error("Error: Failed to link shader program:\n") + log;
    }
    ```
5. 셰이더 오브젝트 지우기
    - 링크를 마친 후에는 셰이더 오브젝트가 필요 없으므로 지운다.
    ```c++
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    ```


# 버텍스 어레이
- 버텍스버퍼와 엘러먼트버퍼 묶음
- 버텍스버퍼
	- 버텍스 배열
	- 버텍스 하나의 크기가 얼마나 되는지, 버텍스 구조체는 어떤 구조인지 디바이스에 알려줘야 함
- 엘러먼트 버퍼
	- 프리미티브를 이루는 버텍스의 인덱스를 담은 배열
	- 앞에서부터 차례대로 프리미티브 하나를 구성

# DSA vs NON-DSA
- DSA: Direct State Access
- ```glGen*``` vs ```glCreate*```
- NON-DSA
    - ```glGen*```
    - 객체 이름만(핸들만) 생성하고, 실제 할당은 바인딩 할 때 일어남
    - 사용하거나 수정하기 직전에 바인딩
    ```c++
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

                        // 로케이션
	glEnableVertexAttribArray(0);
                        // 로케이션, 자료형 개수, 정규화여부, 자료형 타입, 스트라이드, 오프셋
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coord));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    ```


- DSA
    - ```glCreate*```
    - 호출시 객체 할당
    - 객체 이름을 통해 스테이트에 직접 접근
    ```c++
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    glNamedBufferData(vbo, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(ebo, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));

    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribBinding(vao, 2, 0);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coord));

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ebo);
    ```