This is an overview file for loading, manipulating and drawing models

Associated trello: https://trello.com/b/G5ARxeUI/model

Goal:
    Ability to load any .obj model that doesn't use freeform geometry
    Full material property support
    Easy setup with textures
    Ability to draw models in an instanced fashion
    Ability to modify and batch load new vertices and polygons

Shader architecture:
    The model will have 3 vertex attributes: pos, normal and TexCoord (core)
    You should be able to load extra information and thus extend these 3 attributes (less important)
    Model should take into consideration the amount of bindable textures and uniforms (not important)
    
    The model will have certain uniforms available for the shader used. These are:
        materialN.ambient; The ambient color of the material (less important)
        materialN.diffuse; The diffuse color of the material (will be used for ambient and specular if otherwise is not specified)
        materialN.specular; The Specular color of the material (less important)
        materialN.specularStrength; The specular strength of the material  (important)
        materialN.opacity; The opacity of the material (less important)
        materialN.ambientTexture; Texture for ambient color, should be multiplied with materialN.ambient
        materialN.diffuseTexture; Texture for diffuse color, should be multiplied with materialN.diffuse
        materialN.specularTexture; Texture for specular color, should be multiplied with materialN.specular

        N is used to allow multiple materials in the same drawcall

Backend:
    Model should be saved as an abstraction and only convert into triangles on GPU load (core)
    You should be able to export and save model as only triangles
    Model should be able to calculate normals from order of vertices using right hand rule (core)
    Model should be stored in seperate groups for easy batching and optimization for textures and the like
    Lowest type is polygon which doesn't contain anything except external types

Structure:
    Polygons:
        Contains the connections between texture and spacial coordinates

    Groups:
        Contains the material properties
        Non-static, used to assemble optimized index buffers

    Mesh:
        Contains all the texture coordinates and polygons
        Mostly static part

    Model:
        Contains Meshes
        Keeps care of all the buffers
        The interface to the user

Model can ask Group for indices. When this is done, indices will ask for buffer location of different polygons. It will then return
the correct indices. Model can ask Group to bind eventual textures and bind eventual uniforms. It can also ask group of how many texture slots it needs, for proper drawcall splitting in case of a lack of texture slots. It is assumed at this time that a shader is already assigned, and the Group won't use one itself.