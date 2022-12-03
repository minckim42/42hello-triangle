#pragma once
#ifndef GLM_ENABLE_EXPERIMENTAL
# define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"
#include "model.h"
#include "material.h"
#include <filesystem>
#include <map>
#include <iostream>

void load_texture_from_scene(
   const aiScene* scene,
   const std::filesystem::path& parent_directory,
   Texture::Container& texture_container);

Texture::Ptr load_texture_from_material(
   aiTextureType ai_texture_type,
   aiMaterial* ai_material,
   const std::filesystem::path& parent_directory,
   Texture::Container& texture_container);

void load_material_from_scene(
   const aiScene* scene,
   const std::filesystem::path& parent_directory,
   Texture::Container& texture_container,
   Material::Container& material_container);

std::shared_ptr<Mesh> load_mesh(
   const aiScene* scene,
   const aiMesh* ai_mesh,
   const Material::Container& material_container);

void load_mesh_from_scene(
   const aiScene* scene,
   const Material::Container& material_container,
   Mesh::Container& meshes);

Model::Ptr load_node_from_scene(
   const aiScene* scene,
   const aiNode* ai_node,
   const Mesh::Container& meshes);

Model::Ptr assimp_load(
   const std::filesystem::path& path,
   Texture::Container& output_textures,
   Material::Container& output_materials,
   Mesh::Container& output_meshes);

glm::mat4 to_glm(const aiMatrix4x4& v);
glm::vec3 to_glm(const aiColor3D& v);