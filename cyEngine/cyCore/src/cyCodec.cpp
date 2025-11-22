#include "cyCodec.h"

#include "cyRAudio.h"
#include "cyDefaultPrimitives.h"
#include "cyRImage.h"
#include "cyRMesh.h"
#include "cyEngineAssets.h"
#include "cyRModel.h"
#include "cyRResource.h"
#include "cyResourceManager.h"
#include "cyRShader.h"
#include "cyRSkeleton.h"
#include "cyRTexture.h"
#include "cyVertex.h"

#include <cyJSON.h>
#include <cyStdHeaders.h>

// Audio/Sound decoding
// #include <ffmpeg/libavformat/avformat.h>
// #include <ffmpeg/libavcodec/avcodec.h>
// #include <ffmpeg/libswresample/swresample.h>
// #include <ffmpeg/libavutil/opt.h>
// #include <ffmpeg/libavutil/channel_layout.h>

// Shader decoding

// Model decoding
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// Image/Texture decoding
#include <FreeImage/FreeImage.h>


namespace CYLLENE_SDK {


  ImageCodec::ImageCodec() : CDCodec(ImageCodec::staticType()) {
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

    void* data = FreeImage_Load(static_cast<FREE_IMAGE_FORMAT>(format),
      f.path().c_str());
    metadata.m_width = FreeImage_GetWidth(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_height = FreeImage_GetHeight(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_bpp = FreeImage_GetBPP(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_format = format;
    metadata.m_colortype = FreeImage_GetColorType(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_pitch = FreeImage_GetPitch(reinterpret_cast<FIBITMAP*>(data));

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
    processMesh(SPtr<RMesh>& m, aiMesh* node) {

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

      m->m_vertexBuffer.push_back(v);


    }

    // Indices
    for (uint32 f = 0; f < node->mNumFaces; ++f) {
      aiFace face = node->mFaces[f];
      for (uint32 j = 0; j < face.mNumIndices; ++j) {
        m->m_indexBuffer.push_back(face.mIndices[j]);
      }
    }
  }

  String
    getBasenameFromPath(const String& inPath) {
    Path p(inPath);
    return p.baseName();
  }

  Path
    backRecursivelyFindTexturePath(const Path& basePath, const String& baseName, int counter) {



    String folder = FileSystem::open(basePath.path()).isDirectory() ? basePath.path() : basePath.directoryPath();

    Path testPath = Path(folder + baseName);
    if (FileSystem::exists(testPath.fullPath())) {
      return testPath;
    }
    else {

      // Check for other folders first
      File dir = FileSystem::open(folder);
      Vector<Path> subFolders;
      if (dir.isDirectory()) {
        for (FileIterator it = dir.begin(); it != dir.end(); ++it) {
          std::string path = *it;
          File subdir = FileSystem::open(path);
          if (subdir.isDirectory()) {
            subFolders.push_back(Path(path));
          }
        }
      }

      // Search in subfolders
      for (const Path& subFolder : subFolders) {
        File subdir = FileSystem::open(folder);
        if (subdir.isDirectory()) {
          Path possiblePath = Path(subFolder.fullPath() + "/" + baseName);
          if (FileSystem::exists(possiblePath.fullPath())) {
            return possiblePath;
          }
        }
      }

      if (counter < 5) { // Limit recursion depth
        Path parentFolder = folder + "../";
        if (parentFolder.fullPath() != parentFolder.driveLetter()) {
          return backRecursivelyFindTexturePath(parentFolder, baseName, counter + 1);
        }
      }
    }
    // error Not found
    return Path("");
  }

  String
    getAssimpPath(aiMaterial* mat, aiTextureType type) {
    aiString texPath;
    if (mat->GetTextureCount(type) > 0 && mat->GetTexture(type, 0, &texPath) == AI_SUCCESS) {
      Path p(texPath.C_Str());
      return p.fileName();
    }
    return String("");
  }

  String
    getTextureBaseName(aiMaterial* mat, aiTextureType type) {
    aiString texPath;
    if (mat->GetTextureCount(type) > 0 && mat->GetTexture(type, 0, &texPath) == AI_SUCCESS) {
      return getBasenameFromPath(String(texPath.C_Str()));
    }
    return String("");
  }

  String
    createMaterialFromAssimp(aiMaterial* aiMat,
      const String& defaultName,
      const Path& p) {

    String matName = defaultName;
    // Try to get material name from assimp
    aiString aiName;
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_NAME, aiName)) {
      if (aiName.length > 0) matName = String(aiName.C_Str());
    }
    // Build values map using common PBR channels available from Assimp
    UnorderedMap<String, String> values; // key -> texture resource name
    UnorderedMap<String, String> valueImages; // key -> texture resource name
    // Albedo / Diffuse
    String albedo = getTextureBaseName(aiMat, aiTextureType_DIFFUSE);
    String albedoImage = getAssimpPath(aiMat, aiTextureType_DIFFUSE);
    if (!albedo.empty()) {
      values["albedo"] = albedo;
      valueImages["albedoImage"] = albedoImage; // default white
    }
    // Normal
    String normal = getTextureBaseName(aiMat, aiTextureType_NORMALS);
    String normalImage = getAssimpPath(aiMat, aiTextureType_NORMALS);
    if (normal.empty()) normal = getTextureBaseName(aiMat, aiTextureType_HEIGHT); // fallback
    if (normalImage.empty()) normalImage = getAssimpPath(aiMat, aiTextureType_HEIGHT); // fallback
    if (!normal.empty()) {
      values["normal"] = normal;
      valueImages["normalImage"] = normalImage; // default
    }
    // Metallic (use SPECULAR as a generic fallback)
    String metallic = getTextureBaseName(aiMat, aiTextureType_SPECULAR);
    String metallicImage = getAssimpPath(aiMat, aiTextureType_SPECULAR);
    if (!metallic.empty()) {
      values["metallic"] = metallic;
      valueImages["metallicImage"] = metallicImage; // default black
    }
    // Roughness (use SHININESS as a generic fallback)
    String roughness = getTextureBaseName(aiMat, aiTextureType_SHININESS);
    String roughnessImage = getAssimpPath(aiMat, aiTextureType_SHININESS);
    if (!roughness.empty()) {
      values["roughness"] = roughness;
      valueImages["roughnessImage"] = roughnessImage; // default white
    }
    // Emissive
    String emissive = getTextureBaseName(aiMat, aiTextureType_EMISSIVE);
    String emissiveImage = getAssimpPath(aiMat, aiTextureType_EMISSIVE);
    if (!emissive.empty()) {
      values["emissive"] = emissive;
      valueImages["emissiveImage"] = emissiveImage; // default black
    }
    // Occlusion (use AMBIENT as a generic fallback)
    String occlusion = getTextureBaseName(aiMat, aiTextureType_AMBIENT);
    String occlusionImage = getAssimpPath(aiMat, aiTextureType_AMBIENT);
    if (!occlusion.empty()) {
      values["occlusion"] = occlusion;
      valueImages["occlusionImage"] = occlusionImage; // default white
    }
    // Opacity
    String opacity = getTextureBaseName(aiMat, aiTextureType_OPACITY);
    String opacityImage = getAssimpPath(aiMat, aiTextureType_OPACITY);
    if (!opacity.empty()) {
      values["opacity"] = opacity;
      valueImages["opacityImage"] = opacityImage; // default white
    }
    // Register texture resources so material JSON can resolve names when encoding
    for (const auto& kv : values) {
      if (!kv.second.empty()) {
        String imgString = kv.first + "Image";
        String imgPath = valueImages[imgString];
        // imgPath = imgPath.fileName();
        // String p =  imgPath.fullPath();
        String baseName = kv.second;
        String parentPath = p.directoryPath();
        Path possiblePath = backRecursivelyFindTexturePath(parentPath, imgPath, 0);
        if (possiblePath.fullPath().empty() || !FileSystem::exists(possiblePath.fullPath())) {
          Logger::instance().logWarning(Utils::format("Could not find texture image for material %s, texture %s",
            matName.c_str(),
            imgPath.c_str()));
          continue;
        }
        WPtr<RImage> img = ResourceManager::instance().loadFromPath<RImage>(possiblePath.fullPath());
        WPtr<RTexture> txt = ResourceManager::instance().create<RTexture>(kv.second);
        if (img.lock()) {
          txt.lock()->setImage(img.lock());
        }
      }
    }
    // Create and populate material via JSON so it fills default values
    JSONDocument d; d.SetObject();
    auto& allocator = d.GetAllocator();
    d.AddMember("type", "material", allocator);

    // Use default material's base shader if available
    String baseShaderName = "";
    {
      JSONDocument defMat;
      defMat.Parse(DEFAULTMATERIALS::defaultMaterial);
      if (!defMat.HasParseError() && defMat.HasMember("baseShader") && defMat["baseShader"].IsString()) {
        baseShaderName = defMat["baseShader"].GetString();
      }
    }
    if (baseShaderName.empty()) {
      baseShaderName = "DefaultShader"; // fallback
    }
    {
      JSONValue bs(baseShaderName.c_str(), allocator);
      d.AddMember("baseShader", bs, allocator);
    }
    JSONValue jvals; jvals.SetObject();
    for (const auto& kv : values) {
      JSONValue key(kv.first.c_str(), allocator);
      JSONValue val(kv.second.c_str(), allocator);
      jvals.AddMember(key, val, allocator);
    }
    d.AddMember("values", jvals, allocator);
    String json = d.stringify();
    SPtr<RMaterial> mat = ResourceManager::instance().create<RMaterial>(matName);
    void* jsonPtr = const_cast<void*>(reinterpret_cast<const void*>(&json));
    mat->setData(jsonPtr);
    Path newMatPath = CY_PROJECT_DIR.path() + Utils::format("/%s.mat", matName.c_str());
    String pathStr = newMatPath.directoryPath();
    if (!FileSystem::exists(pathStr)) {
      FileSystem::createFolder(newMatPath.directoryPath());
    }
    // Persist the material by encoding the actual RMaterial (not the JSON string)
    ResourceManager::instance().getCodec<CDCodec>(RESOURCE_TYPE::E::eMATERIAL)->encode(newMatPath, mat.get());
    return matName;
  }


  void
    processNode(SPtr<RModel>& m, aiNode* node, const aiScene* scene, const Path& p) {
    uint32 i = 0;
    for (i = 0; i < node->mNumMeshes; ++i) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

      m->m_meshes.push_back(makeSharedPtr<RMesh>());
      processMesh(m->m_meshes.back(), mesh);
      m->m_hasSkeleton = false;
      if (mesh->HasBones()) { m->m_hasSkeleton = true; }
      if (scene->HasMaterials()) {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
        String fallbackName = Utils::format("%s_mat%u", node->mName.C_Str(), i);
        String newName = createMaterialFromAssimp(aiMat, fallbackName, p);
        m->m_meshes.back()->m_materialName = newName;
      }
    }

    for (i = 0; i < node->mNumChildren; ++i) {
      processNode(m, node->mChildren[i], scene, p);
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


    SPtr<RModel> model = makeSharedPtr<RModel>(); // ResourceManager::instance().get<RModel>(p.baseName());
    // reinterpret_cast<RResource*>(&model)->m_name = p.fullPath().c_str();
    // reinterpret_cast<RResource*>(&model)->m_filePath = p;


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
    processNode(model, scene->mRootNode, scene, p);

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
    for (uint32 i = 0; i < model->m_meshes.size(); ++i) {
      JSONValue jsonMesh(rapidjson::kObjectType);

      // vertices
      JSONValue jsonVertices(rapidjson::kArrayType);
      for (uint32 j = 0; j < model->m_meshes[i]->m_vertexBuffer.size(); ++j) {
        // vertex
        JSONValue jsonVertex(rapidjson::kObjectType);

        JSONValue jsonPos(rapidjson::kArrayType);
        jsonPos.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_position.x, allocator);
        jsonPos.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_position.y, allocator);
        jsonPos.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_position.z, allocator);
        jsonVertex.AddMember("position", jsonPos, allocator);



        JSONValue jsonUV(rapidjson::kArrayType);
        jsonUV.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_uv.x, allocator);
        jsonUV.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_uv.y, allocator);
        jsonVertex.AddMember("uv", jsonUV, allocator);


        JSONValue jsonNormal(rapidjson::kArrayType);
        jsonNormal.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_normal.x, allocator);
        jsonNormal.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_normal.y, allocator);
        jsonNormal.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_normal.z, allocator);
        jsonVertex.AddMember("normal", jsonNormal, allocator);

        JSONValue jsonTangent(rapidjson::kArrayType);
        jsonTangent.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_tangent.x, allocator);
        jsonTangent.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_tangent.y, allocator);
        jsonTangent.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_tangent.z, allocator);
        jsonVertex.AddMember("tangent", jsonTangent, allocator);

        JSONValue jsonBinormal(rapidjson::kArrayType);
        jsonBinormal.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_binormal.x, allocator);
        jsonBinormal.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_binormal.y, allocator);
        jsonBinormal.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_binormal.z, allocator);
        jsonVertex.AddMember("binormal", jsonBinormal, allocator);

        JSONValue jsonColor(rapidjson::kArrayType);
        jsonColor.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_color.r, allocator);
        jsonColor.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_color.g, allocator);
        jsonColor.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_color.b, allocator);
        jsonColor.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_color.a, allocator);
        jsonVertex.AddMember("color", jsonColor, allocator);

        JSONValue jsonBoneIDs(rapidjson::kArrayType);
        jsonBoneIDs.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneIDs[0], allocator);
        jsonBoneIDs.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneIDs[1], allocator);
        jsonBoneIDs.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneIDs[2], allocator);
        jsonBoneIDs.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneIDs[3], allocator);
        jsonVertex.AddMember("boneIDs", jsonBoneIDs, allocator);

        JSONValue jsonBoneWeights(rapidjson::kArrayType);
        jsonBoneWeights.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneWeights[0], allocator);
        jsonBoneWeights.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneWeights[1], allocator);
        jsonBoneWeights.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneWeights[2], allocator);
        jsonBoneWeights.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_boneWeights[3], allocator);
        jsonVertex.AddMember("boneWeights", jsonBoneWeights, allocator);

        JSONValue jsonCustomData(rapidjson::kArrayType);
        jsonCustomData.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_customData[0], allocator);
        jsonCustomData.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_customData[1], allocator);
        jsonCustomData.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_customData[2], allocator);
        jsonCustomData.PushBack(model->m_meshes[i]->m_vertexBuffer[j].m_customData[3], allocator);
        jsonVertex.AddMember("customData", jsonCustomData, allocator);

        jsonVertices.PushBack(jsonVertex, allocator);
      }
      jsonMesh.AddMember("vertices", jsonVertices, allocator);

      // indices
      JSONValue jsonIndices(rapidjson::kArrayType);
      for (uint32 j = 0; j < model->m_meshes[i]->m_indexBuffer.size(); ++j) {
        jsonIndices.PushBack(model->m_meshes[i]->m_indexBuffer[j], allocator);
      }
      jsonMesh.AddMember("indices", jsonIndices, allocator);

      // material
      {
        String matName = model->m_meshes[i]->m_materialName;
        JSONValue matVal;
        if (!matName.empty()) {
          matVal.SetString(matName.c_str(), (rapidjson::SizeType)matName.size(), allocator);
        }
        else {
          matVal.SetString("");
        }
        jsonMesh.AddMember("material", matVal, allocator);
      }

      // bones

      // animations
      jsonMeshes.PushBack(jsonMesh, allocator);
    }

    d.AddMember("meshes", jsonMeshes, allocator);

    // create a blob from our resources
    OfStream ofs(Utils::format("%s/%s.cym",
      f.parentDirectory().path().c_str(),
      p.baseName().c_str()).c_str());
    JSONOStream os(ofs);
    JSONWriter<JSONOStream> writer(os);
    d.Accept(writer);

    return reinterpret_cast<void*>(new String(d.stringify()));
  }

  SHADER_TYPE::E detectShaderTypeFromFirstLine(const String& firstLine,
    const SHADER_LANGUAGE::E& language) {
    // Convert first line to lowercase for case-insensitive comparison
    String lineLower = firstLine;
    lineLower = Utils::toLowerCase(lineLower);
    //   std::transform(lineLower.begin(), lineLower.end(), lineLower.begin(),
    //     [](unsigned char c) { return std::tolower(c); });

      // Remove leading/trailing whitespace
    lineLower = lineLower.substr(lineLower.find_first_not_of(" \t"));
    lineLower = lineLower.substr(0, lineLower.find_last_not_of(" \t") + 1);

    // Check for explicit type declarations
    if (lineLower.find("//type:") == 0 || lineLower.find("#type") == 0) {
      String typeStr = lineLower.substr(lineLower.find(':') + 1);
      typeStr = typeStr.substr(0, typeStr.find_first_of(" \t\n\r"));

      if (typeStr == "vertex") return SHADER_TYPE::E::VERTEX;
      if (typeStr == "pixel" || typeStr == "fragment") return SHADER_TYPE::E::PIXEL;
      if (typeStr == "geometry") return SHADER_TYPE::E::GEOMETRY;
      if (typeStr == "compute") return SHADER_TYPE::E::COMPUTE;
      if (typeStr == "domain") return SHADER_TYPE::E::TDOMAIN;
      if (typeStr == "hull") return SHADER_TYPE::E::THULL;
      if (typeStr == "tesscontrol") return SHADER_TYPE::E::TCONTROL;
      if (typeStr == "tesseval") return SHADER_TYPE::E::TEVAL;
    }

    // Fallback to extension-based detection if no explicit type
    return SHADER_TYPE::E::UNKNOWN;
  }

  SHADER_TYPE::E determineShaderType(const Path& filename, const String& firstLine) {
    // First try explicit type declaration
    SHADER_LANGUAGE::E language = SHADER_LANGUAGE::E::UNKNOWN;

    if (filename.extension() == String(".hlsl")) language = SHADER_LANGUAGE::E::HLSL;
    else if (filename.extension() == String(".glsl")) language = SHADER_LANGUAGE::E::GLSL;
    else if (filename.extension() == String(".vert") ||
      filename.extension() == String(".vs_hlsl") ||
      filename.extension() == String(".vs_glsl")) return SHADER_TYPE::E::VERTEX;
    else if (filename.extension() == String(".frag") ||
      filename.extension() == String(".ps_hlsl") ||
      filename.extension() == String(".ps_glsl") ||
      filename.extension() == String(".pix")) return SHADER_TYPE::E::PIXEL;
    else if (filename.extension() == String(".geom")) return SHADER_TYPE::E::GEOMETRY;
    else if (filename.extension() == String(".comp")) return SHADER_TYPE::E::COMPUTE;
    else if (filename.extension() == String(".tese")) return SHADER_TYPE::E::TEVAL;
    else if (filename.extension() == String(".tesc")) return SHADER_TYPE::E::TCONTROL;
    else if (filename.extension() == String(".hs")) return SHADER_TYPE::E::THULL;
    else if (filename.extension() == String(".ds")) return SHADER_TYPE::E::TDOMAIN;

    // If we have language info but no extension hint, use first line content
    if (language != +SHADER_LANGUAGE::E::UNKNOWN) {
      SHADER_TYPE::E stage = detectShaderTypeFromFirstLine(firstLine, language);
      if (stage != +SHADER_TYPE::E::UNKNOWN) return stage;
    }

    // Final fallback - check for common patterns in first line
    String lineLower = firstLine;
    lineLower = Utils::toLowerCase(lineLower);

    if (lineLower.find("vertex") != String::npos) return SHADER_TYPE::E::VERTEX;
    if (lineLower.find("pixel") != String::npos || lineLower.find("fragment") != String::npos)
      return SHADER_TYPE::E::PIXEL;
    if (lineLower.find("geometry") != String::npos) return SHADER_TYPE::E::GEOMETRY;
    if (lineLower.find("compute") != String::npos) return SHADER_TYPE::E::COMPUTE;

    return SHADER_TYPE::E::UNKNOWN;
  }

  void*
    ShaderCodec::decode(const File& f) {

    // Create a copy so there is no dangling pointers
    Path p(f.path());
    String tmp = f.readFile();


    // This is a temporal hack which SHOULD work
    void* data = reinterpret_cast<void*>(tmp.c_str()[0]);



    bool isBlob = p.extension().compare(".blob") == 0;

    JSONDocument d;
    d.SetObject();

    SHADER_TYPE::E shaderType = SHADER_TYPE::E::UNKNOWN;
    SHADER_LANGUAGE::E shaderLanguage = SHADER_LANGUAGE::E::UNKNOWN;

    if (p.extension() == String(".cysl")) {
      // Shader is Cyllene Shader Language and inside tells wtf is extract from there
    }
    else {

      IStringStream iss(tmp);

      String line;
      std::getline(iss, line);

      shaderType = determineShaderType(p, line);
    }

    JSONDocument::AllocatorType& allocator = d.GetAllocator();
    d.AddMember("type", "shader", allocator);
    d.AddMember("shaderType", shaderType._to_integral(), allocator);
    d.AddMember("isBlob", isBlob, allocator);
    d.AddMember("data", tmp, allocator);


    // return REINTERPRETPOINTER(Resource, newResource);
    return reinterpret_cast<void*>(new String(d.stringify()));

  }

// void
// cleanupffmpeg(AVFormatContext* formatCtx,
//               AVCodecContext* codecCtx, 
//               SwrContext* swrCtx, 
//               AVFrame* frame,
//               AVPacket* packet) {
//   if (packet) av_packet_free(&packet);
//   if (frame) av_frame_free(&frame);
//   if (swrCtx) swr_free(&swrCtx);
//   if (codecCtx) avcodec_free_context(&codecCtx);
//   if (formatCtx) avformat_close_input(&formatCtx);
// }
// 
// 
//   
// void*
// AudioCodec::decode(const File& f) {
//   AVFormatContext* formatCtx = nullptr;
//   AVCodecContext* codecCtx = nullptr;
//   SwrContext* swrCtx = nullptr;
//   AVFrame* frame = nullptr;
//   AVPacket* packet = nullptr;
//   AudioData* audioData = nullptr;
// 
//   // auto cleanup = [&]() {
//   //   if (packet) av_packet_free(&packet);
//   //   if (frame) av_frame_free(&frame);
//   //   if (swrCtx) swr_free(&swrCtx);
//   //   if (codecCtx) avcodec_free_context(&codecCtx);
//   //   if (formatCtx) avformat_close_input(&formatCtx);
//   // };
// 
//   auto logError = [&](const String& msg) {
//     Logger::instance().logError(Utils::format("%s [%s]", msg.c_str(), f.path().c_str()));
//     };
// 
//   if (avformat_open_input(&formatCtx, f.path().c_str(), nullptr, nullptr) < 0) {
//     logError("Could not open audio file");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
//     logError("Could not read audio stream info");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   int audioStreamIndex = -1;
//   for (uint32 i = 0; i < formatCtx->nb_streams; ++i) {
//     if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//       audioStreamIndex = static_cast<int>(i);
//       break;
//     }
//   }
// 
//   if (audioStreamIndex == -1) {
//     logError("No audio stream found in file");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   AVCodecParameters* codecpar = formatCtx->streams[audioStreamIndex]->codecpar;
//   const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
//   if (!codec) {
//     logError("Unsupported audio codec");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   codecCtx = avcodec_alloc_context3(codec);
//   if (!codecCtx) {
//     logError("Failed to allocate codec context");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   if (avcodec_parameters_to_context(codecCtx, codecpar) < 0) {
//     logError("Failed to fill codec context");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
//     logError("Failed to open codec");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   int sampleRate = codecCtx->sample_rate;
//   int channels = codecCtx->ch_layout.nb_channels;
//   AVChannelLayout inLayout;
//   if (codecCtx->ch_layout.nb_channels > 0) {
//     av_channel_layout_copy(&inLayout, &codecCtx->ch_layout);
//   } else {
//     av_channel_layout_default(&inLayout, channels);
//   }
//   AVChannelLayout outLayout;
//   av_channel_layout_copy(&outLayout, &inLayout);
// 
//   int swrRes = swr_alloc_set_opts2(&swrCtx,
//                                    &outLayout,
//                                    AV_SAMPLE_FMT_FLT,
//                                    sampleRate,
//                                    &inLayout,
//                                    codecCtx->sample_fmt,
//                                    codecCtx->sample_rate,
//                                    0,
//                                    nullptr);
//   if (swrRes < 0 || !swrCtx || swr_init(swrCtx) < 0) {
//     logError("Failed to initialize resampler");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   frame = av_frame_alloc();
//   packet = av_packet_alloc();
//   if (!frame || !packet) {
//     logError("Failed to allocate ffmpeg frame or packet");
//     cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//     return nullptr;
//   }
// 
//   const int outChannels = outLayout.nb_channels;
//   channels = outChannels;
//   Vector<float> samples;
// 
//   auto convertFrame = [&](AVFrame* inputFrame) -> bool {
//     int maxOutSamples = swr_get_out_samples(swrCtx, inputFrame->nb_samples);
//     if (maxOutSamples <= 0) return false;
// 
//     Vector<float> converted;
//     converted.resize(maxOutSamples * outChannels);
//     uint8_t* outData[1] = { reinterpret_cast<uint8_t*>(converted.data()) };
// 
//     int convertedSamples = swr_convert(swrCtx,
//                                        outData,
//                                        maxOutSamples,
//                                        inputFrame->data,
//                                        inputFrame->nb_samples);
//     if (convertedSamples < 0) return false;
// 
//     converted.resize(convertedSamples * outChannels);
//     samples.insert(samples.end(), converted.begin(), converted.end());
//     return true;
//   };
// 
//   int readRet = 0;
//   while ((readRet = av_read_frame(formatCtx, packet)) >= 0) {
//     if (packet->stream_index != audioStreamIndex) {
//       av_packet_unref(packet);
//       continue;
//     }
// 
//     if (avcodec_send_packet(codecCtx, packet) < 0) {
//       av_packet_unref(packet);
//       logError("Failed while sending audio packet to decoder");
//       cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//       return nullptr;
//     }
// 
//     av_packet_unref(packet);
// 
//     while (true) {
//       int receiveRet = avcodec_receive_frame(codecCtx, frame);
//       if (receiveRet == AVERROR(EAGAIN) || receiveRet == AVERROR_EOF) break;
//       if (receiveRet < 0) {
//         logError("Failed while receiving decoded audio frame");
//         cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//         return nullptr;
//       }
// 
//       if (!convertFrame(frame)) {
//         logError("Failed while converting audio frame");
//         cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//         return nullptr;
//       }
// 
//       av_frame_unref(frame);
//     }
//   }
// 
//   avcodec_send_packet(codecCtx, nullptr);
//   while (true) {
//     int receiveRet = avcodec_receive_frame(codecCtx, frame);
//     if (receiveRet == AVERROR(EAGAIN) || receiveRet == AVERROR_EOF) break;
//     if (receiveRet < 0) {
//       logError("Failed while flushing audio decoder");
//       cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//       return nullptr;
//     }
// 
//     if (!convertFrame(frame)) {
//       logError("Failed while converting flushed audio frame");
//       cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//       return nullptr;
//     }
// 
//     av_frame_unref(frame);
//   }
// 
//   audioData = new AudioData();
//   audioData->m_sampleRate = sampleRate;
//   audioData->m_channels = channels;
//   audioData->m_samples = std::move(samples);
// 
//   cleanupffmpeg(formatCtx, codecCtx, swrCtx, frame, packet);
//   return reinterpret_cast<void*>(audioData);
// }

void*
MaterialCodec::decode(const File& f) {
  // Read material JSON and pass the string to RMaterial
  String content = f.readFile();
  return reinterpret_cast<void*>(new String(content));
}

String
MaterialCodec::encodeToJSON(const RMaterial& material) {
  JSONDocument d; d.SetObject();
  auto& allocator = d.GetAllocator();
  d.AddMember("type", "material", allocator);

  String shaderName = "";
  if (!material.getBaseShader().expired()) {
    shaderName = material.getBaseShader().lock()->getName();
  }
  d.AddMember("baseShader", shaderName, allocator);

  JSONValue jvals; jvals.SetObject();
  for (const auto& kv : material.getDefaultValues()) {
    String key = kv.first;
    RTexture* tex = reinterpret_cast<RTexture*>(kv.second);
    if (tex) {
      JSONValue keyValue(key.c_str(), allocator);
      JSONValue valueValue(tex->getName().c_str(), allocator);
      jvals.AddMember(keyValue, valueValue, allocator);
    }
  }
  d.AddMember("values", jvals, allocator);

  return d.prettyString();
}

bool
MaterialCodec::saveToFile(const RMaterial& material, const String& filePath) {
  String json = encodeToJSON(material);
  std::ofstream ofs(filePath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
  if (!ofs.is_open()) return false;
  ofs.write(json.c_str(), (std::streamsize)json.size());
  ofs.close();
  return true;
}


void
MaterialCodec::encode(const Path& pathToResource, void* data) {
  RMaterial* material = reinterpret_cast<RMaterial*>(data);
  saveToFile(*material, pathToResource.fullPath());
}

}
