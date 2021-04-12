/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#include <student/gpu.hpp>




/// \addtogroup gpu_init
/// @{

/**
 * @brief Constructor of GPU
 */
GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
  frame_buffer.color_buffer = NULL;
  frame_buffer.depth_buffer = NULL;
  active_puller = emptyID;
  active_shader = emptyID;
  
}

/**
 * @brief Destructor of GPU
 */
GPU::~GPU(){
  /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu
  for(int i = 0; i < buffer_arr.size(); i++)
  {
   if(isBuffer(i)) 
    deleteBuffer(i);
  }
  for(int i = 0; i < vstable_array.size(); i++)
  {
    if(isVertexPuller(i))
      deleteVertexPuller(i);
  }
  for(int i = 0; i < prog_settings_array.size();i++)
  {
    if(isProgram(i))
      deleteProgram(i);
  }
  buffer_arr.clear();
  vstable_array.clear();
  prog_settings_array.clear();
  deleteFramebuffer();


}

/// @}

/** \addtogroup buffer_tasks 01. Implementace obslužných funkcí pro buffery
 * @{
 */

/**
 * @brief This function allocates buffer on GPU.
 *
 * @param size size in bytes of new buffer on GPU.
 *
 * @return unique identificator of the buffer
 */
BufferID GPU::createBuffer(uint64_t size) { 
  /// \todo Tato funkce by měla na grafické kartě vytvořit buffer dat.<br>
  /// Velikost bufferu je v parameteru size (v bajtech).<br>
  /// Funkce by měla vrátit unikátní identifikátor identifikátor bufferu.<br>
  /// Na grafické kartě by mělo být možné alkovat libovolné množství bufferů o libovolné velikosti.<br>
  
  Buffer buffer;
  buffer.data = malloc(size);
  buffer.size = size;
  buffer_arr.push_back(buffer);
  return (buffer_arr.size()-1);
}

/**
 * @brief This function frees allocated buffer on GPU.
 *
 * @param buffer buffer identificator
 */
void GPU::deleteBuffer(BufferID buffer) {
  /// \todo Tato funkce uvolní buffer na grafické kartě.
  /// Buffer pro smazání je vybrán identifikátorem v parameteru "buffer".
  /// Po uvolnění bufferu je identifikátor volný a může být znovu použit při vytvoření nového bufferu.

  free(buffer_arr[buffer].data);
  buffer_arr[buffer].data=NULL;
}

/**
 * @brief This function uploads data to selected buffer on the GPU
 *
 * @param buffer buffer identificator
 * @param offset specifies the offset into the buffer's data
 * @param size specifies the size of buffer that will be uploaded
 * @param data specifies a pointer to new data
 */
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
  /// \todo Tato funkce nakopíruje data z cpu na "gpu".<br>
  /// Data by měla být nakopírována do bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje, kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) kam se data nakopírují.<br>
  /// Parametr data obsahuje ukazatel na data na cpu pro kopírování.<br>
  
  std::memcpy(buffer_arr[buffer].data + offset,data,size);
}

/**
 * @brief This function downloads data from GPU.
 *
 * @param buffer specfies buffer
 * @param offset specifies the offset into the buffer from which data will be returned, measured in bytes. 
 * @param size specifies data size that will be copied
 * @param data specifies a pointer to the location where buffer data is returned. 
 */
void GPU::getBufferData(BufferID buffer,
                        uint64_t offset,
                        uint64_t size,
                        void*    data)
{
  /// \todo Tato funkce vykopíruje data z "gpu" na cpu.
  /// Data by měla být vykopírována z bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) odkud se začne kopírovat.<br>
  /// Parametr data obsahuje ukazatel, kam se data nakopírují.<br>

 std::memcpy(data,buffer_arr[buffer].data + offset,size);
}

/**
 * @brief This function tests if buffer exists
 *
 * @param buffer selected buffer id
 *
 * @return true if buffer points to existing buffer on the GPU.
 */
bool GPU::isBuffer(BufferID buffer) { 
  /// \todo Tato funkce by měla vrátit true pokud buffer je identifikátor existující bufferu.<br>
  /// Tato funkce by měla vrátit false, pokud buffer není identifikátor existujícího bufferu. (nebo bufferu, který byl smazán).<br>
  /// Pro emptyId vrací false.<br>

  
  return (buffer_arr.size() > buffer && buffer_arr[buffer].data != NULL) ? true:false;
}

/// @}

/**
 * \addtogroup vertexpuller_tasks 02. Implementace obslužných funkcí pro vertex puller
 * @{
 */

/**
 * @brief This function creates new vertex puller settings on the GPU,
 *
 * @return unique vertex puller identificator
 */
ObjectID GPU::createVertexPuller     (){
  /// \todo Tato funkce vytvoří novou práznou tabulku s nastavením pro vertex puller.<br>
  /// Funkce by měla vrátit identifikátor nové tabulky.
  /// Prázdná tabulka s nastavením neobsahuje indexování a všechny čtecí hlavy jsou vypnuté.


  VertexSettingsTable vstable;
  vstable.is_created = true;
  for(int i = 0; i < maxAttributes; i++)
  {
    vstable.head_settings[i].enabled = false;
    vstable.head_settings[i].buffer_id = emptyID;
    vstable.head_settings[i].attribute_type  = AttributeType::EMPTY;
  }
  vstable.indexing.buffer_id = emptyID;

  vstable_array.push_back(vstable);

  return vstable_array.size() - 1;
}

/**
 * @brief This function deletes vertex puller settings
 *
 * @param vao vertex puller identificator
 */
void     GPU::deleteVertexPuller     (VertexPullerID vao){
  /// \todo Tato funkce by měla odstranit tabulku s nastavení pro vertex puller.<br>
  /// Parameter "vao" obsahuje identifikátor tabulky s nastavením.<br>
  /// Po uvolnění nastavení je identifiktátor volný a může být znovu použit.<br>
 if(vao < vstable_array.size())
 {
  for(int i = 0; i < maxAttributes; i++)
  {
    vstable_array[vao].head_settings[i].enabled = false;
    vstable_array[vao].head_settings[i].buffer_id = emptyID;
    vstable_array[vao].head_settings[i].attribute_type = AttributeType::EMPTY;
  }
  vstable_array[vao].indexing.buffer_id = emptyID;
  vstable_array[vao].is_created = false;
 }
}

/**
 * @brief This function sets one vertex puller reading head.
 *
 * @param vao identificator of vertex puller
 * @param head id of vertex puller head
 * @param type type of attribute
 * @param stride stride in bytes
 * @param offset offset in bytes
 * @param buffer id of buffer
 */
void     GPU::setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer){
  /// \todo Tato funkce nastaví jednu čtecí hlavu vertex pulleru.<br>
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "head" vybírá čtecí hlavu vybraného vertex pulleru.<br>
  /// Parametr "type" nastaví typ atributu, který čtecí hlava čte. Tímto se vybere kolik dat v bajtech se přečte.<br>
  /// Parametr "stride" nastaví krok čtecí hlavy.<br>
  /// Parametr "offset" nastaví počáteční pozici čtecí hlavy.<br>
  /// Parametr "buffer" vybere buffer, ze kterého bude čtecí hlava číst.<br>

  vstable_array[vao].head_settings[head].attribute_type = type;
  vstable_array[vao].head_settings[head].stride = stride;
  vstable_array[vao].head_settings[head].offset = offset;
  vstable_array[vao].head_settings[head].buffer_id = buffer;
}

/**
 * @brief This function sets vertex puller indexing.
 *
 * @param vao vertex puller id
 * @param type type of index
 * @param buffer buffer with indices
 */
void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer){
  /// \todo Tato funkce nastaví indexování vertex pulleru.
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "type" volí typ indexu, který je uložený v bufferu.<br>
  /// Parametr "buffer" volí buffer, ve kterém jsou uloženy indexy.<br>
  vstable_array[vao].indexing.index_type = type;
  vstable_array[vao].indexing.buffer_id = buffer;


}

/**
 * @brief This function enables vertex puller's head.
 *
 * @param vao vertex puller 
 * @param head head id
 */
void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce povolí čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava povolena, hodnoty z bufferu se budou kopírovat do atributu vrcholů vertex shaderu.<br>
  /// Parametr "vao" volí tabulku s nastavením vertex pulleru (vybírá vertex puller).<br>
  /// Parametr "head" volí čtecí hlavu.<br>
  
  vstable_array[vao].head_settings[head].enabled = true;
}

/**
 * @brief This function disables vertex puller's head
 *
 * @param vao vertex puller id
 * @param head head id
 */
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce zakáže čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava zakázána, hodnoty z bufferu se nebudou kopírovat do atributu vrcholu.<br>
  /// Parametry "vao" a "head" vybírají vertex puller a čtecí hlavu.<br>
  vstable_array[vao].head_settings[head].enabled = false;
}

/**
 * @brief This function selects active vertex puller.
 *
 * @param vao id of vertex puller
 */
void     GPU::bindVertexPuller       (VertexPullerID vao){
  /// \todo Tato funkce aktivuje nastavení vertex pulleru.<br>
  /// Pokud je daný vertex puller aktivován, atributy z bufferů jsou vybírány na základě jeho nastavení.<br>
  active_puller = vao;
}

/**
 * @brief This function deactivates vertex puller.
 */
void     GPU::unbindVertexPuller     (){
  /// \todo Tato funkce deaktivuje vertex puller.
  /// To většinou znamená, že se vybere neexistující "emptyID" vertex puller.
  active_puller = emptyID;
}

/**
 * @brief This function tests if vertex puller exists.
 *
 * @param vao vertex puller
 *
 * @return true, if vertex puller "vao" exists
 */
bool     GPU::isVertexPuller         (VertexPullerID vao){
  /// \todo Tato funkce otestuje, zda daný vertex puller existuje.
  /// Pokud ano, funkce vrací true.

  return (vstable_array.size() > vao && vstable_array[vao].is_created)?true:false;
}

/// @}

/** \addtogroup program_tasks 03. Implementace obslužných funkcí pro shader programy
 * @{
 */

/**
 * @brief This function creates new shader program.
 *
 * @return shader program id
 */
ProgramID        GPU::createProgram         (){
  /// \todo Tato funkce by měla vytvořit nový shader program.<br>
  /// Funkce vrací unikátní identifikátor nového proramu.<br>
  /// Program je seznam nastavení, které obsahuje: ukazatel na vertex a fragment shader.<br>
  /// Dále obsahuje uniformní proměnné a typ výstupních vertex attributů z vertex shaderu, které jsou použity pro interpolaci do fragment atributů.<br>
  ProgramSettings prog_settings;
  prog_settings.vshader = NULL;
  prog_settings.fshader = NULL;
  prog_settings.is_created = 1;

  for(int i =0; i < maxAttributes; i++)
  {
    prog_settings.vout_type[i] = AttributeType::EMPTY;
  }

  prog_settings_array.push_back(prog_settings);
  return prog_settings_array.size() - 1;
}

/**
 * @brief This function deletes shader program
 *
 * @param prg shader program id
 */
void             GPU::deleteProgram         (ProgramID prg){
  /// \todo Tato funkce by měla smazat vybraný shader program.<br>
  /// Funkce smaže nastavení shader programu.<br>
  /// Identifikátor programu se stane volným a může být znovu využit.<br>
  prog_settings_array[prg].vshader = NULL;
  prog_settings_array[prg].fshader = NULL;
  
  
  for(int i = 0; i < maxAttributes; i++)
  {
    prog_settings_array[prg].vout_type[i] = AttributeType::EMPTY;
  }
  prog_settings_array[prg].vout_type;
  prog_settings_array[prg].is_created = 0;
}

/**
 * @brief This function attaches vertex and frament shader to shader program.
 *
 * @param prg shader program
 * @param vs vertex shader 
 * @param fs fragment shader
 */
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
  /// \todo Tato funkce by měla připojít k vybranému shader programu vertex a fragment shader.
  prog_settings_array[prg].vshader = vs;
  prog_settings_array[prg].fshader = fs;
}

/**
 * @brief This function selects which vertex attributes should be interpolated during rasterization into fragment attributes.
 *
 * @param prg shader program
 * @param attrib id of attribute
 * @param type type of attribute
 */
void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
  /// \todo tato funkce by měla zvolit typ vertex atributu, který je posílán z vertex shaderu do fragment shaderu.<br>
  /// V průběhu rasterizace vznikají fragment.<br>
  /// Fragment obsahují fragment atributy.<br>
  /// Tyto atributy obsahují interpolované hodnoty vertex atributů.<br>
  /// Tato funkce vybere jakého typu jsou tyto interpolované atributy.<br>
  /// Bez jakéhokoliv nastavení jsou atributy prázdne AttributeType::EMPTY<br>
  prog_settings_array[prg].vout_type[attrib] = type;  
}

/**
 * @brief This function actives selected shader program
 *
 * @param prg shader program id
 */
void             GPU::useProgram            (ProgramID prg){
  /// \todo tato funkce by měla vybrat aktivní shader program.
  active_shader = prg;
}

/**
 * @brief This function tests if selected shader program exists.
 *
 * @param prg shader program
 *
 * @return true, if shader program "prg" exists.
 */
bool             GPU::isProgram             (ProgramID prg){
  /// \todo tato funkce by měla zjistit, zda daný program existuje.<br>
  /// Funkce vráti true, pokud program existuje.<br>
  return (prog_settings_array.size() > prg && prog_settings_array[prg].is_created)? true:false;
}

/**
 * @brief This function sets uniform value (1 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform1f      (ProgramID prg,uint32_t uniformId,float     const&d){
  /// \todo tato funkce by měla nastavit uniformní proměnnou shader programu.<br>
  /// Parametr "prg" vybírá shader program.<br>
  /// Parametr "uniformId" vybírá uniformní proměnnou. Maximální počet uniformních proměnných je uložen v programné \link maxUniforms \endlink.<br>
  /// Parametr "d" obsahuje data (1 float).<br>
  prog_settings_array[prg].uniforms.uniform[uniformId].v1 = d;

}

/**
 * @brief This function sets uniform value (2 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 2 floaty.
  prog_settings_array[prg].uniforms.uniform[uniformId].v2 = d;
}

/**
 * @brief This function sets uniform value (3 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 3 floaty.
  prog_settings_array[prg].uniforms.uniform[uniformId].v3 = d;
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 4 floaty.
prog_settings_array[prg].uniforms.uniform[uniformId].v4 = d;
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává matici 4x4 (16 floatů).
  prog_settings_array[prg].uniforms.uniform[uniformId].m4 = d;
}

/// @}





/** \addtogroup framebuffer_tasks 04. Implementace obslužných funkcí pro framebuffer
 * @{
 */

/**
 * @brief This function creates framebuffer on GPU.
 *
 * @param width width of framebuffer
 * @param height height of framebuffer
 */
void GPU::createFramebuffer      (uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla alokovat framebuffer od daném rozlišení.<br>
  /// Framebuffer se skládá z barevného a hloukového bufferu.<br>
  /// Buffery obsahují width x height pixelů.<br>
  /// Barevný pixel je složen z 4 x uint8_t hodnot - to reprezentuje RGBA barvu.<br>
  /// Hloubkový pixel obsahuje 1 x float - to reprezentuje hloubku.<br>
  /// Nultý pixel framebufferu je vlevo dole.<br>
  frame_buffer.color_buffer = new uint8_t[width * height * 4];
  frame_buffer.depth_buffer = new float[width * height];
  frame_buffer.width = width;
  frame_buffer.height = height;
  GPU::clear(0,0,0,0);


}

/**
 * @brief This function deletes framebuffer.
 */
void GPU::deleteFramebuffer      (){
  /// \todo tato funkce by měla dealokovat framebuffer.
  if(frame_buffer.color_buffer != NULL)
   delete[] frame_buffer.color_buffer;
  if(frame_buffer.depth_buffer != NULL)
   delete[] frame_buffer.depth_buffer;
  frame_buffer.depth_buffer = NULL;
  frame_buffer.color_buffer = NULL;
}

/**
 * @brief This function resizes framebuffer.
 *
 * @param width new width of framebuffer
 * @param height new heght of framebuffer
 */
void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
  GPU::deleteFramebuffer();
  /// \todo Tato funkce by měla změnit velikost framebuffer.
 
  frame_buffer.color_buffer = new uint8_t[width * height * 4];
  frame_buffer.depth_buffer = new float[width*height];

  frame_buffer.width = width;
  frame_buffer.height = height;
}

/**
 * @brief This function returns pointer to color buffer.
 *
 * @return pointer to color buffer
 */
uint8_t* GPU::getFramebufferColor  (){
  /// \todo Tato funkce by měla vrátit ukazatel na začátek barevného bufferu.<br>
  return frame_buffer.color_buffer;
}

/**
 * @brief This function returns pointer to depth buffer.
 *
 * @return pointer to dept buffer.
 */
float* GPU::getFramebufferDepth    (){
  /// \todo tato funkce by mla vrátit ukazatel na začátek hloubkového bufferu.<br>
  return frame_buffer.depth_buffer;
}

/**
 * @brief This function returns width of framebuffer
 *
 * @return width of framebuffer
 */
uint32_t GPU::getFramebufferWidth (){
  /// \todo Tato funkce by měla vrátit šířku framebufferu.
  return frame_buffer.width;
}

/**
 * @brief This function returns height of framebuffer.
 *
 * @return height of framebuffer
 */
uint32_t GPU::getFramebufferHeight(){
  /// \todo Tato funkce by měla vrátit výšku framebufferu.
  return frame_buffer.height;
}

/// @}

/** \addtogroup draw_tasks 05. Implementace vykreslovacích funkcí
 * Bližší informace jsou uvedeny na hlavní stránce dokumentace.
 * @{
 */

/**
 * @brief This functino clears framebuffer.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void            GPU::clear                 (float r,float g,float b,float a){
  /// \todo Tato funkce by měla vyčistit framebuffer.<br>
  /// Barevný buffer vyčistí na barvu podle parametrů r g b a (0 - nulová intenzita, 1 a větší - maximální intenzita).<br>
  /// (0,0,0) - černá barva, (1,1,1) - bílá barva.<br>
  /// Hloubkový buffer nastaví na takovou hodnotu, která umožní rasterizaci trojúhelníka, který leží v rámci pohledového tělesa.<br>
  /// Hloubka by měla být tedy větší než maximální hloubka v NDC (normalized device coordinates).<br
  float rgba[] = {r,g,b,a};
  for(int i = 0; i < (frame_buffer.width * frame_buffer.height * 4); i++)
  {
    frame_buffer.color_buffer[i] =  (rgba[i%4] > 255.0) ? (uint8_t)255: (uint8_t)(rgba[i%4] * 255);
  }
  for(int i = 0; i < (frame_buffer.width * frame_buffer.height); i++)
  {
    frame_buffer.depth_buffer[i] = 1.33;
  }
}



void  GPU::drawTriangles  (uint32_t  nofVertices){
  /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
  /// Vrcholy se budou vybírat podle nastavení z aktivního vertex pulleru (pomocí bindVertexPuller).<br>
  /// Vertex shader a fragment shader se zvolí podle aktivního shader programu (pomocí useProgram).<br>
  /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
  InVertex * inVertexArr = new InVertex[nofVertices];
  OutVertex * outVertexArr = new OutVertex[nofVertices];
  Triangle * triangle = (GPU::Triangle*)malloc(sizeof(GPU::Triangle) * (nofVertices/3));

  VertexSettingsTable VertexPuller = vstable_array[active_puller];
  ProgramSettings Shaders = prog_settings_array[active_shader];

/* Vertex puller */
  for(int i = 0; i < nofVertices; i++)
  {
    if(VertexPuller.indexing.buffer_id == emptyID)
      inVertexArr[i].gl_VertexID = i;
    else
    {
      switch(VertexPuller.indexing.index_type)
      {
        case IndexType::UINT8:
        std::memcpy(&inVertexArr[i].gl_VertexID,buffer_arr[VertexPuller.indexing.buffer_id].data + i*sizeof(uint8_t),sizeof(uint8_t));
        break;
        case IndexType::UINT16:
        std::memcpy(&inVertexArr[i].gl_VertexID,buffer_arr[VertexPuller.indexing.buffer_id].data + i*sizeof(uint16_t),sizeof(uint16_t));
        break;
        case IndexType::UINT32:
        std::memcpy(&inVertexArr[i].gl_VertexID,buffer_arr[VertexPuller.indexing.buffer_id].data + i*sizeof(uint32_t),sizeof(uint32_t));
        break;
        default:
        break;
      }
    }
    
  
    for(int j = 0; j < maxAttributes; j++)
    {
      if(VertexPuller.head_settings[j].enabled)
      {
        switch (VertexPuller.head_settings[j].attribute_type)
        {
          case AttributeType::FLOAT:
          std::memcpy((void*)&inVertexArr[i].attributes[j].v1,buffer_arr[VertexPuller.head_settings[j].buffer_id].data 
                                                 + VertexPuller.head_settings[j].offset +
                                                + (VertexPuller.head_settings[j].stride * inVertexArr[i].gl_VertexID),
                                                  sizeof(float)  );
            break;
          case AttributeType::VEC2:
          std::memcpy((void*)&inVertexArr[i].attributes[j].v2,buffer_arr[VertexPuller.head_settings[j].buffer_id].data 
                                                 + VertexPuller.head_settings[j].offset +
                                                + (VertexPuller.head_settings[j].stride * inVertexArr[i].gl_VertexID),
                                                  sizeof(float)  * 2 );
        
            break;
          case AttributeType::VEC3:
            std::memcpy((void*)&inVertexArr[i].attributes[j].v3,buffer_arr[VertexPuller.head_settings[j].buffer_id].data 
                                                 + VertexPuller.head_settings[j].offset +
                                                + (VertexPuller.head_settings[j].stride * inVertexArr[i].gl_VertexID),
                                                  sizeof(float) * 3);
            break;
          case AttributeType::VEC4:
          std::memcpy((void*)&inVertexArr[i].attributes[j].v4,buffer_arr[VertexPuller.head_settings[j].buffer_id].data 
                                                 + VertexPuller.head_settings[j].offset +
                                                + (VertexPuller.head_settings[j].stride * inVertexArr[i].gl_VertexID),
                                                  sizeof(float) * 4 );
            
            break;
          default:
            break;
        }
      }
   }
  


    /* Vertex shader*/
    Shaders.vshader(outVertexArr[i],inVertexArr[i],Shaders.uniforms);
   
    /* Primitive assembly */
    if((i+1) % 3 == 0 && i != 0)
    {
      int index = ((i+1) / 3)-1;

      triangle[index].vertex[0] = outVertexArr[i];
      triangle[index].vertex[1]= outVertexArr[i - 1];
      triangle[index].vertex[2] = outVertexArr[i - 2];
    }
  }


 
  for(int i = 0; i < (nofVertices/3);i++)
  {
    // Perspective division 
    for(int j = 0; j < 3; j++)
    {
      if(triangle[i].vertex[j].gl_Position.w >= 0.000001 || triangle[i].vertex[j].gl_Position.w < 0)
      {
        triangle[i].vertex[j].gl_Position.x /= triangle[i].vertex[j].gl_Position[3];
        triangle[i].vertex[j].gl_Position.y /= triangle[i].vertex[j].gl_Position[3];
        triangle[i].vertex[j].gl_Position.z /= triangle[i].vertex[j].gl_Position[3];
      }
    }


  
   //Viewport matrice 
    for(int j = 0; j < 3; j++)
    {
      triangle[i].vertex[j].gl_Position.x = ((triangle[i].vertex[j].gl_Position.x + 1)/2)*frame_buffer.width;
      triangle[i].vertex[j].gl_Position.y = ((triangle[i].vertex[j].gl_Position.y + 1)/2)*frame_buffer.height;
    }
  

    float max_x = triangle[i].vertex[0].gl_Position.x;
    float max_y = triangle[i].vertex[0].gl_Position.y;
    float min_x = triangle[i].vertex[0].gl_Position.x;
    float min_y = triangle[i].vertex[0].gl_Position.y;

    
    for(int vert_i = 0; vert_i < 3; vert_i++)
    {
      if(triangle[i].vertex[vert_i].gl_Position.x > max_x)
        max_x = triangle[i].vertex[vert_i].gl_Position.x;
      if(triangle[i].vertex[vert_i].gl_Position.y > max_y)
        max_y = triangle[i].vertex[vert_i].gl_Position.y;
      if(triangle[i].vertex[vert_i].gl_Position.x < min_x)
        min_x = triangle[i].vertex[vert_i].gl_Position.x;
      if(triangle[i].vertex[vert_i].gl_Position.y < min_y)
        min_y = triangle[i].vertex[vert_i].gl_Position.y;
    }

   
    for(int y = min_y; y < max_y; y++)
    {
      for(int x = min_x ; x < max_x; x++)
      {
        InFragment infragment;
        
        /* Zdroj vypoctu hranovych funkci a barycentrickych koordinat https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation*/
        float area = GPU::edgeFunction(triangle[i],0,1,triangle[i].vertex[2].gl_Position.x,triangle[i].vertex[2].gl_Position.y);
        float a = GPU::edgeFunction(triangle[i],0,1,(float)x + 0.5,(float)y + 0.5);
        float b = GPU::edgeFunction(triangle[i],1,2,(float)x + 0.5,(float)y  +0.5);
        float c = GPU::edgeFunction(triangle[i],2,0,(float)x + 0.5,(float)y  +0.5);

      
        
        if(a >= 0 && b >= 0 && c>= 0)
        {
          float l0 = b/area;
          float l1 = c/area;
          float l2 = a/area;


          float h0 = float(triangle[i].vertex[0].gl_Position.w);
          float h1 = float(triangle[i].vertex[1].gl_Position.w);
          float h2 = float(triangle[i].vertex[2].gl_Position.w);

          /* Konec zdroje */
          float A0,A1,A2;

          
          for(int k = 0; k < maxAttributes; k++)
          {

            switch(Shaders.vout_type[k])
            {
              case AttributeType::FLOAT:
                A0 = triangle[i].vertex[0].attributes[k].v1;
                A1 = triangle[i].vertex[1].attributes[k].v1;
                A2 = triangle[i].vertex[2].attributes[k].v1;
               infragment.attributes[k].v1 = ((A0*l0)/h0 + (A1*l1)/h1 + (A2*l2)/h2) / (l0/h0 + l1/h1 + l2/h2);
               break;
              case AttributeType::VEC2:
               for(int j = 0; j < 2; j++) 
               {
                 A0 = triangle[i].vertex[0].attributes[k].v2[j];
                 A1 = triangle[i].vertex[1].attributes[k].v2[j];
                 A2 = triangle[i].vertex[2].attributes[k].v2[j];
                 infragment.attributes[k].v2[j] = ((A0*l0)/h0 + (A1*l1)/h1 + (A2*l2)/h2) / (l0/h0 + l1/h1 + l2/h2);
               }
               break;

              case AttributeType::VEC3:
               for(int j = 0; j < 3; j++) 
               {
                 A0 = triangle[i].vertex[0].attributes[k].v3[j];
                 A1 = triangle[i].vertex[1].attributes[k].v3[j];
                 A2 = triangle[i].vertex[2].attributes[k].v3[j];
                 infragment.attributes[k].v3[j] = (((A0*l0)/h0) + ((A1*l1)/h1) + ((A2*l2)/h2)) / ((l0/h0) + (l1/h1) + (l2/h2));
               }
               break;
              case AttributeType::VEC4:
               for(int j = 0; j < 4; j++) 
               {
                 A0 = triangle[i].vertex[0].attributes[k].v4[j];
                 A1 = triangle[i].vertex[1].attributes[k].v4[j];
                 A2 = triangle[i].vertex[2].attributes[k].v4[j];
                 infragment.attributes[k].v4[j] = ((A0*l0)/h0 + (A1*l1)/h1 + (A2*l2)/h2) / (l0/h0 + l1/h1 + l2/h2);
               }
               break;
              default:
               break;

            }
          }

          A0 = triangle[i].vertex[0].gl_Position.z;
          A1 = triangle[i].vertex[1].gl_Position.z;
          A2 = triangle[i].vertex[2].gl_Position.z;
          infragment.gl_FragCoord.z = ((A0*l0)/h0 + (A1*l1)/h1 + (A2*l2)/h2) / (l0/h0 + l1/h1 + l2/h2);
          

          OutFragment outfragment;
          infragment.gl_FragCoord.x = x+0.5;
          infragment.gl_FragCoord.y = y+0.5;
          Shaders.fshader(outfragment,infragment,Shaders.uniforms);

          if(infragment.gl_FragCoord.z < frame_buffer.depth_buffer[y * frame_buffer.width +x])
          {
            frame_buffer.color_buffer[4*(y * frame_buffer.width  + x)] = (int(255*outfragment.gl_FragColor.r) > 255)? 254:uint8_t((255*outfragment.gl_FragColor.r));
            frame_buffer.color_buffer[4*(y * frame_buffer.width  + x) + 1] = (int(255*outfragment.gl_FragColor.g) > 255)? 254:uint8_t((255*outfragment.gl_FragColor.g));
            frame_buffer.color_buffer[4*(y * frame_buffer.width  + x) + 2] = (int(255*outfragment.gl_FragColor.b) > 255)? 254:uint8_t((255*outfragment.gl_FragColor.b));
            frame_buffer.color_buffer[4*(y * frame_buffer.width  + x) + 3] = (int(255*outfragment.gl_FragColor.a) > 255)? 254:uint8_t((255*outfragment.gl_FragColor.a));
            frame_buffer.depth_buffer[y * frame_buffer.width + x] = float(infragment.gl_FragCoord.z);
          }
        }
      }
    }
  }
}


/* Zdroj vzorce: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation*/
float GPU::edgeFunction(GPU::Triangle triangle,int v1,int v2,float x, float y)
{
  return  (x - triangle.vertex[v1].gl_Position.x)
         *(triangle.vertex[v2].gl_Position.y - triangle.vertex[v1].gl_Position.y)
         -(y - triangle.vertex[v1].gl_Position.y)
         *(triangle.vertex[v2].gl_Position.x - triangle.vertex[v1].gl_Position.x);
}
    /* Konec zdroje */



/// @}
