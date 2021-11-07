#include "ModuleAssImp.h"
#include "Application.h"

#include "Primitive.h"

#include "cimport.h"
#include "scene.h"
#include "postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	return true;
}

bool ModuleAssImp::CleanUp()
{
	LOG("Destroying Assets Importer");

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleAssImp::LoadMesh(const char* meshFilename)
{
	const aiScene* scene = aiImportFile(meshFilename, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh* mesh = new Mesh;

			mesh->numVertices = scene->mMeshes[i]->mNumVertices;
			mesh->vertices = new float[mesh->numVertices * 3];
			memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->numVertices * 3);

			LOG("New mesh with %d vertices", mesh->numVertices);

			if (scene->mMeshes[i]->HasFaces())
			{
				mesh->numIndices = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indices = new uint[mesh->numIndices]; // Each face is a triangle

				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
			}

			// Copying Texture coordinates
			if (scene->mMeshes[i]->mTextureCoords != NULL)
			{
				mesh->textureCoords = new float[mesh->numVertices * 2];

				for (uint j = 0; j < mesh->numVertices; j++)
				{
					mesh->textureCoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					mesh->textureCoords[(j * 2) + 1] = 1.0f - scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}
			else
				LOG("Warning, No texture coordinates found");

			if (scene->mMeshes[i]->mTextureCoords != NULL)
			{
				mesh->normals = new float[mesh->numVertices * 3];

				for (uint j = 0; j < mesh->numVertices; j++)
				{
					mesh->normals[j * 2] = scene->mMeshes[i]->mNormals[j].x;
					mesh->normals[(j * 2) + 1] = scene->mMeshes[i]->mNormals[j].y;
					mesh->normals[(j * 2) + 2] = scene->mMeshes[i]->mNormals[j].z;
				}
			}
			else
				LOG("Warning, No Normal coordinates found");

			// Assign Mesh Texture
			scene->mMaterials[0]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &aiString::aiString("Assets/Textures/baker_house.png"));
			mesh->textureID = LoadTexture("Assets/Textures/baker_house.png");

			// Create Checkers Texture
			mesh->checkerTextureID = mesh->CreateCheckerTexture();

			mesh->Initialize();
			App->scene->meshList.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", meshFilename);
}

uint ModuleAssImp::LoadTexture(const char* path)
{
	uint textureID;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int height, width, compPerPixel;
	unsigned char* textureData = stbi_load(path, &width, &height, &compPerPixel, STBI_rgb);
	GLint internalFormat = GL_RGBA;
	if (compPerPixel == 3) internalFormat = GL_RGB;

	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, textureData);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		LOG("Failed to create texture");

	stbi_image_free(textureData);

	return textureID;
}