/*!
 * @file
 * @brief This file contains class that represents graphic card.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#pragma once

#include <student/fwd.hpp>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>

/**
 * @brief This class represent software GPU
 */
class GPU{
  public:
    GPU();
    virtual ~GPU();

    //buffer object commands
    BufferID  createBuffer           (uint64_t size);
    void      deleteBuffer           (BufferID buffer);
    void      setBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void const* data);
    void      getBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void      * data);
    bool      isBuffer               (BufferID buffer);

    //vertex array object commands (vertex puller)
    ObjectID  createVertexPuller     ();
    void      deleteVertexPuller     (VertexPullerID vao);
    void      setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer);
    void      setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer);
    void      enableVertexPullerHead (VertexPullerID vao,uint32_t head);
    void      disableVertexPullerHead(VertexPullerID vao,uint32_t head);
    void      bindVertexPuller       (VertexPullerID vao);
    void      unbindVertexPuller     ();
    bool      isVertexPuller         (VertexPullerID vao);

    //program object commands
    ProgramID createProgram          ();
    void      deleteProgram          (ProgramID prg);
    void      attachShaders          (ProgramID prg,VertexShader vs,FragmentShader fs);
    void      setVS2FSType           (ProgramID prg,uint32_t attrib,AttributeType type);
    void      useProgram             (ProgramID prg);
    bool      isProgram              (ProgramID prg);
    void      programUniform1f       (ProgramID prg,uint32_t uniformId,float     const&d);
    void      programUniform2f       (ProgramID prg,uint32_t uniformId,glm::vec2 const&d);
    void      programUniform3f       (ProgramID prg,uint32_t uniformId,glm::vec3 const&d);
    void      programUniform4f       (ProgramID prg,uint32_t uniformId,glm::vec4 const&d);
    void      programUniformMatrix4f (ProgramID prg,uint32_t uniformId,glm::mat4 const&d);

    //framebuffer functions
    void      createFramebuffer      (uint32_t width,uint32_t height);
    void      deleteFramebuffer      ();
    void      resizeFramebuffer      (uint32_t width,uint32_t height);
    uint8_t*  getFramebufferColor    ();
    float*    getFramebufferDepth    ();
    uint32_t  getFramebufferWidth    ();
    uint32_t  getFramebufferHeight   ();

    //execution commands
    void      clear                  (float r,float g,float b,float a);
    void      drawTriangles          (uint32_t  nofVertices);

    /// \addtogroup gpu_init 00. proměnné, inicializace / deinicializace grafické karty
    /// @{
    /// \todo zde si můžete vytvořit proměnné grafické karty (buffery, programy, ...)
    /// @}


    int64_t num_of_buffers;
    struct Buffer
    {
      int64_t size;
      void *data;   
    };

    struct ProgramSettings
    {
      VertexShader vshader;
      FragmentShader fshader;
      Uniforms uniforms;
      AttributeType vout_type[maxAttributes];
      bool is_created;
    };

    struct Triangle
    {
      OutVertex vertex[3];
    };
    
    struct VertexIndex
    {
      BufferID buffer_id;
      IndexType index_type;
    };
    struct VertexHead
    {
      BufferID buffer_id;
      uint64_t offset;
      uint64_t stride;
      AttributeType attribute_type;
      bool enabled;
    };

    struct VertexSettingsTable
    {
      VertexIndex indexing;
      VertexHead head_settings[maxAttributes];
      bool is_created;
    };

    struct FrameBuffer
    {;
      uint8_t *color_buffer;
      float *depth_buffer;
      int32_t width;
      int32_t height;
    };

    std::vector<Buffer> buffer_arr;
    std::vector<VertexSettingsTable> vstable_array;
    std::vector<ProgramSettings>prog_settings_array;
    uint64_t active_puller;
    uint64_t active_shader;
    FrameBuffer frame_buffer;

    float edgeFunction(Triangle triangle,int v1,int v2,float x, float y);
};


