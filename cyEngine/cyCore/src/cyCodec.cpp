#include "cyCodec.h"

#include "cyAudio.h"
#include "cyDefaultPrimitives.h"
#include "cyImage.h"
#include "cyMesh.h"
#include "cyModel.h"
#include "cyResource.h"
#include "cyResourceManager.h"
#include "cyShader.h"
#include "cySkeleton.h"
#include "cyTexture.h"
#include "cyVertex.h"

#include <cyJSON.h>
#include <cyStdHeaders.h>

// Sound decoding


// Shader decoding

// Model decoding

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// Image/Texture decoding
#include <FreeImage/FreeImage.h>


namespace CYLLENE_SDK {


ImageCodec::ImageCodec() : Codec(ImageCodec::staticType()) {
  for (auto extension : IMGEXT::E::_names()) {
    m_fileExtensions.push_back(Utils::toLowerCase(extension));
  }
  // Should be called once
  FreeImage_Initialise();
}

ImageCodec::~ImageCodec() {
  // Should be called when shutting down everything
  FreeImage_DeInitialise();
}

void*
ImageCodec::decode(const File& f) {
    
  Path p(f.path());

  int32 format = -1;

  for (IMGEXT::E v : IMGEXT::E::_values()) {
    String strExt = v._to_string();
    if (Utils::toLowerCase(p.extension().substr(1)) == Utils::toLowerCase(strExt)) {
        
      format = v._to_integral();
      break;
    }
  }
    
  CY_ASSERT(format != -1 && "You should first check if this codec is able to decode a format");

    
  ImageMetadata metadata;

  void* data                = FreeImage_Load(static_cast<FREE_IMAGE_FORMAT>(format), 
                                              f.path().c_str());
  metadata.m_width          = FreeImage_GetWidth(reinterpret_cast<FIBITMAP*>(data));
  metadata.m_height         = FreeImage_GetHeight(reinterpret_cast<FIBITMAP*>(data));
  metadata.m_bpp            = FreeImage_GetBPP(reinterpret_cast<FIBITMAP*>(data));
  metadata.m_format         = format;
  metadata.m_colortype      = FreeImage_GetColorType(reinterpret_cast<FIBITMAP*>(data));

  Vector<Color> colorData(metadata.m_height * metadata.m_width);

  for (uint32 y = 0; y < metadata.m_height; ++y) {
    for (uint32 x = 0; x < metadata.m_width; ++x) {
      // Get the pixel color
      RGBQUAD color;
      if (FreeImage_GetPixelColor(reinterpret_cast<FIBITMAP*>(data), x, y, &color)) {
        // Convert the pixel to Color (normalize values to [0, 1])
        float r = color.rgbRed / 255.0f;
        float g = color.rgbGreen / 255.0f;
        float b = color.rgbBlue / 255.0f;
        float a = (metadata.m_bpp == 32) ? color.rgbReserved / 255.0f : 1.0f; // Alpha if 32-bit

        colorData[y * metadata.m_width + x] = Color(r, g, b, a);
      }
      else {
        Utils::throwRuntimeError("Failed to retrieve pixel color.");
      }
    }
  }

  // Check for metadata
    
  // TODO: Extract file path starting from our project folder path so name includes this
  // SPtr<TextureResource> newResource = 
  //   ResourceManager::instance().create<TextureResource>(p.fullPath());

  /* Vector<Color> colors;
    * TextureMetaData metadata;
    * // Fill metadata
    * // Fill colors
    * newResource->setData(colors, metadata);
    * 
    * 
    */

  std::tuple<ImageMetadata, Vector<Color>>* tupleData = new std::tuple<ImageMetadata, Vector<Color>>(metadata, colorData);
  // newResource->setData(&tupleData);
  // newResource->m_metadata = metadata;
  // return REINTERPRETPOINTER(Resource, newResource);
  return reinterpret_cast<void*>(tupleData);
}
  
void
processMesh(MeshResource& m, aiMesh* node) {

  // Vertices
  for (uint32 i = 0; i < node->mNumVertices; ++i) {
    Vertex v;
    v.m_position.x = node->mVertices[i].x;
    v.m_position.y = node->mVertices[i].y;
    v.m_position.z = node->mVertices[i].z;

    // UVs
    if (node->HasTextureCoords(0)) {
      v.m_uv.x = node->mTextureCoords[0][i].x;
      v.m_uv.y = node->mTextureCoords[0][i].y;
    }
    else {
      v.m_uv.x = v.m_uv.y = 0;
    }

    // Normals
    if (node->HasNormals()) {

      v.m_normal.x = node->mNormals->x;
      v.m_normal.y = node->mNormals->y;
      v.m_normal.z = node->mNormals->z;
    }

    // Tangents and Binormals/Bitangent
    if (node->HasTangentsAndBitangents()) {
      v.m_tangent.y = node->mTangents->y;
      v.m_tangent.x = node->mTangents->x;
      v.m_tangent.z = node->mTangents->z;

      v.m_binormal.x = node->mBitangents->x;
      v.m_binormal.y = node->mBitangents->y;
      v.m_binormal.z = node->mBitangents->z;
    }

    //If it didn't have normals, but did have tangent and bi normals, we calculate normals
    if (!node->HasNormals() && node->HasTangentsAndBitangents()) {
      v.m_normal = Vector3f(v.m_tangent ^ v.m_binormal);
    }

    m.m_vertexBuffer.push_back(v);


  }
  
  // Indices
  for (uint32 f = 0; f < node->mNumFaces; ++f) {
    aiFace face = node->mFaces[f];
    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      m.m_indexBuffer.push_back(face.mIndices[j]);
    }
  }
}


void
processNode(ModelResource& m, aiNode* node, const aiScene* scene) {
  uint32 i = 0;
  for (i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    m.m_meshes.push_back(makeSharedPtr<MeshResource>());
    processMesh(*m.m_meshes.back().get(), mesh);
    m.m_hasSkeleton = false;
    if (mesh->HasBones()) { m.m_hasSkeleton = true; }
    if (scene->HasMaterials()) {

      m.m_meshes.back()->m_material = makeSharedPtr<MaterialResource>();
      aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
      
    }
  }

  for (i = 0; i < node->mNumChildren; ++i) {
    processNode(m, node->mChildren[i], scene);
  }

}

void*
ModelCodec::decode(const File& f) {

  Path p(f.path());

  // SPtr<MeshResource> newResource = 
  //   ResourceManager::instance().create<MeshResource>(p.baseName());

  // Check if we are importing a Cyllene Engine Model or a "any other format" model

  if (Utils::toLowerCase(p.extension().substr(1)) == "cym") {
    // We are using our format. we can just load the json

    // TODO: Check if the asset actually is inside a model and not any other resource
    
    JSONDocument cylleneModel;
    cylleneModel.Parse(f.readFile());
    return reinterpret_cast<void*>(&cylleneModel);
  }


  ModelResource model;
  reinterpret_cast<Resource*>(&model)->m_name = p.fullPath().c_str();
  reinterpret_cast<Resource*>(&model)->m_filePath = p;
  

  Assimp::Importer importer;

  uint32 assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality |
                       aiProcess_Triangulate |
                       aiProcess_ConvertToLeftHanded;

  auto scene = importer.ReadFile(f.path(), assimpFlags);

  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    Logger::instance().logError(Utils::format("Error loading Model at path %s", 
                                              f.path().c_str()));
    return nullptr;
  }

  // Meshes
  processNode(model, scene->mRootNode, scene);

  // Bones

  // Map<String, Bone*> sceneBones;
  // 
  // if (model.hasBones()) {
  //   uint32 index = 0;
  //   for (uint32 i = 0; i < scene->mNumMeshes; ++i) {
  //     if (scene->mMeshes[i]->HasBones()) {
  //       for (uint32 j = 0; j < scene->mMeshes[i]->mNumBones; ++j) {
  //         Bone* newBone = new Bone(*scene->mMeshes[i]->mBones[j]);
  //         newBone->m_ID = index;
  //         if (sceneBones.find(newBone->m_name) == sceneBones.end()) {
  //           sceneBones.insert(std::make_pair(newBone->m_name, newBone));
  //           ++index;
  //         }
  //       }
  //     }
  //   }
  // 
  //   for (auto bone : sceneBones) {
  //     auto node = scene->mRootNode->FindNode(bone.second->m_name.c_str());
  //     modelNodes.push_back(node);
  //   }
  //   String newExtension = filePath.substr(0, filePath.find_last_of('.'));
  //   m_skeleton = 
  //     std::static_pointer_cast<Skeleton>(ResourceManager::instance().create(newExtension,
  //                                                                           RESOURCETYPE::SKELETON));
  //   m_skeleton->init(sceneBones, modelNodes);
  // 
  // }

  // Animations


  // export to our actual file
  // TODO: Change this json to another type of writing data
  JSONDocument d;
  d.SetObject();
  JSONDocument::AllocatorType& allocator = d.GetAllocator();
  d.AddMember("type", "model", allocator);

  // meshes
  JSONValue jsonMeshes(rapidjson::kArrayType);
  for (uint32 i = 0; i < model.m_meshes.size(); ++i) {
    JSONValue jsonMesh(rapidjson::kObjectType);

    // vertices
    JSONValue jsonVertices(rapidjson::kArrayType);
    for (uint32 j = 0; j < model.m_meshes[i]->m_vertexBuffer.size(); ++j) {
      // vertex
      JSONValue jsonVertex(rapidjson::kObjectType);

      JSONValue jsonPos(rapidjson::kArrayType);
      jsonPos.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_position.x, allocator);
      jsonPos.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_position.y, allocator);
      jsonPos.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_position.z, allocator);
      jsonVertex.AddMember("position", jsonPos, allocator);
    


      JSONValue jsonUV(rapidjson::kArrayType);
      jsonUV.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_uv.x, allocator);
      jsonUV.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_uv.y, allocator);
      jsonVertex.AddMember("uv", jsonUV, allocator);


      JSONValue jsonNormal(rapidjson::kArrayType);
      jsonNormal.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_normal.x, allocator);
      jsonNormal.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_normal.y, allocator);
      jsonNormal.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_normal.z, allocator);
      jsonVertex.AddMember("normal", jsonNormal, allocator);

      JSONValue jsonTangent(rapidjson::kArrayType);
      jsonTangent.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_tangent.x, allocator);
      jsonTangent.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_tangent.y, allocator);
      jsonTangent.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_tangent.z, allocator);
      jsonVertex.AddMember("tangent", jsonTangent, allocator);

      JSONValue jsonBinormal(rapidjson::kArrayType);
      jsonBinormal.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_binormal.x, allocator);
      jsonBinormal.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_binormal.y, allocator);
      jsonBinormal.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_binormal.z, allocator);
      jsonVertex.AddMember("binormal", jsonBinormal, allocator);

      JSONValue jsonColor(rapidjson::kArrayType);
      jsonColor.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_color.r, allocator);
      jsonColor.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_color.g, allocator);
      jsonColor.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_color.b, allocator);
      jsonColor.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_color.a, allocator);
      jsonVertex.AddMember("color", jsonColor, allocator);

      JSONValue jsonBoneIDs(rapidjson::kArrayType);
      jsonBoneIDs.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneIDs[0], allocator);
      jsonBoneIDs.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneIDs[1], allocator);
      jsonBoneIDs.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneIDs[2], allocator);
      jsonBoneIDs.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneIDs[3], allocator);
      jsonVertex.AddMember("boneIDs", jsonBoneIDs, allocator);
    
      JSONValue jsonBoneWeights(rapidjson::kArrayType);
      jsonBoneWeights.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneWeights[0], allocator);
      jsonBoneWeights.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneWeights[1], allocator);
      jsonBoneWeights.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneWeights[2], allocator);
      jsonBoneWeights.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_boneWeights[3], allocator);
      jsonVertex.AddMember("boneWeights", jsonBoneWeights, allocator);

      JSONValue jsonCustomData(rapidjson::kArrayType);
      jsonCustomData.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_customData[0], allocator);
      jsonCustomData.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_customData[1], allocator);
      jsonCustomData.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_customData[2], allocator);
      jsonCustomData.PushBack(model.m_meshes[i]->m_vertexBuffer[j].m_customData[3], allocator);
      jsonVertex.AddMember("customData", jsonCustomData, allocator);

      jsonVertices.PushBack(jsonVertex, allocator);
    }
    jsonMesh.AddMember("vertices", jsonVertices, allocator);

    // indices
    JSONValue jsonIndices(rapidjson::kArrayType);
    for (uint32 j = 0; j < model.m_meshes[i]->m_indexBuffer.size(); ++j) {
      jsonIndices.PushBack(model.m_meshes[i]->m_indexBuffer[j], allocator);
    }
    jsonMesh.AddMember("indices", jsonIndices, allocator);

    // material

    // bones

    // animations
    jsonMeshes.PushBack(jsonMesh, allocator);
  }

  d.AddMember("meshes", jsonMeshes, allocator);

  // create a blob from our resources
  OfStream ofs(Utils::format("%s/%s.cym", 
                             f.parentDirectory().path().c_str(), 
                             f.fileName().c_str()).c_str());
  JSONOStream os(ofs);
  JSONWriter<JSONOStream> writer(os);
  d.Accept(writer);

  return reinterpret_cast<void*>(new String(d.stringify()));
}


  
void*
ShaderCodec::decode(const File& f) {

  // Create a copy so there is no dangling pointers
  Path p(f.path());
  String tmp = f.readFile();
    
  // This is a temporal hack which SHOULD work
  void* data = reinterpret_cast<void*>(tmp.c_str()[0]);
    
  // auto tmpPointer = new ShaderResource(pathToResource, data);
  // return SPtr<Resource>(tmpPointer);
  // 
  // delete(data);

  SPtr<ShaderResource> newResource =
    ResourceManager::instance().create<ShaderResource>(p.baseName());

  newResource->m_isBlob = p.extension().compare(".blob") == 0;
  // return REINTERPRETPOINTER(Resource, newResource);
  return nullptr;
}
  
void*
AudioCodec::decode(const File& f) {
    
  SPtr<AudioResource> newResource;
  // return REINTERPRETPOINTER(Resource, newResource);
  return nullptr;
}


}
