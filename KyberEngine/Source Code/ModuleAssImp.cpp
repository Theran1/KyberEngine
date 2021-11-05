#include "ModuleAssImp.h"
#include "Application.h"

#include "Primitive.h"
//#include "ModuleScene.h"

#include "cimport.h"
#include "scene.h"
#include "postprocess.h"

ModuleAssImp::ModuleAssImp(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleAssImp::~ModuleAssImp()
{}

bool ModuleAssImp::Init()
{
	LOG("Init Assets Importer");

	// AssImp
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleAssImp::Start()
{
	LoadMesh("Assets/Meshes/baker_house.fbx");

	return true;
}

bool ModuleAssImp::CleanUp()
{
	LOG("Destroying Assets Importer");

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleAssImp::LoadMesh(const char* filePath)
{
	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh* tmpMesh = new Mesh;

			tmpMesh->numVertices = scene->mMeshes[i]->mNumVertices;
			tmpMesh->vertices = new float[tmpMesh->numVertices * 3];
			memcpy(tmpMesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * tmpMesh->numVertices * 3);

			LOG("New mesh with %d vertices", tmpMesh->numVertices);

			if (scene->mMeshes[i]->HasFaces())
			{
				tmpMesh->numIndices = scene->mMeshes[i]->mNumFaces * 3;
				tmpMesh->indices = new uint[tmpMesh->numIndices]; // Each face is a triangle

				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&tmpMesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
			}

			// Copying Texture coordinates
			if (scene->mMeshes[i]->mTextureCoords != NULL)
			{
				tmpMesh->textureCoords = new float[tmpMesh->numVertices * 2];

				for (uint j = 0; j < tmpMesh->numVertices; j++)
				{
					tmpMesh->textureCoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					tmpMesh->textureCoords[(j * 2) + 1] = 1.0f - scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}
			else
			{
				LOG("Warning, No texture coordinates found");
			}


			if (scene->mMeshes[i]->mTextureCoords != NULL)
			{
				tmpMesh->normals = new float[tmpMesh->numVertices * 3];

				for (uint j = 0; j < tmpMesh->numVertices; j++)
				{
					tmpMesh->normals[j * 2] = scene->mMeshes[i]->mNormals[j].x;
					tmpMesh->normals[(j * 2) + 1] = scene->mMeshes[i]->mNormals[j].y;
					tmpMesh->normals[(j * 2) + 2] = scene->mMeshes[i]->mNormals[j].z;
				}
			}
			else
			{
				LOG("Warning, No Normal coordinates found");
			}

			scene->mMaterials[0]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &aiString::aiString("Assets/Textures/baker_house.png"));
			tmpMesh->textureID = tmpMesh->CreateTexture("Assets/Textures/baker_house.png");

			tmpMesh->Initialize();
			meshList.push_back(tmpMesh);
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", filePath);
}