# 3D 모델을 불러오기
### assimp 라이브러리 이용하기
- https://github.com/assimp/assimp
- CMake에 추가하기
```cmake
  
add_subdirectory(${ASSIMP_DIR})
  
target_link_libraries(06_model_load
    ...
    assimp
    ...
)

target_include_directories(06_model_load
    ...
    ${ASSIMP_DIR}/include
    ...
)
```

- Scene
  - Assimp에서 파일을 로딩하는 것은 ```aiScene scene```으로부터 시작한다.
  - scene에는 메쉬, 노드(모델은 트리구조로 되어 있음.), 재질, 애니메이션 등의 정보가 저장되어 있다.
- 텍스처
  - ```scene```이 초기화 될 때, 텍스처는 불러오지 않는 것 같다. 나중에 재질을 저장할 때 불러오자.
  - 텍스처는 여러 재질에서 공유할 수도 있다는 것을 감안해서 저장하자.
- 재질
  - scene의 ```mMaterials```에 저장되어 있다.
  - 재질은 텍스처를 제외하고 불러오는 것 같다.
  - 앰비언트, 디퓨즈, 스펙큘러 외에도 노멀맵 등 다양한 정보를 담고 있다.
  - 텍스처와 마찬가지로 재질 역시 공유될 수 있으므로, 이점을 감안해서 저장하자.
- 메쉬
  - 메쉬는 각 노드에 저장되는 것이 아니라, ```scene```에 저장되어 있다. ```scene```의 ```mMeshes```는 ```aiMesh```의 배열이다.
  - 메쉬 역시 여러 노드에서 공유될 수 있으므로, 이점을 감안해서 저장하자.
  - 벡터에 ```scene```의 메쉬를 저장해도 좋지만, 자료를 추가/삭제할 때 인덱스의 위치가 바뀔 수 있으므로 주의한다.
  - 벡터를 사용하려면 인덱스에 관계없게끔 공유포인터로 메쉬를 저장한다.
  - ```mVertices```
    - 위치좌표
  - ```mNormals```
    - 법선벡터
  - ```mFace```
    - 면인데, ```importer```에 ```aiProcess_Triangulate```플래그를 보냈기 때문에, 삼각형으로 저장한다.
    - 면을 이루는 버텍스의 인덱스가 담겨있는데, 삼각형이므로 세 개 담겨있다.
  - 이 외에도 ```bone``` 등 애니메이션에 필요한 여러 데이터가 담겨있다.
- 노드
  - 노드는 단지 scene의 mMeshes상의 인덱스를 저장할 뿐이다.
  - mTransform
    - 부모 노드로부터의 상대적인 위치나 회전 등을 담고 있는 행렬