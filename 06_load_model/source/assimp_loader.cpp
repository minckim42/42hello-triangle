#include "assimp_loader.h"

void load_texture_from_scene(
   const aiScene* scene,
   const std::filesystem::path& parent_directory,
   Texture::Container& texture_container)
{
   for (int i = 0; i < scene->mNumTextures; ++i)
   {
      aiTexture* ai_texture = scene->mTextures[i];
      std::string texture_path = parent_directory / ai_texture->mFilename.C_Str();
      Texture::Container::iterator it = texture_container.find(texture_path);
      std::cout << '[' << texture_path << ']' << std::endl;
      if (it != texture_container.end()) // 이미 텍스처가 로드 된 경우
      {
         continue;
      }
      texture_container[texture_path] = std::make_shared<Texture>(texture_path);
   }
}

Texture::Ptr load_texture_from_material(
   aiTextureType ai_texture_type,
   aiMaterial* ai_material,
   const std::filesystem::path& parent_directory,
   Texture::Container& texture_container)
{
   int texture_count = ai_material->GetTextureCount(ai_texture_type);
   if (!texture_count)
   {
      return nullptr;
   }
   aiString ai_texture_path;
   ai_material->GetTexture(ai_texture_type, 0, &ai_texture_path); // 1개만 있다고 가정하고 첫 번째 텍스처를 로드
   if (ai_texture_path.length == 0)
   {
      return nullptr;
   }
   std::filesystem::path texture_path = parent_directory / ai_texture_path.C_Str();
   Texture::Container::iterator it_texture = texture_container.find(texture_path);
   if (it_texture == texture_container.cend()) // assimp가 처음에는 텍스처를 직접 로드하지 않을 것이므로, 거의 항상 호출됨
   {
      texture_container[texture_path] = std::make_shared<Texture>(texture_path);
      return texture_container[texture_path];
   }
   return it_texture->second;
}

void load_material_from_scene(
   const aiScene* scene,
   const std::filesystem::path& parent_directory,
   Texture::Container& texture_container,
   Material::Container& material_container)
{
   for (int i = 0; i < scene->mNumMaterials; ++i)
   {
      aiMaterial* ai_material = scene->mMaterials[i];
      std::string material_name = ai_material->GetName().C_Str();
      if (material_name.empty())
      {
         continue;
      }
      Material::Container::iterator it = material_container.find(material_name);
      if (it != material_container.end()) // 이미 재질이 로드 된 경우
      {
         continue;
      }
      //material_container[material_name] =
      Material::Ptr material = std::make_shared<Material>(
         load_texture_from_material(aiTextureType_AMBIENT, ai_material, parent_directory, texture_container),
         load_texture_from_material(aiTextureType_DIFFUSE, ai_material, parent_directory, texture_container),
         load_texture_from_material(aiTextureType_SPECULAR, ai_material, parent_directory, texture_container)
      );

      aiColor3D ai_color;
      ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ai_color);
      material->ka = to_glm(ai_color);
      ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
      material->kd = to_glm(ai_color);
      ai_material->Get(AI_MATKEY_COLOR_SPECULAR, ai_color);
      material->ks = to_glm(ai_color);

      material_container[material_name] = material;
   }
}

std::shared_ptr<Mesh> load_mesh(
   const aiScene* scene,
   const aiMesh* ai_mesh,
   const Material::Container& material_container)
{
   std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

   // position
   mesh->vertices.reserve(ai_mesh->mNumVertices);
   for (int i = 0; i < ai_mesh->mNumVertices; ++i)
   {
      mesh->vertices.emplace_back();
      Vertex& vertex = mesh->vertices.back();

      aiVector3D& ai_vertex = ai_mesh->mVertices[i];
      vertex.position = {ai_vertex.x, ai_vertex.y, ai_vertex.z};

      if (ai_mesh->HasNormals())
      {
         aiVector3D& ai_normal = ai_mesh->mNormals[i];
         vertex.normal = {ai_normal.x, ai_normal.y, ai_normal.z};
      }
      else
      {
         vertex.normal = {0.0f, 0.0f, 1.0f};
      }

      if (ai_mesh->mTextureCoords[0])
      {
         aiVector3D& ai_texture_coord = ai_mesh->mTextureCoords[0][i];
         vertex.texture_coord = {ai_texture_coord[0], ai_texture_coord[1]};
      }
      else
      {
         vertex.texture_coord = {0.0f, 0.0f};
      }
   }

   // indices
   // aiProcess_Triangulate를 호출했으므로, face 하나에 인덱스가 3개여야만 함.
   // 아니면 어차피 glDraw...(GL_TRIANGLE, ....)할 때 실패함
   mesh->indices.reserve(ai_mesh->mNumFaces * 3);
   for (int idx_face = 0; idx_face < ai_mesh->mNumFaces; ++idx_face)
   {
      aiFace& ai_face = ai_mesh->mFaces[idx_face];
      for (int j = 0; j < ai_face.mNumIndices; ++j)
      {
         mesh->indices.push_back(ai_face.mIndices[j]);
      }
   }

   // material
   const aiString& material_name = scene->mMaterials[ai_mesh->mMaterialIndex]->GetName();
   Material::Container::const_iterator it_material = material_container.find(material_name.C_Str());

   if (it_material != material_container.cend())
   {
      mesh->material = it_material->second;
   }
   else
   {
      std::cerr << "material not found: " << material_name.C_Str() << std::endl;
   }

   mesh->setup();
   return mesh;
}

void load_mesh_from_scene(
   const aiScene* scene,
   const Material::Container& material_container,
   Mesh::Container& meshes)
{
   for (int idx_mesh = 0; idx_mesh < scene->mNumMeshes; ++idx_mesh)
   {
      aiMesh* ai_mesh = scene->mMeshes[idx_mesh];
      meshes[ai_mesh->mName.C_Str()] = load_mesh(scene, ai_mesh, material_container);
   }
}

Model::Ptr load_node_from_scene(
   const aiScene* scene,
   const aiNode* ai_node,
   const Mesh::Container& meshes)
{
   Model::Ptr model = std::make_shared<Model>();

   model->meshes.reserve(ai_node->mNumMeshes);
   for (int i = 0; i < ai_node->mNumMeshes; ++i)
   {
      unsigned int idx_mesh_from_scene = ai_node->mMeshes[i];
      aiMesh* ai_mesh = scene->mMeshes[idx_mesh_from_scene];
      aiString& mesh_name = ai_mesh->mName;
      Mesh::Container::const_iterator it_mesh = meshes.find(mesh_name.C_Str());
      if (it_mesh == meshes.cend())
      {
         std::cerr << "Warning: mesh not found: " << mesh_name.C_Str() << std::endl;
      }
      else
      {
         model->meshes.emplace_back(it_mesh->second);
      }
   }

   model->children.reserve(ai_node->mNumChildren);
   for (int i = 0; i < ai_node->mNumChildren; ++i)
   {
      aiNode* ai_child = ai_node->mChildren[i];
      model->children.emplace_back(load_node_from_scene(scene, ai_child, meshes));
   }
   model->transform = to_glm(ai_node->mTransformation);
   return model;
}

Model::Ptr assimp_load(
   const std::filesystem::path& path,
   Texture::Container& output_textures,
   Material::Container& output_materials,
   Mesh::Container& output_meshes)
{
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      throw std::runtime_error(std::string("Error: ") + importer.GetErrorString());
   }
   std::string parent_path = path.parent_path();

   load_texture_from_scene(scene, parent_path, output_textures);
   load_material_from_scene(scene, parent_path, output_textures, output_materials);
   load_mesh_from_scene(scene, output_materials, output_meshes);

   return load_node_from_scene(scene, scene->mRootNode, output_meshes);
}


glm::mat4 to_glm(const aiMatrix4x4& v)
{
   return {
      v[0][0], v[1][0], v[2][0], v[3][0],
      v[0][1], v[1][1], v[2][1], v[3][1],
      v[0][2], v[1][2], v[2][2], v[3][2],
      v[0][3], v[1][3], v[2][3], v[3][3],
   };
}

glm::vec3 to_glm(const aiColor3D& v)
{
   return {v[0], v[1], v[2]};
}
