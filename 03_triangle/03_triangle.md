# 파일 로더
셰이더 파일을 읽을 때 쓸 간단한 코드
```c++
string file_loader(const filesystem::path& file_name)
{
    ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        throw string("Error: Failed to open file: ") + file_name.string();
    }
    stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return ss.str();
}
```

# 셰이더 컴파일
```c++
GLuint complie_shader(const filesystem::path& vertex_shader_path, const filesystem::path& fragment_shader_path)
{
    GLuint shader_program;
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        // 버텍스 셰이더
        {
            vertex_shader = glCreateShader(GL_VERTEX_SHADER); // 셰이더 오브젝트 생성
            string shader_source = file_loader(vertex_shader_path);
            const char* shader_source_ptr = shader_source.data();
            glShaderSource(vertex_shader, 1, &shader_source_ptr, nullptr); // 셰이더 소스파일 로드
            glCompileShader(vertex_shader); // 셰이더 컴파일
            int success;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success); // 잘 됐는지 확인
            if (!success)
            {
                char log[512];
                glGetShaderInfoLog(vertex_shader, 512, nullptr, log); // 로그 확인
                throw string("Error: Failed to compile vertex shader:\n") + log;
            }
        }
        // 프래그먼트 셰이더
        {
            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            string shader_source = file_loader(fragment_shader_path);
            const char* shader_source_ptr = shader_source.data();
            glShaderSource(fragment_shader, 1, &shader_source_ptr, nullptr);
            glCompileShader(fragment_shader);
            int success;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char log[512];
                glGetShaderInfoLog(fragment_shader, 512, nullptr, log);
                throw string("Error: Failed to compile fragment shader:\n") + log;
            }
        }
        // 셰이더 프로그램 링크
        {
            shader_program = glCreateProgram();
            glAttachShader(shader_program, vertex_shader);
            glAttachShader(shader_program, fragment_shader);
            glLinkProgram(shader_program);
            int success;
            glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char log[512];
                glGetProgramInfoLog(shader_program, 512, nullptr, log);
                throw string("Error: Failed to link shader program:\n") + log;
            }
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    return shader_program;
}
```

# 버텍스 어레이
- 버텍스버퍼와 엘러먼트버퍼 묶음
- 버텍스버퍼
	- 버텍스 배열
	- 버텍스 하나의 크기가 얼마나 되는지, 버텍스 구조체는 어떤 구조인지 디바이스에 알려줘야 함
- 엘러먼트 버퍼
	- 프리미티브를 이루는 버텍스의 인덱스를 담은 배열
	- 앞에서부터 차례대로 프리미티브 하나를 구성
- ```glGen*``` vs ```glCreate*``` : 초기화 시점의 차이
	- ```glGen*```은 오브젝트를 곧바로 생성하지 않고, 실제로 필요할 때 생성함
	- ```glCreate*```는 오브젝트를 곧바로 생성

- ```glBind*``` : 컨텍스트에 인자로 보낸 오브젝트를 바인딩
- ```glBufferData()``` : 데이터 전송
```c++
glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
```
종류, 버퍼 크기, 버퍼의 주소, 빈도
- ```glVertexAttribPointer```: 버퍼의 구조
```c++
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
```
레이아웃 번호, 요소의 개수, 자료형, 정규화여부, 스트라이드 크기, 오프셋