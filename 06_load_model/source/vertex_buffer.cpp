#include "vertex_buffer.h"

void set_vertex(
   GLuint& vao,
   GLuint& vbo,
   GLuint& ebo,
   const std::vector<Vertex>& vertices,
   std::vector<unsigned int>& indices)
{
#ifdef DSA
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
   glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

   glEnableVertexArrayAttrib(vao, 2);
   glVertexArrayAttribBinding(vao, 2, 0);
   glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));

   glEnableVertexArrayAttrib(vao, 3);
   glVertexArrayAttribBinding(vao, 3, 0);
   glVertexArrayAttribFormat(vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coord));

   glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
   glVertexArrayElementBuffer(vao, ebo);

#else
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coord));

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
#endif
}